#include "controller.h"
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
    , m_connectionState(ConnectionState::Disconnected)
{
    m_databaseController = new ItemModelController();
    auto dbRootItem = m_databaseController->rootItem();
    dbRootItem->append(new Item(QIcon(":/icons/folder-favorites.svg"), "Playlists"));
    dbRootItem->append(new Item(QIcon(":/icons/server-database.svg"), "Albums"));
    dbRootItem->append(new Item(QIcon(":/icons/server-database.svg"), "Compilations"));
    dbRootItem->append(new Item(QIcon(":/icons/server-database.svg"), "Songs"));
    dbRootItem->append(new Item(QIcon(":/icons/server-database.svg"), "Genres"));
    dbRootItem->append(new Item(QIcon(":/icons/server-database.svg"), "Composers"));
    dbRootItem->append(new Item(QIcon(":/icons/drive-harddisk"), "/"));

    m_playlistController = new ItemModelController();

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
    qDebug() << "Connecting to MPD";
    setConnectionState(ConnectionState::Connecting);

    // This is literally how libmpdclient determines if a host is a Unix socket,
    // at least as of 2.18.
    if (host.startsWith("/") || host.startsWith("@")) {
        createMPD(host, port, timeout_ms);

    } else {
        // If it's a Tcp socket, then we first use Qt to asynchronously check if we can actually
        // connect, because libmpdclient's internal host address resolution is blocking and
        // can take a long time to return if it fails
        qDebug() << "looking up locahost";
        QHostInfo::lookupHost(host, [=](QHostInfo hostInfo) {
            if (hostInfo.error() == QHostInfo::NoError) {
                createMPD(host, port, timeout_ms);
            } else {
                // The expected error is QHostInfo::HostNotFound, "Host not found".
                emit connectionErrorMessage(hostInfo.errorString());
                setConnectionState(ConnectionState::Disconnected);
            }
        });
    }
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

    m_notifier->setEnabled(false);
    mpd_run_noidle(m_connection);

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

    m_notifier->setEnabled(true);
    mpd_send_idle(m_connection);

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
    qDebug() << "Controller has received an idle";
    if (!m_connection) {
        return;
    }

    if (!idle && mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
        // This means we lost the connection.

        m_queueVersion = 0;

        qDebug() << mpd_connection_get_error_message(m_connection);
        mpd_connection_free(m_connection);
        m_connection = nullptr;
        setConnectionState(ConnectionState::Disconnected);
    }

    if (idle & MPD_IDLE_QUEUE) {
        qDebug() << "THE QUEUE HAS CHANGED";
        emit queueChanged();
    }
}

void Controller::createMPD(QString host, int port, int timeout_ms)
{
    auto connection = mpd_connection_new(host.toUtf8().constData(), port, timeout_ms);

    if (!connection) {
        // I'm not really sure how I'm supposed to handle this, but for now...
        emit connectionErrorMessage("Out of memory");
    }

    if (m_connection) {
        mpd_connection_free(m_connection);
    }

    if (m_notifier) {
        delete m_notifier;
    }

    m_connection = connection;

    if (mpd_connection_get_error(m_connection) == MPD_ERROR_SUCCESS) {
        m_notifier = new QSocketNotifier(mpd_connection_get_fd(m_connection),
                                         QSocketNotifier::Read,
                                         this);

        connect(m_notifier, &QSocketNotifier::activated, this, &Controller::handleActivation);

        mpd_send_idle(m_connection);
        setConnectionState(ConnectionState::Connected);
    } else {
        // In the case where MPD is not running at the port, which is the expected error,
        // we get a MPD_ERROR_SYSTEM with a "Connection refused" message. On Linux and
        // Windows.
        emit connectionErrorMessage(mpd_connection_get_error_message(m_connection));
        setConnectionState(ConnectionState::Disconnected);
        // and we don't need to free it. I checked.
        m_connection = nullptr;
    }
}

void Controller::handleActivation()
{
    handleIdle(mpd_recv_idle(m_connection, false));
}

Controller::ConnectionState Controller::connectionState() const
{
    return m_connectionState;
}

void Controller::setConnectionState(Controller::ConnectionState state)
{
    if (m_connectionState != state) {
        m_connectionState = state;
        emit connectionStateChanged(state);
    }
}

ItemModelController *Controller::databaseController() const
{
    return m_databaseController;
}

ItemModelController *Controller::playlistController() const
{
    return m_playlistController;
}
