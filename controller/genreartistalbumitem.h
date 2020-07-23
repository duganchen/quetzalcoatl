#ifndef GENREARTISTALBUMITEM_H
#define GENREARTISTALBUMITEM_H

#include "dbitem.h"

class GenreArtistAlbumItem : public DBItem
{
public:
    explicit GenreArtistAlbumItem(
        QIcon, Qt::ItemFlags, bool, bool, QString, QString, QString, Item *parent = nullptr);

    QVector<Item *> fetchMore(Controller *) override;

private:
    QString m_genre;
    QString m_artist;
};

#endif // GENREARTISTALBUMITEM_H
