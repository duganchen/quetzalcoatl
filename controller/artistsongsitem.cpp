#include "artistsongsitem.h"
#include "controller.h"
#include "iconnames.h"
#include "strformats.h"
#include "unorderedmpdsongitem.h"

ArtistSongsItem::ArtistSongsItem(QString artist)
    : Item({"All Songs"}, QIcon::fromTheme(IconNames::Database), Qt::ItemIsEnabled, true, true)
    , m_artist(artist)
{}

QVector<Item *> ArtistSongsItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    QVector<QPair<mpd_tag_type, QString>> tags{{MPD_TAG_ARTIST, m_artist}};
    for (auto song : controller->searchSongs(tags)) {
        items.append(new UnorderedMPDSongItem({songLabel(song)}, song, songToolTip(song)));
    }
    return items;
}