#ifndef playlist_h
#define playlist_h

#include <mpd/client.h>

namespace mpd {

class Playlist
{
public:
    explicit operator bool();
    Playlist(mpd_playlist * = nullptr);

    virtual const char *getPath();

    Playlist(const Playlist &other);
    Playlist &operator=(const Playlist &other);
    Playlist(Playlist &&other);
    Playlist &operator=(Playlist &&other);
    virtual ~Playlist();

private:
    mpd_playlist *m_playlist{};
};

} // namespace mpd
#endif