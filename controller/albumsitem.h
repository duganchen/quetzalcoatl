#ifndef ALBUMSITEM_H
#define ALBUMSITEM_H

#include "item.h"

class AlbumsItem : public Item
{
public:
    explicit AlbumsItem(Item *parent = nullptr);

    QString text(int) const override;
    QVector<Item *> fetchMore(Controller *) override;
};

#endif // ALBUMSITEM_H
