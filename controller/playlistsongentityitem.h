#ifndef PLAYLISTSONGENTITYITEM_H
#define PLAYLISTSONGENTITYITEM_H

#include "songentityitem.h"

class PlaylistSongEntityItem : public SongEntityItem
{
public:
    PlaylistSongEntityItem(mpd_entity *, Item *parent = nullptr);
    void onDoubleClicked(Controller *) override;
};

#endif // PLAYLISTSONGENTITYITEM_H
