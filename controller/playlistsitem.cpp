#include "playlistsitem.h"
#include "controller.h"
#include "iconnames.h"
#include "playlistitem.h"

PlaylistsItem::PlaylistsItem(Item *parent)
    : Item(QIcon::fromTheme(IconNames::Favorites), Qt::ItemIsEnabled, true, true, parent)
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
    };

    return items;
}
