#ifndef entitysong_h
#define entitysong_h

#include "isong.h"
#include <mpd/client.h>

namespace mpd {

class EntitySong : public ISong
{
public:
    explicit EntitySong(const mpd_song *);
    explicit operator bool();

    const char *getTag(mpd_tag_type, unsigned) override;
    unsigned getId() override;
    const char *getURI() override;
    unsigned getDuration() override;

private:
    const mpd_song *m_song{};
};

} // namespace mpd
#endif