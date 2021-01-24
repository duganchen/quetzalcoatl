This is to be a wrapper for libmpdclient.

A unit test that uses this wrapper might look like this:

    #include <gmock/gmock.h>
    #include <gtest/gtest.h>

    #include "mpdfacade.h"
    #include "song.h"

    // Test a factory method, which uses two mocks.

    class MockConnection : public mpd::Connection {
    public:
    MOCK_METHOD(std::vector<std::unique_ptr<mpd::Song>>, recvSongs, (),
                (override));
    };

    class MockSong : public mpd::Song {
    public:
    MOCK_METHOD(const char *, getTag, (mpd_tag_type, unsigned), (override));
    };

    TEST(SampleTest, Hello) {
    auto song_ptr = new MockSong();
    std::unique_ptr<mpd::Song> song{song_ptr};
    EXPECT_CALL(*song_ptr, getTag(MPD_TAG_TITLE, 0))
        .WillOnce(::testing::Return("Presentiment / Chrono Trigger"));

    MockConnection connection;

    std::vector<std::unique_ptr<mpd::Song>> songs;
    songs.push_back(std::move(song));

    EXPECT_CALL(connection, recvSongs())
        .WillOnce(::testing::Return(::testing::ByMove(std::move(songs))));

    MPDFacade facade(connection);

    EXPECT_EQ("Presentiment / Chrono Trigger", facade.getFirstSong());
    }
