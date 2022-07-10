#include "artistitem.h"
#include "artistalbumitem.h"
#include "artistsongsitem.h"
#include "controller.h"
#include "iconnames.h"

ArtistItem::ArtistItem(const std::vector<QString> &labels, QString artist, Item *parent)
    : Item(labels, QIcon::fromTheme(IconNames::Database), Qt::ItemIsEnabled, true, true, parent)

    , m_artist(artist)
{}

QVector<Item *> ArtistItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    items.append(new ArtistSongsItem(m_artist));

    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_ARTIST, m_artist}};
    for (auto album : controller->searchTags(MPD_TAG_ALBUM, filter)) {
        items.append(new ArtistAlbumItem({album}, m_artist, album));
    }
    return items;
}
