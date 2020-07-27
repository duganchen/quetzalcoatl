#ifndef ALBUMSONGITEM_H
#define ALBUMSONGITEM_H

#include "songitem.h"

class AlbumSongItem : public SongItem
{
public:
    AlbumSongItem(mpd_song *, Item *parent = nullptr);
    void onDoubleClicked(Controller *) override;
};

#endif // ALBUMSONGITEM_H
