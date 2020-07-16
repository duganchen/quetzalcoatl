#include "controller.h"
#include "dbitem.h"
#include "songitem.h"
#include <mpd/client.h>
#include <QDebug>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QLocalSocket>
#include <QtNetwork/QTcpSocket>

Controller::Controller(QObject *parent)
    : QObject(parent)
    , m_connection(nullptr)
    , m_notifier(nullptr)
    , m_queueVersion(0)
{
    auto dbRootItem = new DBItem(QIcon(), "");
    dbRootItem->append(new DBItem(QIcon(":/icons/folder-favorites.svg"), "Playlists"));
    dbRootItem->append(new DBItem(QIcon(":/icons/server-database.svg"), "Albums"));
    dbRootItem->append(new DBItem(QIcon(":/icons/server-database.svg"), "Compilations"));
    dbRootItem->append(new DBItem(QIcon(":/icons/server-database.svg"), "Songs"));
    dbRootItem->append(new DBItem(QIcon(":/icons/server-database.svg"), "Genres"));
    dbRootItem->append(new DBItem(QIcon(":/icons/server-database.svg"), "Composers"));
    dbRootItem->append(new DBItem(QIcon(":/icons/drive-harddisk"), "/"));
    m_databaseItems = new Items(dbRootItem);

    auto playlistRootItem = new DBItem(QIcon(), "");
    m_playlistItems = new Items(playlistRootItem);

    qRegisterMetaType<Controller::ConnectionState>();

    auto settings = mpd_settings_new(nullptr, 0, 0, nullptr, nullptr);
    m_defaultHost = mpd_settings_get_host(settings);
    m_defaultPort = mpd_settings_get_port(settings);
    m_defaultTimeout = mpd_settings_get_timeout_ms(settings);
    mpd_settings_free(settings);
}

Controller::~Controller()
{
    if (m_connection) {
        mpd_connection_free(m_connection);
    }
}

void Controller::connectToMPD(QString host, int port, int timeout_ms)
{
    emit connectionState(ConnectionState::Connecting);

    // This is literally how libmpdclient determines if a host is a Unix socket,
    // at least as of 2.18.
    if (host.startsWith("/") || host.startsWith("@")) {
        createMPD(host, port, 0);

    } else {
        // libmpdclient's internal host address resolution is blocking and
        // can take a long time to return if it fails. We use Qt to confirm that we
        // can resolve the host first.
        QHostInfo::lookupHost(host, [=](QHostInfo hostInfo) {
            if (hostInfo.error() == QHostInfo::NoError) {
                createMPD(host, port, 0);
            } else {
                // The expected error is QHostInfo::HostNotFound, "Host not found".
                emit connectionErrorMessage(hostInfo.errorString());
                emit connectionState(ConnectionState::Disconnected);
            }
        });
    }
}

void Controller::updateStatus()
{
    disableIdle();
    pollForStatus();
    enableIdle();
}

void Controller::pollForStatus()
{
    if (!m_connection) {
        return;
    }

    auto status = mpd_run_status(m_connection);
    if (nullptr == status) {
        auto msg = mpd_connection_get_error_message(m_connection);
        emit errorMessage(msg);
        return;
    }

    auto total = mpd_status_get_total_time(status);
    emit sliderMax(total);
    auto elapsed = mpd_status_get_elapsed_time(status);
    emit sliderValue(elapsed);

    unsigned queueVersion = mpd_status_get_queue_version(status);
    if (m_queueVersion != queueVersion) {
        m_queueVersion = queueVersion;

        if (!mpd_send_list_queue_meta(m_connection)) {
            emit errorMessage(mpd_connection_get_error_message(m_connection));
            return;
        }

        emit m_playlistItems->modelAboutToBeReset();

        clearQueue();

        mpd_entity *entity = nullptr;
        while ((entity = mpd_recv_entity(m_connection)) != nullptr) {
            if (mpd_entity_get_type(entity) == MPD_ENTITY_TYPE_SONG) {
                m_playlistItems->rootItem()->append(
                    new SongItem(QIcon(":/icons/audio-x-generic.svg"), entity));
            }
        }

        emit m_playlistItems->modelReset();

        if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
            emit errorMessage(mpd_connection_get_error_message(m_connection));
            return;
        }
    }

    mpd_status_free(status);
}

void Controller::clearQueue()
{
    emit m_playlistItems->modelAboutToBeReset();
    m_playlistItems->rootItem()->clear();
    emit m_playlistItems->modelReset();
}

void Controller::handleListAlbumsClick()
{
    if (!m_connection) {
        return;
    }

    for (auto album : getAlbumList()) {
        qDebug() << album;
    }
}

QVector<QString> Controller::getAlbumList()
{
    QVector<QString> albums;

    if (!m_connection) {
        return albums;
    }

    disableIdle();

    if (!mpd_search_db_tags(m_connection, MPD_TAG_ALBUM)) {
        qDebug() << mpd_connection_get_error_message(m_connection);
        return albums;
    }

    if (!mpd_search_commit(m_connection)) {
        qDebug() << mpd_connection_get_error_message(m_connection);
        return albums;
    }

    struct mpd_pair *pair = nullptr;
    while ((pair = mpd_recv_pair_tag(m_connection, MPD_TAG_ALBUM)) != nullptr) {
        albums.push_back(pair->value);
        mpd_return_pair(m_connection, pair);
    }

    enableIdle();

    return albums;
}

QString Controller::defaultHost()
{
    return m_defaultHost;
}

unsigned Controller::defaultPort()
{
    return m_defaultPort;
}

void Controller::handleIdle(mpd_idle idle)
{
    if (mpd_connection_get_error(m_connection) == MPD_ERROR_CLOSED) {
        m_notifier->setEnabled(false);
        m_notifier->deleteLater();
        m_notifier = nullptr;

        m_queueVersion = 0;

        // The expected error message, should you query it with
        // mpd_connection_get_error_message, is
        // "Connection closed by the server"

        mpd_connection_free(m_connection);
        m_connection = nullptr;
        emit connectionState(ConnectionState::Disconnected);
        emit sliderMax(0);
        emit sliderValue(0);

        clearQueue();

    } else {
        if (idle & MPD_IDLE_DATABASE) {
            qDebug() << "song database has been updated";
        }
        if (idle & MPD_IDLE_STORED_PLAYLIST) {
            qDebug() << "a stored playlist has been modified, created, deleted or renamed";
        }
        if (idle & MPD_IDLE_QUEUE) {
            pollForStatus();
        }
        if (idle & MPD_IDLE_PLAYER) {
            qDebug() << "the player state has changed: play, stop, pause, seek, ...";
            pollForStatus();
        }
        if (idle & MPD_IDLE_MIXER) {
            qDebug() << "the volume has been modified";
        }
        if (idle & MPD_IDLE_OUTPUT) {
            qDebug() << "an audio output device has been enabled or disabled";
        }
        if (idle & MPD_IDLE_OPTIONS) {
            qDebug() << "options have changed: crossfade, random, repeat, ...";
        }
        if (idle & MPD_IDLE_UPDATE) {
            qDebug() << "a database update has started or finished.";
        }
        if (idle & MPD_IDLE_STICKER) {
            qDebug() << "a sticker has been modified.";
        }
        if (idle & MPD_IDLE_SUBSCRIPTION) {
            qDebug() << "a client has subscribed to or unsubscribed from a channel";
        }
        if (idle & MPD_IDLE_MESSAGE) {
            qDebug() << "a message on a subscribed channel was received";
        }
    }
}

void Controller::createMPD(QString host, int port, int timeout_ms)
{
    auto connection = mpd_connection_new(host.toUtf8().constData(), port, timeout_ms);

    if (!connection) {
        emit connectionErrorMessage("Out of memory");
        return;
    }

    m_connection = connection;

    if (mpd_connection_get_error(m_connection) == MPD_ERROR_SUCCESS) {
        emit connectionState(ConnectionState::Connected);

        pollForStatus();

        m_notifier = new QSocketNotifier(mpd_connection_get_fd(m_connection),
                                         QSocketNotifier::Read,
                                         this);

        connect(m_notifier, &QSocketNotifier::activated, this, &Controller::handleActivation);

        mpd_send_idle(m_connection);

    } else {
        // In the case where MPD is not running at the port, which is the expected error,
        // we get a MPD_ERROR_SYSTEM. On Linux and OS X (at least), the error message is
        // "Connection refused".
        emit connectionErrorMessage(mpd_connection_get_error_message(m_connection));
        emit connectionState(ConnectionState::Disconnected);
        // and we don't need to free it. I checked.
        m_connection = nullptr;
    }
}

void Controller::handleActivation()
{
    handleIdle(mpd_recv_idle(m_connection, false));
    if (m_connection) {
        mpd_send_idle(m_connection);
    }
}

Items *Controller::databaseItems() const
{
    return m_databaseItems;
}

Items *Controller::playlistItems() const
{
    return m_playlistItems;
}

// Call these two before and after most synchronous MPD commands.
// Make sure they're called OUTside the idle handler!

void Controller::disableIdle()
{
    if (!m_notifier || !m_connection) {
        return;
    }
    m_notifier->setEnabled(false);
    auto idle = mpd_run_noidle(m_connection);
    handleIdle(idle);
}

void Controller::enableIdle()
{
    if (!m_notifier || !m_connection) {
        return;
    }
    m_notifier->setEnabled(true);
    mpd_send_idle(m_connection);
}
