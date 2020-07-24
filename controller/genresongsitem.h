#ifndef GENRESONGSITEM_H
#define GENRESONGSITEM_H

#include "dbitem.h"

class GenreSongsItem : public Item
{
public:
    explicit GenreSongsItem(Item *parent = nullptr);

    QVector<Item *> fetchMore(Controller *) override;

    QString text(int) const override;
};

#endif // GENRESONGSITEM_H
