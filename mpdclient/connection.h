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
    explicit operator bool();
    mpd_error getError();
    const char *getErrorMessage();
    const unsigned *getServerVersion();
    bool searchDBTags(mpd_tag_type type);
    bool searchCommit();
    std::vector<QString> recvValues();
    bool searchDBSongs(bool);
    bool searchAddTagConstraint(mpd_operator, mpd_tag_type, const char *);
    bool mpdSearchCommit();

    // Can we at least mock this?
    virtual std::unique_ptr<mpd::Song> getASong() { return std::make_unique<mpd::Song>(); };

    // Use this to demonstrate mocking
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