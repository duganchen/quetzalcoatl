#include "constsong.h"

mpd::ConstSong::ConstSong(const mpd_song *song)
    : m_song{song}
{}

mpd::ConstSong::operator bool()
{
    return bool(m_song);
}

const char *mpd::ConstSong::getTag(mpd_tag_type type, unsigned idx)
{
    return mpd_song_get_tag(m_song, type, idx);
}

unsigned mpd::ConstSong::getId()
{
    return mpd_song_get_id(m_song);
}

const char *mpd::ConstSong::getURI()
{
    return mpd_song_get_uri(m_song);
}

unsigned mpd::ConstSong::getDuration()
{
    return mpd_song_get_duration(m_song);
}