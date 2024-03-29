#include "albumsitem.h"

#include "albumitem.h"
#include "controller.h"
#include "iconnames.h"

AlbumsItem::AlbumsItem()
    : Item({"Albums"}, QIcon::fromTheme(IconNames::Database), Qt::ItemIsEnabled, true, true)
{}

QVector<Item *> AlbumsItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    for (auto album : controller->listTags(MPD_TAG_ALBUM)) {
        items.append(new AlbumItem({album}));
    }
    return items;
}
