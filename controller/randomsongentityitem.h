#ifndef RANDOMSONGENTITYITEM_H
#define RANDOMSONGENTITYITEM_H

#include "songentityitem.h"

// This is under "Songs"
class RandomSongEntityItem : public SongEntityItem
{
public:
    RandomSongEntityItem(mpd_entity *, Item *parent = nullptr);
    void onDoubleClicked(Controller *) override;
};

#endif // RANDOMSONGENTITYITEM_H
