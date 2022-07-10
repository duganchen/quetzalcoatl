#ifndef GENRESITEM_H
#define GENRESITEM_H

#include "item.h"

class GenresItem : public Item
{
public:
    GenresItem(Item *parent = nullptr);

    QVector<Item *> fetchMore(Controller *) override;
};

#endif // GENRESITEM_H
