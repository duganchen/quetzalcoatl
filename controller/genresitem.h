#ifndef GENRESITEM_H
#define GENRESITEM_H

#include "item.h"

class GenresItem : public Item
{
public:
    explicit GenresItem(Item *parent = nullptr);

    QString text(int) const override;
    QVector<Item *> fetchMore(Controller *) override;
};

#endif // GENRESITEM_H
