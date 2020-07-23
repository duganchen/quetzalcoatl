#include "genreartistsongsitem.h"
#include "controller.h"
#include "songitem.h"

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
    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_GENRE, m_genre},
                                                 {MPD_TAG_ARTIST, m_artist}};
    QVector<Item *> items;
    for (auto song : controller->searchSongs(filter)) {
        items.append(
            new SongItem(QIcon(":/icons/audio-x-generic.svg"), Qt::ItemIsEnabled, false, song));
    }
    return items;
}
