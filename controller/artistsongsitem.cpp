#include "artistsongsitem.h"
#include "unorderedmpdsongitem.h"

ArtistSongsItem::ArtistSongsItem(QString artist, Item *parent)
    : Item(QIcon::fromTheme(IconNames::Database), Qt::ItemIsEnabled, true, true, parent)
    , m_artist(artist)
{}

QVector<Item *> ArtistSongsItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    QVector<QPair<mpd_tag_type, QString>> tags{{MPD_TAG_ARTIST, m_artist}};
    for (auto song : controller->searchSongs(tags)) {
        items.append(new UnorderedMPDSongItem(song));
    }
    return items;
}

QString ArtistSongsItem::text(int column) const
{
    if (0 == column) {
        return "All Songs";
    }

    return QString();
}
