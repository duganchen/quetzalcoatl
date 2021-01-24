#ifndef connection_h
#define connection_h

#include "song.h"
#include <memory>
#include <mpd/client.h>
#include <vector>
#include <QString>

namespace mpd {

class Connection
{
public:
    Connection();
    Connection(const char *, unsigned, unsigned);

    virtual explicit operator bool();
    virtual mpd_error getError();
    virtual const char *getErrorMessage();
    virtual const unsigned *getServerVersion();
    virtual bool searchDBTags(mpd_tag_type type);
    virtual bool searchCommit();
    virtual std::vector<QString> recvValues();
    virtual bool searchDBSongs(bool);
    virtual bool searchAddTagConstraint(mpd_operator, mpd_tag_type, const char *);
    virtual bool mpdSearchCommit();
    virtual std::vector<std::unique_ptr<mpd::Song>> recvSongs();

    Connection(const Connection &) = delete;
    Connection &operator=(const Connection &) = delete;
    Connection(Connection &&);
    Connection &operator=(Connection &&);
    virtual ~Connection();

private:
    mpd_connection *m_connection;
};

} // namespace mpd

#endif