#ifndef ALLSONGITEM_H
#define ALLSONGITEM_H

#include "songitem.h"

// A node under "All Songs".

class AllSongItem : public SongItem
{
public:
    AllSongItem(mpd_song *, Item *parent = nullptr);
    void onDoubleClicked(Controller *) override;
};

#endif // ALLSONGITEM_H
