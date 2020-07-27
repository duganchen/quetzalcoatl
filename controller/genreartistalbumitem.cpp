#include "genreartistalbumitem.h"
#include "albumsongitem.h"
#include "controller.h"

GenreArtistAlbumItem::GenreArtistAlbumItem(QString genre,
                                           QString artist,
                                           QString album,
                                           Item *parent)
    : Item(QIcon(":/icons/server-database.svg"), Qt::ItemIsEnabled, true, true, parent)
    , m_genre(genre)
    , m_artist(artist)
    , m_album(album)
{}

QVector<Item *> GenreArtistAlbumItem::fetchMore(Controller *controller)
{
    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_GENRE, m_genre},
                                                 {MPD_TAG_ARTIST, m_artist},
                                                 {MPD_TAG_ALBUM, m_album}};
    auto songs = controller->searchSongs(filter);
    QVector<Item *> items;
    for (auto song : songs) {
        items.append(new AlbumSongItem(song));
    }

    return items;
}

QString GenreArtistAlbumItem::text(int column) const
{
    if (0 == column) {
        return m_album;
    }
    return QString();
}
