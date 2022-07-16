#include "genreartistsongsitem.h"
#include "controller.h"
#include "iconnames.h"
#include "strformats.h"
#include "unorderedmpdsongitem.h"

GenreArtistSongsItem::GenreArtistSongsItem(QString genre, QString artist)
    : Item({"All Songs"}, QIcon::fromTheme(IconNames::Database), Qt::ItemIsEnabled, true, true)
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
        items.append(new UnorderedMPDSongItem({songLabel(song)}, song));
    }
    return items;
}