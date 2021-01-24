#include "song.h"
#include <mpd/client.h>

mpd::Song::operator bool() { return bool(m_song); }

const char *mpd::Song::getTag(mpd_tag_type type, unsigned idx) {
  return mpd_song_get_tag(m_song, type, idx);
}

mpd::Song::Song(mpd_song *song) : m_song{song} {}

mpd::Song::Song(const mpd::Song &other) {
  if (m_song) {
    mpd_song_free(m_song);
    m_song = nullptr;
  }

  if (other.m_song) {
    m_song = mpd_song_dup(other.m_song);
  }
}

mpd::Song &mpd::Song::operator=(const mpd::Song &other) {
  if (m_song) {
    mpd_song_free(m_song);
    m_song = nullptr;
  }

  return *this;
}

mpd::Song::Song(mpd::Song &&other) {
  if (m_song) {
    mpd_song_free(m_song);
  }

  m_song = other.m_song;
  other.m_song = nullptr;
}

mpd::Song &mpd::Song::operator=(mpd::Song &&other) {
  if (m_song) {
    mpd_song_free(m_song);
  }

  m_song = other.m_song;
  other.m_song = nullptr;

  return *this;
}

mpd::Song::~Song() {
  if (m_song) {
    mpd_song_free(m_song);
  }
}