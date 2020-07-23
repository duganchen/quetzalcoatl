#ifndef GENREARTISTITEM_H
#define GENREARTISTITEM_H

#include "dbitem.h"

class GenreArtistItem : public DBItem
{
public:
    explicit GenreArtistItem(
        QIcon, Qt::ItemFlags, bool, bool, QString, QString, Item *parent = nullptr);

    QVector<Item *> fetchMore(Controller *) override;

private:
    QString m_genre;
};

#endif // GENREARTISTITEM_H
