#include "controller.h"
#include "genresitem.h"
#include "item.h"
#include "playlistitem.h"
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
    , m_mpdPlayerState(MPD_STATE_UNKNOWN)
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
    if (MPD_STATE_PLAY == m_mpdPlayerState) {
        disableIdle();
        pollForStatus();
        enableIdle();
    }
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
        if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
            emit errorMessage(mpd_connection_get_error_message(m_connection));

            return songs;
        }
    }
    for (auto tag : tags) {
        if (!mpd_search_add_tag_constraint(m_connection,
                                           MPD_OPERATOR_DEFAULT,
                                           tag.first,
                                           tag.second.toUtf8().constData())) {
            if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
                emit errorMessage(mpd_connection_get_error_message(m_connection));
                return songs;
            }
        }
    }

    if (!mpd_search_commit(m_connection)) {
        if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
            emit errorMessage(mpd_connection_get_error_message(m_connection));
            return songs;
        }
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
        if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
            emit errorMessage(mpd_connection_get_error_message(m_connection));
            return tags;
        }
    }

    for (auto &tag : criteria) {
        if (!mpd_search_add_tag_constraint(m_connection,
                                           MPD_OPERATOR_DEFAULT,
                                           tag.first,
                                           tag.second.toUtf8().constData())) {
            if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
                emit errorMessage(mpd_connection_get_error_message(m_connection));
                return tags;
            }
        }
    }

    if (!mpd_search_commit(m_connection)) {
        if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
            emit errorMessage(mpd_connection_get_error_message(m_connection));
        }
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

QVector<Item *> Controller::listPlaylists()
{
    QVector<Item *> playlists;

    if (!m_connection) {
        return playlists;
    }
    disableIdle();

    auto thesePlaylists = listPlaylistsImpl();

    enableIdle();
    return thesePlaylists;
}

QVector<Item *> Controller::listPlaylistsImpl()
{
    QVector<Item *> playlists;
    if (!mpd_send_list_playlists(m_connection)) {
        if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
            emit errorMessage(mpd_connection_get_error_message(m_connection));
            return playlists;
        }
    }

    mpd_playlist *playlist = nullptr;

    while ((playlist = mpd_recv_playlist(m_connection))) {
        playlists.append(new PlaylistItem(playlist));
    }

    if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
        emit errorMessage(mpd_connection_get_error_message(m_connection));
    }

    QCollator collator;
    std::sort(playlists.begin(), playlists.end(), [&collator](Item *a, Item *b) {
        return collator.compare(a->uri(), b->uri()) < 0;
    });
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
        if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
            emit errorMessage(mpd_connection_get_error_message(m_connection));
            return songs;
        }
    }

    mpd_entity *entity = nullptr;
    while ((entity = mpd_recv_entity(m_connection))) {
        if (mpd_entity_get_type(entity) == MPD_ENTITY_TYPE_SONG) {
            songs.append(entity);
        } else {
            mpd_entity_free(entity);
        }
    }

    if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
        emit errorMessage(mpd_connection_get_error_message(m_connection));
        return songs;
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
        if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
            emit errorMessage(mpd_connection_get_error_message(m_connection));
            return entities;
        }
    }

    mpd_entity *entity = nullptr;
    while ((entity = mpd_recv_entity(m_connection))) {
        if (mpd_entity_get_type(entity) == MPD_ENTITY_TYPE_SONG) {
            entities.append(entity);
        } else {
            mpd_entity_free(entity);
        }
    }

    if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
        emit errorMessage(mpd_connection_get_error_message(m_connection));
        return entities;
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
        if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
            emit errorMessage(mpd_connection_get_error_message(m_connection));
            return listing;
        }
    }
    while ((newEntity = mpd_recv_entity(m_connection))) {
        auto entityType = mpd_entity_get_type(newEntity);
        if (entityType == MPD_ENTITY_TYPE_SONG || entityType == MPD_ENTITY_TYPE_DIRECTORY) {
            listing.append(newEntity);
        } else {
            mpd_entity_free(newEntity);
        }
    }

    if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
        emit errorMessage(mpd_connection_get_error_message(m_connection));
        return listing;
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
            if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
                emit errorMessage(mpd_connection_get_error_message(m_connection));
                return;
            }
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
        auto error = mpd_connection_get_error(m_connection);
        if (MPD_ERROR_SUCCESS != error) {
            auto msg = mpd_connection_get_error_message(m_connection);
            emit errorMessage(msg);
            return;
        }
    }

    auto total = mpd_status_get_total_time(status);
    emit sliderMax(total);
    auto elapsed = mpd_status_get_elapsed_ms(status) / 1000;
    emit sliderValue(elapsed);
    emit statusMessage(QStringLiteral("%1/%2").arg(timeStr(elapsed), timeStr(total)), 0);

    emit shuffled(mpd_status_get_random(status));
    emit repeating(mpd_status_get_repeat(status));
    emit volume(mpd_status_get_volume(status));
    emit crossfade(mpd_status_get_crossfade(status));
    emit songId(mpd_status_get_song_id(status));

    m_mpdPlayerState = mpd_status_get_state(status);

    unsigned queueVersion = mpd_status_get_queue_version(status);
    mpd_status_free(status);

    if (m_queueVersion != queueVersion) {
        m_queueVersion = queueVersion;

        if (!mpd_send_list_queue_meta(m_connection)) {
            if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
                emit errorMessage(mpd_connection_get_error_message(m_connection));
                return;
            }
        }

        QVector<Item *> queue;

        mpd_entity *entity = nullptr;
        while ((entity = mpd_recv_entity(m_connection)) != nullptr) {
            if (mpd_entity_get_type(entity) == MPD_ENTITY_TYPE_SONG) {
                queue.append(new QueuedItem(entity));
            } else {
                mpd_entity_free(entity);
            }
        }

        auto error = mpd_connection_get_error(m_connection);
        if (MPD_ERROR_SUCCESS != error) {
            auto message = mpd_connection_get_error_message(m_connection);
            emit errorMessage(message);
            return;
        }

        emit queueChanged(queue);
    }
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
    qDebug() << "When handling idle, it is " << idle;
    if (!idle && mpd_connection_get_error(m_connection) == MPD_ERROR_CLOSED) {
        qDebug() << "CLOSED";
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
        m_mpdPlayerState = MPD_STATE_UNKNOWN;

        QVector<Item *> emptyQueue;
        emit queueChanged(emptyQueue);

    } else {
        bool statusUpdate = false;
        if (idle & MPD_IDLE_DATABASE) {
            qDebug() << "song database has been updated";
        }
        if (idle & MPD_IDLE_STORED_PLAYLIST) {
            qDebug() << "a stored playlist has been modified, created, deleted or renamed";
            emit playlists(listPlaylistsImpl());
        }
        if (idle & MPD_IDLE_QUEUE || idle & MPD_IDLE_PLAYER) {
            qDebug() << "The queue has changed";
            statusUpdate = true;
        }
        if (idle & MPD_IDLE_PLAYER) {
            qDebug() << "the player state has changed: play, stop, pause, seek, ...";
            statusUpdate = true;
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

        if (statusUpdate) {
            pollForStatus();
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

void Controller::disableIdle()
{
    if (!m_notifier || !m_connection) {
        return;
    }
    m_notifier->setEnabled(false);
    auto idle = mpd_run_noidle(m_connection);
    if (idle) {
        handleIdle(idle);
    } else {
        if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
            auto message = mpd_connection_get_error_message(m_connection);
            emit errorMessage(message);
        }
    }
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
        if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
            qDebug() << mpd_connection_get_error_message(m_connection);
            return tags;
        }
    }

    if (!mpd_search_commit(m_connection)) {
        if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
            qDebug() << mpd_connection_get_error_message(m_connection);
            return tags;
        }
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
        if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
            emit errorMessage(mpd_connection_get_error_message(m_connection));
        }
    }
    enableIdle();
}

void Controller::addAndPlaySong(QString uri)
{
    if (!m_connection) {
        return;
    }

    if (uri.isEmpty()) {
        return;
    }

    disableIdle();

    int id = mpd_run_add_id(m_connection, uri.toUtf8().constData());

    if (id == -1) {
        if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
            emit errorMessage(mpd_connection_get_error_message(m_connection));
            return;
        }
    }

    if (!mpd_run_play_id(m_connection, static_cast<unsigned>(id))) {
        if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
            emit errorMessage(mpd_connection_get_error_message(m_connection));
            return;
        }
    }

    enableIdle();
}

void Controller::playAlbum(const QVector<QString> &uris, QString uri)
{
    if (!m_connection) {
        return;
    }

    disableIdle();
    if (!mpd_run_clear(m_connection)) {
        if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
            emit errorMessage(mpd_connection_get_error_message(m_connection));
            return;
        }
    }

    int returnedId = -1;
    unsigned id = UINT_MAX;
    for (auto songUri : uris) {
        if ((returnedId = mpd_run_add_id(m_connection, songUri.toUtf8().constData())) == -1) {
            if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
                emit errorMessage(mpd_connection_get_error_message(m_connection));
                return;
            }
        }

        id = static_cast<unsigned>(returnedId);

        if (songUri == uri) {
            if (!mpd_run_play_id(m_connection, id)) {
                if (mpd_connection_get_error(m_connection) != MPD_ERROR_SUCCESS) {
                    emit errorMessage(mpd_connection_get_error_message(m_connection));

                    return;
                }
            }
        }
    }

    enableIdle();
}

void Controller::setCombinedTime(unsigned duration)
{
    if (duration) {
        emit combinedTime(timeStr(duration));
    } else {
        emit combinedTime("");
    }
}

void Controller::renamePlaylist(QString from, QString to)
{
    if (!m_connection) {
        return;
    }

    disableIdle();

    auto newName = to.trimmed();

    if (newName.isEmpty()) {
        return;
    }

    if (newName.contains("\\")) {
        return;
    }

    if (newName.contains("/")) {
        return;
    }

    if (newName.startsWith(".")) {
        return;
    }

    qDebug() << "renaming from " << from.toUtf8().constData() << " to "
             << newName.toUtf8().constData();
    if (mpd_run_rename(m_connection, from.toUtf8().constData(), to.toUtf8().constData())) {
        emit playlists(listPlaylistsImpl());
    } else {
        // This is robust enough, but I'm going to say that I've seen it lock up completely if the
        // playlist renaming fails.
        auto error = mpd_connection_get_error(m_connection);
        if (error == MPD_ERROR_SERVER) {
            auto serverError = mpd_connection_get_server_error(m_connection);
            auto message = mpd_connection_get_error_message(m_connection);
            qDebug() << "The server error is " << serverError << " " << message;
            emit errorMessage(message);
        }
    }

    enableIdle();
}

void Controller::deletePlaylist(QString name)
{
    if (!m_connection) {
        return;
    }

    disableIdle();
    if (!mpd_run_rm(m_connection, name.toUtf8().constData())) {
        emit errorMessage(mpd_connection_get_error_message(m_connection));
        return;
    }
    enableIdle();
}