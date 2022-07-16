#ifndef GENRESONGSITEM_H
#define GENRESONGSITEM_H

#include "item.h"

class GenreSongsItem : public Item
{
public:
    GenreSongsItem();

    QVector<Item *> fetchMore(Controller *) override;
};

#endif // GENRESONGSITEM_H
