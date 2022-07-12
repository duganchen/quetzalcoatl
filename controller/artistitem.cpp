#include "artistitem.h"
#include "artistalbumitem.h"
#include "artistsongsitem.h"
#include "controller.h"
#include "iconnames.h"

ArtistItem::ArtistItem(const std::vector<QString> &labels, Item *parent)
    : Item(labels, QIcon::fromTheme(IconNames::Database), Qt::ItemIsEnabled, true, true, parent)
{}

QVector<Item *> ArtistItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    items.append(new ArtistSongsItem(text(0)));

    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_ARTIST, text(0)}};
    for (auto album : controller->searchTags(MPD_TAG_ALBUM, filter)) {
        items.append(new ArtistAlbumItem({album}, text(0)));
    }
    return items;
}
