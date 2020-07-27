#include "controller.h"
#include "genresitem.h"
#include "item.h"
#include "queueditem.h"
#include "strformats.h"
#include <mpd/client.h>
#include <QCollator>
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

void Controller::moveSongs(const QVector<QPair<unsigned, unsigned>> &sources, unsigned to)
{
    // The pairs are (source row, song id). They are expected to be sorted by row.

    if (!m_connection) {
        return;
    }

    disableIdle();

    // As far as I know, Quetzalcoatl was the first Qt program to have a drag-and-drop-
    // rearrangeable playlist. This has always been one of the trickier parts of the
    // program to get right.

    // Keep in mind that it supports dragging multiple, non-continguous blocks of items.

    // I haven't found a problem with the code below yet...

    for (auto it = sources.cbegin(); it != sources.cend() && it->first < to; it++) {
        if (!mpd_run_move_id(m_connection, it->second, to - 1)) {
            emit errorMessage(mpd_connection_get_error_message(m_connection));
            return;
        }
    }

    for (auto it = sources.crbegin(); it != sources.crend() && it->first >= to; it++) {
        if (!mpd_run_move_id(m_connection, it->second, to)) {
            emit errorMessage(mpd_connection_get_error_message(m_connection));
            return;
        }
    }

    enableIdle();
}

// TODO: Obviously, searchTags and searchSongs should be combined.

QVector<mpd_song *> Controller::searchSongs(const QVector<QPair<mpd_tag_type, QString>> &tags)
{
    QVector<mpd_song *> songs;

    if (!m_connection) {
        return songs;
    }

    disableIdle();

    if (!mpd_search_db_songs(m_connection, true)) {
        emit errorMessage(mpd_connection_get_error_message(m_connection));
        return songs;
    }
    for (auto tag : tags) {
        if (!mpd_search_add_tag_constraint(m_connection,
                                           MPD_OPERATOR_DEFAULT,
                                           tag.first,
                                           tag.second.toUtf8().constData())) {
            emit errorMessage(mpd_connection_get_error_message(m_connection));
            return songs;
        }
    }

    if (!mpd_search_commit(m_connection)) {
        emit errorMessage(mpd_connection_get_error_message(m_connection));
    }

    mpd_song *song = nullptr;

    while ((song = mpd_recv_song(m_connection))) {
        songs.append(song);
    }

    enableIdle();

    return songs;
}

QVector<QString> Controller::searchTags(mpd_tag_type tagType,
                                        const QVector<QPair<mpd_tag_type, QString>> criteria)
{
    // The "criteria" is the search filter. To list all songs on Ice T's
    // Power album, the tagType would be MPD_TAG_ALBUM, and the criteria would be
    // QVector(QPair(MPD_TAG_ARTIST, "Ice T")).

    QVector<QString> tags;

    if (!m_connection) {
        return tags;
    }

    disableIdle();

    if (!mpd_search_db_songs(m_connection, true)) {
        emit errorMessage(mpd_connection_get_error_message(m_connection));
        return tags;
    }

    for (auto &tag : criteria) {
        if (!mpd_search_add_tag_constraint(m_connection,
                                           MPD_OPERATOR_DEFAULT,
                                           tag.first,
                                           tag.second.toUtf8().constData())) {
            emit errorMessage(mpd_connection_get_error_message(m_connection));
            return tags;
        }
    }

    if (!mpd_search_commit(m_connection)) {
        emit errorMessage(mpd_connection_get_error_message(m_connection));
    }

    QSet<QString> tagSet;

    mpd_song *song;
    QString tag;
    while ((song = mpd_recv_song(m_connection))) {
        tag = mpd_song_get_tag(song, tagType, 0);
        mpd_song_free(song);
        if (!tag.trimmed().isEmpty()) {
            tagSet.insert(tag);
        };
    }

    enableIdle();

    for (auto tag : tagSet) {
        tags.append(tag);
    }

    std::sort(tags.begin(), tags.end(), QCollator());

    return tags;
}

QVector<mpd_playlist *> Controller::listPlaylists()
{
    QVector<mpd_playlist *> playlists;

    if (!m_connection) {
        return playlists;
    }
    disableIdle();

    if (!mpd_send_list_playlists(m_connection)) {
        emit errorMessage(mpd_connection_get_error_message(m_connection));
        return playlists;
    }

    mpd_playlist *playlist = nullptr;

    while ((playlist = mpd_recv_playlist(m_connection))) {
        playlists.append(playlist);
    }

    enableIdle();
    return playlists;
}

QVector<mpd_entity *> Controller::listPlaylist(mpd_playlist *playlist)
{
    QVector<mpd_entity *> songs;
    if (!m_connection) {
        return songs;
    }

    if (!playlist) {
        return songs;
    }

    disableIdle();

    if (!mpd_send_list_playlist_meta(m_connection, mpd_playlist_get_path(playlist))) {
        emit errorMessage(mpd_connection_get_error_message(m_connection));
        return songs;
    }

    mpd_entity *entity = nullptr;
    while ((entity = mpd_recv_entity(m_connection))) {
        if (mpd_entity_get_type(entity) == MPD_ENTITY_TYPE_SONG) {
            songs.append(entity);
        }
    }

    enableIdle();

    return songs;
}

QVector<mpd_entity *> Controller::listSongs()
{
    QVector<mpd_entity *> entities;
    if (!m_connection) {
        return entities;
    }

    disableIdle();

    if (!mpd_send_list_all_meta(m_connection, nullptr)) {
        emit errorMessage(mpd_connection_get_error_message(m_connection));
        return entities;
    }

    mpd_entity *entity = nullptr;
    while ((entity = mpd_recv_entity(m_connection))) {
        if (mpd_entity_get_type(entity) == MPD_ENTITY_TYPE_SONG) {
            entities.append(entity);
        } else {
            mpd_entity_free(entity);
        }
    }

    enableIdle();
    QCollator collator;

    std::sort(entities.begin(), entities.end(), [&collator](mpd_entity *a, mpd_entity *b) {
        return collator.compare(songEntityLabel(a), songEntityLabel(b)) < 0;
    });

    return entities;
}

QVector<mpd_entity *> Controller::listDir(mpd_entity *entity)
{
    QVector<mpd_entity *> listing;

    if (!m_connection) {
        return listing;
    }

    auto directory = entity && mpd_entity_get_type(entity) == MPD_ENTITY_TYPE_DIRECTORY
                         ? mpd_entity_get_directory(entity)
                         : nullptr;
    auto path = directory ? mpd_directory_get_path(directory) : nullptr;

    disableIdle();

    mpd_entity *newEntity = nullptr;

    if (!mpd_send_list_all_meta(m_connection, path)) {
        emit errorMessage(mpd_connection_get_error_message(m_connection));
        return listing;
    }
    while ((newEntity = mpd_recv_entity(m_connection))) {
        auto entityType = mpd_entity_get_type(newEntity);
        if (entityType == MPD_ENTITY_TYPE_SONG || entityType == MPD_ENTITY_TYPE_DIRECTORY) {
            listing.append(newEntity);
        } else {
            mpd_entity_free(newEntity);
        }
    }

    enableIdle();

    QCollator collator;
    std::stable_sort(listing.begin(), listing.end(), [&collator](mpd_entity *a, mpd_entity *b) {
        return collator.compare(dirEntryLabel(a), dirEntryLabel(b)) < 0;
    });

    std::stable_sort(listing.begin(), listing.end(), [=](mpd_entity *a, mpd_entity *b) {
        return mpd_entity_get_type(a) < mpd_entity_get_type(b);
    });

    return listing;
}

void Controller::queueUris(const QVector<QString> &uris, int row)
{
    if (!m_connection) {
        return;
    }
    auto to = static_cast<unsigned>(row);

    disableIdle();
    for (auto it = uris.crbegin(); it != uris.crend(); it++) {
        if ((mpd_run_add_id_to(m_connection, it->toUtf8().constData(), to)) == -1) {
            emit errorMessage(mpd_connection_get_error_message(m_connection));
            return;
        }
    }
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
    auto elapsed = mpd_status_get_elapsed_ms(status) / 1000;
    emit sliderValue(elapsed);

    emit statusMessage(QStringLiteral("%1/%2").arg(timeStr(elapsed), timeStr(total)), 0);

    unsigned queueVersion = mpd_status_get_queue_version(status);

    if (m_queueVersion != queueVersion) {
        m_queueVersion = queueVersion;

        if (!mpd_send_list_queue_meta(m_connection)) {
            emit errorMessage(mpd_connection_get_error_message(m_connection));
            return;
        }

        QVector<Item *> queue;

        mpd_entity *entity = nullptr;
        while ((entity = mpd_recv_entity(m_connection)) != nullptr) {
            if (mpd_entity_get_type(entity) == MPD_ENTITY_TYPE_SONG) {
                queue.append(new QueuedItem(entity));
            }
        }

        if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
            emit errorMessage(mpd_connection_get_error_message(m_connection));
            return;
        }

        emit queueChanged(queue);
    }

    emit shuffled(mpd_status_get_random(status));
    emit repeating(mpd_status_get_repeat(status));
    emit volume(mpd_status_get_volume(status));
    emit crossfade(mpd_status_get_crossfade(status));
    emit songId(mpd_status_get_song_id(status));

    mpd_status_free(status);
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

QSocketNotifier *Controller::notifier() const
{
    return m_notifier;
}

mpd_connection *Controller::mpd() const
{
    return m_connection;
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

        QVector<Item *> emptyQueue;
        emit queueChanged(emptyQueue);

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

QVector<QString> Controller::listTags(mpd_tag_type tagType)
{
    QVector<QString> tags;

    if (!m_connection) {
        return tags;
    }

    disableIdle();

    if (!mpd_search_db_tags(m_connection, tagType)) {
        qDebug() << mpd_connection_get_error_message(m_connection);
        return tags;
    }

    if (!mpd_search_commit(m_connection)) {
        qDebug() << mpd_connection_get_error_message(m_connection);
        return tags;
    }

    struct mpd_pair *pair = nullptr;
    while ((pair = mpd_recv_pair_tag(m_connection, tagType)) != nullptr) {
        QString tag{pair->value};
        mpd_return_pair(m_connection, pair);
        if (!tag.trimmed().isEmpty()) {
            tags.push_back(tag);
        }
    }

    enableIdle();

    QCollator collator;
    std::sort(tags.begin(), tags.end(), collator);
    return tags;
}

void Controller::playSongEntity(mpd_entity *entity)
{
    if (!m_connection) {
        return;
    }
    if (!entity) {
        return;
    }

    if (mpd_entity_get_type(entity) != MPD_ENTITY_TYPE_SONG) {
        return;
    }
    disableIdle();

    auto song = mpd_entity_get_song(entity);
    auto songid = mpd_song_get_id(song);

    if (!mpd_run_play_id(m_connection, songid)) {
        emit errorMessage(mpd_connection_get_error_message(m_connection));
    }
    enableIdle();
}

void Controller::playAlbum(const QVector<QString> &uris)
{
    if (!m_connection) {
        return;
    }

    disableIdle();
    if (!mpd_run_clear(m_connection)) {
        emit errorMessage(mpd_connection_get_error_message(m_connection));
        return;
    }

    for (auto uri : uris) {
        if (!mpd_run_add(m_connection, uri.toUtf8().constData())) {
            emit errorMessage(mpd_connection_get_error_message(m_connection));
            return;
        }
    }

    enableIdle();
}
