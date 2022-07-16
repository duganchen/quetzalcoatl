#ifndef ALBUMSITEM_H
#define ALBUMSITEM_H

#include "item.h"

class AlbumsItem : public Item
{
public:
    AlbumsItem();

    QVector<Item *> fetchMore(Controller *) override;
};

#endif // ALBUMSITEM_H
