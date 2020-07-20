#ifndef GENRESITEM_H
#define GENRESITEM_H

#include "dbitem.h"

class GenresItem : public DBItem
{
public:
    explicit GenresItem(QIcon, Qt::ItemFlags, bool, bool, QString, Item *parent = nullptr);

    QVector<Item *> fetchMore(Controller *) override;
};

#endif // GENRESITEM_H
