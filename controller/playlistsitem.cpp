#include "playlistsitem.h"
#include "controller.h"
#include "iconnames.h"
#include "playlistitem.h"

PlaylistsItem::PlaylistsItem(Item *parent)
    : Item({"Playlists"},
           QIcon::fromTheme(IconNames::Favorites),
           Qt::ItemIsEnabled,
           true,
           true,
           parent)
{}

QVector<Item *> PlaylistsItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    for (auto playlist : controller->listPlaylists()) {
        items.append(new PlaylistItem({mpd_playlist_get_path(playlist)}));
    };

    return items;
}
