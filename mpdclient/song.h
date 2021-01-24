#ifndef song_h
#define song_h

#include "isong.h"
#include <mpd/client.h>

namespace mpd {

class Song : public ISong
{
public:
    explicit operator bool();

    const char *getTag(mpd_tag_type, unsigned) override;
    unsigned getId() override;
    const char *getURI() override;
    unsigned getDuration() override;

    Song(mpd_song * = nullptr);
    Song(const Song &other);
    Song &operator=(const Song &other);
    Song(Song &&other);
    Song &operator=(Song &&other);
    ~Song() override;

private:
    mpd_song *m_song{};
};

} // namespace mpd
#endif