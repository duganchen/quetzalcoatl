#ifndef GENREITEM_H
#define GENREITEM_H

#include "dbitem.h"

class GenreItem : public DBItem
{
public:
    explicit GenreItem(QIcon, Qt::ItemFlags, bool, bool, QString, Item *parent = nullptr);

    QVector<Item *> fetchMore(Controller *) override;
};
#endif // GENREITEM_H
