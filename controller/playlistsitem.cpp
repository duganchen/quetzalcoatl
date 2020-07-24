#include "playlistsitem.h"
#include "playlistitem.h"
#include <QDebug>

PlaylistsItem::PlaylistsItem(Item *parent)
    : Item(QIcon(":/icons/folder-favorites.svg"), Qt::ItemIsEnabled, true, true, parent)
{}

QString PlaylistsItem::text(int column) const
{
    if (0 == column) {
        return "Playlists";
    }
    return QString();
}

QVector<Item *> PlaylistsItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    for (auto playlist : controller->listPlaylists()) {
        items.append(new PlaylistItem(playlist));
    }
    return items;
}
