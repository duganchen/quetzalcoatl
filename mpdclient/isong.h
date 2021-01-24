#ifndef isong_h
#define isong_h

#include <mpd/client.h>

namespace mpd {

class ISong
{
public:
    virtual const char *getTag(mpd_tag_type, unsigned);
    virtual unsigned getId();
    virtual const char *getURI();
    virtual unsigned getDuration();
    virtual ~ISong(){};
};

} // namespace mpd

#endif