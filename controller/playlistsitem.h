#ifndef PLAYLISTSITEM_H
#define PLAYLISTSITEM_H

#include "item.h"

class PlaylistsItem : public Item
{
public:
    PlaylistsItem(Item *parent = nullptr);

    QString text(int) const override;
    QVector<Item *> fetchMore(Controller *) override;
};

#endif // PLAYLISTSITEM_H
