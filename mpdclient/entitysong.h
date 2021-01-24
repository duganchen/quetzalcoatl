#ifndef entitysong_h
#define entitysong_h

#include <mpd/client.h>

namespace mpd {

class EntitySong
{
public:
    explicit EntitySong(const mpd_song *);
    explicit operator bool();
    virtual const char *getTag(mpd_tag_type, unsigned);
    virtual unsigned getId();
    virtual const char *getURI();
    virtual unsigned getDuration();

private:
    const mpd_song *m_song{};
};

} // namespace mpd
#endif