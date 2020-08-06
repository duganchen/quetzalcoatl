#include "artistitem.h"
#include "artistalbumitem.h"
#include "artistsongsitem.h"

ArtistItem::ArtistItem(QString artist, Item *parent)
    : Item(QIcon::fromTheme("server-database"), Qt::ItemIsEnabled, true, true, parent)

    , m_artist(artist)
{}

QVector<Item *> ArtistItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    items.append(new ArtistSongsItem(m_artist));

    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_ARTIST, m_artist}};
    for (auto album : controller->searchTags(MPD_TAG_ALBUM, filter)) {
        items.append(new ArtistAlbumItem(m_artist, album));
    }
    return items;
}

QString ArtistItem::text(int column) const
{
    if (0 == column) {
        return m_artist;
    }

    return QString();
}
