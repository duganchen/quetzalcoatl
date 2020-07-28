#include "genreartistsongsitem.h"
#include "unorderedmpdsongitem.h"
#include "controller.h"

GenreArtistSongsItem::GenreArtistSongsItem(QString genre, QString artist, Item *parent)
    : Item(QIcon(":/icons/server-database.svg"), Qt::ItemIsEnabled, true, true, parent)
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
        items.append(new UnorderedMPDSongItem(song));
    }
    return items;
}

QString GenreArtistSongsItem::text(int column) const
{
    if (0 == column) {
        return "All Songs";
    }

    return QString();
}
