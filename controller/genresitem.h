#ifndef GENRESITEM_H
#define GENRESITEM_H

#include "dbitem.h"

class GenresItem : public DBItem
{
public:
    explicit GenresItem(QIcon, Qt::ItemFlags, bool, QString, Item *parent = nullptr);

    void fetchMore() override;
};

#endif // GENRESITEM_H
