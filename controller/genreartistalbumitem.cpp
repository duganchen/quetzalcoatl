#include "genreartistalbumitem.h"
#include "controller.h"
#include "songitem.h"

GenreArtistAlbumItem::GenreArtistAlbumItem(QIcon icon,
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

QVector<Item *> GenreArtistAlbumItem::fetchMore(Controller *controller)
{
    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_GENRE, m_genre},
                                                 {MPD_TAG_ARTIST, m_artist},
                                                 {MPD_TAG_ALBUM, text(0)}};
    auto songs = controller->searchSongs(filter);
    QVector<Item *> items;
    for (auto song : songs) {
        items.append(
            new SongItem(QIcon(":/icons/audio-x-generic.svg"), Qt::ItemIsEnabled, false, song));
    }

    return items;
}
