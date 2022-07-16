#ifndef GENRESITEM_H
#define GENRESITEM_H

#include "item.h"

class GenresItem : public Item
{
public:
    GenresItem();

    QVector<Item *> fetchMore(Controller *) override;
};

#endif // GENRESITEM_H
