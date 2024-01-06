#pragma once
#ifndef QMPD_H
#define QMPD_H

/*
A C++ libmpdclient wrapper. You'd better believe that a LOT
of thought went into this.

The idea is to free the programmer from managing the lifetimes of libmpdclient's objects.
It is not to be an abstraction layer on top of libmpdclient.

Handle errors the way you usually would (e.g. mpd_connection_get_error).

For reference, the definitive list of ways to implement this is here:
https://www.reddit.com/r/cpp/comments/18zyae6/comment/kgkp9fw/?utm_source=share&utm_medium=web2x&context=3

*/

#include <memory>
#include <mpd/client.h>
#include <vector>

namespace mpd {

namespace {

/*
Note C++17 syntax. See:
https://stackoverflow.com/a/43378781/240515

This can be improved when the target compilers support P0315R4 (C++20) See:
https://www.reddit.com/r/cpp/comments/rlvsq0/comment/hpmg0k6/?utm_source=share&utm_medium=web2x&context=3
*/

template<typename T, auto free_fn>
struct Deleter
{
    void operator()(T *ptr)
    {
        // std::unique_ptr is smart enough to not call the deleter if the pointer is null. At least,
        // that's true of g++ 11.2.1. I checked.
        free_fn(ptr);
    }
};

} // namespace

// Handle types
using connection = std::unique_ptr<mpd_connection, Deleter<mpd_connection, mpd_connection_free>>;
using song = std::unique_ptr<mpd_song, Deleter<mpd_song, mpd_song_free>>;
using entity = std::unique_ptr<mpd_entity, Deleter<mpd_entity, mpd_entity_free>>;
using playlist = std::unique_ptr<mpd_playlist, Deleter<mpd_playlist, mpd_playlist_free>>;
using settings = std::unique_ptr<mpd_settings, Deleter<mpd_settings, mpd_settings_free>>;
using status = std::unique_ptr<mpd_status, Deleter<mpd_status, mpd_status_free>>;

template<typename T, auto recv_fn>
std::vector<T> recv_objects(mpd_connection *conn)
{
    std::vector<T> objects;
    while (auto obj = recv_fn(conn)) {
        objects.push_back(T(obj));
    }
    return objects;
}

// Note C++ 14 syntax
// See: https://stackoverflow.com/a/18919937/240515

constexpr auto recv_songs = recv_objects<song, mpd_recv_song>;
constexpr auto recv_playlists = recv_objects<playlist, mpd_recv_playlist>;
constexpr auto recv_entities = recv_objects<entity, mpd_recv_entity>;

// Hardcoded for QStrings because that's Quetzalcoatl's actual requirement.
// If this were a more general library, then it would be templated for anything
// constructible from const char *.
std::vector<QString> recv_tag_values(mpd_connection *conn, mpd_tag_type type)
{
    std::vector<QString> values;
    while (auto pair = mpd_recv_pair_tag(conn, type)) {
        values.emplace_back(pair->value);
        mpd_return_pair(conn, pair);
    }
    return values;
}

}; // namespace mpd

#endif