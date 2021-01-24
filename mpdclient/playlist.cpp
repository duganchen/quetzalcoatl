#include "playlist.h"
#include <mpd/client.h>

mpd::Playlist::operator bool()
{
    return bool(m_playlist);
}

mpd::Playlist::Playlist(mpd_playlist *playlist)
    : m_playlist{playlist}
{}

const char *mpd::Playlist::getPath()
{
    return mpd_playlist_get_path(m_playlist);
}

mpd::Playlist::Playlist(const mpd::Playlist &other)
{
    if (m_playlist) {
        mpd_playlist_free(m_playlist);
        m_playlist = nullptr;
    }

    if (other.m_playlist) {
        m_playlist = mpd_playlist_dup(other.m_playlist);
    }
}

mpd::Playlist &mpd::Playlist::operator=(const mpd::Playlist &other)
{
    if (m_playlist) {
        mpd_playlist_free(m_playlist);
        m_playlist = nullptr;
    }

    if (other.m_playlist) {
        m_playlist = mpd_playlist_dup(other.m_playlist);
    }

    return *this;
}

mpd::Playlist::Playlist(mpd::Playlist &&other)
{
    if (m_playlist) {
        mpd_playlist_free(m_playlist);
    }

    m_playlist = other.m_playlist;
    other.m_playlist = nullptr;
}

mpd::Playlist &mpd::Playlist::operator=(mpd::Playlist &&other)
{
    if (m_playlist) {
        mpd_playlist_free(m_playlist);
    }

    m_playlist = other.m_playlist;
    other.m_playlist = nullptr;

    return *this;
}

mpd::Playlist::~Playlist()
{
    if (m_playlist) {
        mpd_playlist_free(m_playlist);
    }
}