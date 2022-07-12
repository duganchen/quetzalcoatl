#ifndef GENREITEM_H
#define GENREITEM_H

#include "item.h"

class GenreItem : public Item
{
public:
    explicit GenreItem(const std::vector<QString> &, Item *parent = nullptr);
    QVector<Item *> fetchMore(Controller *) override;
};
#endif // GENREITEM_H
