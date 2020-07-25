#include "songsitem.h"
#include "playlistsongitem.h"

SongsItem::SongsItem(Item *parent)
    : Item(QIcon(":/icons/server-database.svg"), Qt::ItemIsEnabled, true, true, parent)
{}

QString SongsItem::text(int column) const
{
    if (0 == column) {
        return "Songs";
    }
    return QString();
}

QVector<Item *> SongsItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    for (auto entity : controller->listAll(MPD_ENTITY_TYPE_SONG)) {
        items.append(new PlaylistSongItem(entity));
    }
    return items;
}
