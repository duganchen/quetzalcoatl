#include "albumsitem.h"

#include "albumitem.h"
#include "controller.h"

AlbumsItem::AlbumsItem(Item *parent)
    : Item(QIcon::fromTheme("server-database"), Qt::ItemIsEnabled, true, true, parent)
{}

QString AlbumsItem::text(int column) const
{
    if (0 == column) {
        return "Albums";
    }
    return QString();
}

QVector<Item *> AlbumsItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    for (auto album : controller->listTags(MPD_TAG_ALBUM)) {
        items.append(new AlbumItem(album));
    }
    return items;
}
