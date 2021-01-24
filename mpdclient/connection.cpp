#include "connection.h"
#include "song.h"

mpd::Connection::Connection()
    : m_connection{}
{}

mpd::Connection::Connection(const char *host, unsigned port, unsigned timeout_ms)
    : m_connection{mpd_connection_new(host, port, timeout_ms)}
{}

mpd::Connection::operator bool()
{
    return bool(m_connection);
}

mpd_error mpd::Connection::getError()
{
    return mpd_connection_get_error(m_connection);
};

const char *mpd::Connection::getErrorMessage()
{
    return mpd_connection_get_error_message(m_connection);
}

const unsigned *mpd::Connection::getServerVersion()
{
    return mpd_connection_get_server_version(m_connection);
}

bool mpd::Connection::searchDBTags(mpd_tag_type type)
{
    return mpd_search_db_tags(m_connection, type);
}

bool mpd::Connection::searchCommit()
{
    return mpd_search_commit(m_connection);
}

std::vector<QString> mpd::Connection::recvValues()
{
    std::vector<QString> values;
    while (auto pair = mpd_recv_pair(m_connection)) {
        values.push_back(pair->value);
        mpd_return_pair(m_connection, pair);
    }
    return values;
}

bool mpd::Connection::searchDBSongs(bool exact)
{
    return mpd_search_db_songs(m_connection, exact);
}

bool mpd::Connection::searchAddTagConstraint(mpd_operator oper, mpd_tag_type type, const char *value)
{
    return mpd_search_add_tag_constraint(m_connection, oper, type, value);
}

std::vector<std::unique_ptr<mpd::ISong>> mpd::Connection::recvSongs()
{
    std::vector<std::unique_ptr<mpd::ISong>> songs;
    while (mpd_song *song = mpd_recv_song(m_connection)) {
        songs.push_back(std::make_unique<mpd::Song>(song));
    }
    return songs;
}

bool mpd::Connection::clearError()
{
    return mpd_connection_clear_error(m_connection);
}

int mpd::Connection::getFD()
{
    return mpd_connection_get_fd(m_connection);
}

bool mpd::Connection::commandListBegin(bool discrete_ok)
{
    return mpd_command_list_begin(m_connection, discrete_ok);
}

bool mpd::Connection::commandListEnd()
{
    return mpd_command_list_end(m_connection);
}

bool mpd::Connection::sendListPlaylists()
{
    return mpd_send_list_playlists(m_connection);
}

std::vector<std::unique_ptr<mpd::Playlist>> mpd::Connection::recvPlaylists()
{
    std::vector<std::unique_ptr<mpd::Playlist>> playlists;
    while (auto playlist = mpd_recv_playlist(m_connection)) {
        playlists.push_back(std::make_unique<Playlist>(playlist));
    }
    return playlists;
}

mpd::Connection::Connection(mpd::Connection &&other)
    : m_connection(other.m_connection)
{
    other.m_connection = nullptr;
}

mpd::Connection &mpd::Connection::operator=(mpd::Connection &&other)
{
    if (this != &other) {
        if (m_connection) {
            mpd_connection_free(m_connection);
        }

        m_connection = other.m_connection;
        other.m_connection = nullptr;
    }
    return *this;
}

mpd::Connection::~Connection()
{
    if (m_connection) {
        mpd_connection_free(m_connection);
    }
}