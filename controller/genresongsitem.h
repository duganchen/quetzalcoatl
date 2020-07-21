#ifndef GENRESONGSITEM_H
#define GENRESONGSITEM_H

#include "dbitem.h"

class GenreSongsItem : public DBItem
{
public:
    explicit GenreSongsItem(QIcon, Qt::ItemFlags, bool, bool, QString, Item *parent = nullptr);

    QVector<Item *> fetchMore(Controller *) override;
};

#endif // GENRESONGSITEM_H
