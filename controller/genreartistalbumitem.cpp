#include "genreartistalbumitem.h"
#include "controller.h"
#include "iconnames.h"
#include "orderedmpdsongitem.h"
#include "strformats.h"

GenreArtistAlbumItem::GenreArtistAlbumItem(const std::vector<QString> &labels,
                                           QString genre,
                                           QString artist)
    : Item(labels, QIcon::fromTheme(IconNames::Database), Qt::ItemIsEnabled, true, true)
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
        items.append(new OrderedMPDSongItem({songLabel(song)}, song));
    }

    return items;
}