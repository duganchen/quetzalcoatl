#ifndef GENREARTISTALBUMITEM_H
#define GENREARTISTALBUMITEM_H

#include "item.h"

class GenreArtistAlbumItem : public Item
{
public:
    GenreArtistAlbumItem(const std::vector<QString> &, QString, QString, Item *parent = nullptr);
    QVector<Item *> fetchMore(Controller *) override;

private:
    QString m_genre;
    QString m_artist;
};

#endif // GENREARTISTALBUMITEM_H
