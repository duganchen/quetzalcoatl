#ifndef GENREARTISTSONGSITEM_H
#define GENREARTISTSONGSITEM_H

#include "dbitem.h"

class GenreArtistSongsItem : public DBItem
{
public:
    explicit GenreArtistSongsItem(
        QIcon, Qt::ItemFlags, bool, bool, QString, QString, QString, Item *parent = nullptr);

    QVector<Item *> fetchMore(Controller *) override;

private:
    QString m_genre;
    QString m_artist;
};

#endif // GENREARTISTSONGSITEM_H
