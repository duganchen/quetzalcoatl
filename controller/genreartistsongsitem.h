#ifndef GENREARTISTSONGSITEM_H
#define GENREARTISTSONGSITEM_H

#include "item.h"

class GenreArtistSongsItem : public Item
{
public:
    GenreArtistSongsItem(QString, QString, Item *parent = nullptr);

    QVector<Item *> fetchMore(Controller *) override;

private:
    QString m_genre;
    QString m_artist;
};

#endif // GENREARTISTSONGSITEM_H
