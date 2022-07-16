#ifndef GENREARTISTITEM_H
#define GENREARTISTITEM_H

#include "item.h"

class GenreArtistItem : public Item
{
public:
    GenreArtistItem(const std::vector<QString> &, QString);

    QVector<Item *> fetchMore(Controller *) override;

private:
    QString m_genre;
};

#endif // GENREARTISTITEM_H
