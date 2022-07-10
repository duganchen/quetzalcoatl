#ifndef ARTISTSITEM_H
#define ARTISTSITEM_H

#include "item.h"

class ArtistsItem : public Item
{
public:
    ArtistsItem(Item *parent = nullptr);

    QVector<Item *> fetchMore(Controller *) override;
};

#endif // ARTISTSITEM_H
