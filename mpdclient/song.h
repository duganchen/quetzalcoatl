#ifndef song_h
#define song_h

#include <mpd/client.h>

namespace mpd {

class Song
{
public:
    explicit operator bool();

    virtual const char *getTag(mpd_tag_type, unsigned);
    virtual unsigned getId();
    virtual const char *getURI();
    virtual unsigned getDuration();

    Song(mpd_song * = nullptr);
    Song(const Song &other);
    Song &operator=(const Song &other);
    Song(Song &&other);
    Song &operator=(Song &&other);
    virtual ~Song();

private:
    mpd_song *m_song{};
};

} // namespace mpd
#endif