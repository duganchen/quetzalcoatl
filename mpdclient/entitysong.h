#ifndef constsong_h
#define constsong_h

#include "isong.h"

namespace mpd {

class ConstSong : public ISong
{
public:
    explicit ConstSong(const mpd_song *);
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