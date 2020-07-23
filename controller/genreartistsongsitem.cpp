#include "genreartistsongsitem.h"

GenreArtistSongsItem::GenreArtistSongsItem(QIcon icon,
                                           Qt::ItemFlags myFlags,
                                           bool hazChildren,
                                           bool couldFetchMore,
                                           QString genre,
                                           QString artist,
                                           QString text,
                                           Item *parent)
    : DBItem(icon, myFlags, hazChildren, couldFetchMore, text, parent)
    , m_genre(genre)
    , m_artist(artist)
{}

QVector<Item *> GenreArtistSongsItem::fetchMore(Controller *controller)
{
    Q_UNUSED(controller);
    QVector<Item *> items;
    return items;
}
