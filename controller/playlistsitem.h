#ifndef PLAYLISTSITEM_H
#define PLAYLISTSITEM_H

#include "item.h"

class PlaylistsItem : public Item
{
public:
    PlaylistsItem();

    QVector<Item *> fetchMore(Controller *) override;
};

#endif // PLAYLISTSITEM_H
