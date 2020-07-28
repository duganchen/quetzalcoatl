#include "playlistsongentityitem.h"
#include "controller.h"

PlaylistSongEntityItem::PlaylistSongEntityItem(mpd_entity *entity, Item *parent)
    : SongEntityItem(entity, parent)
{}

void PlaylistSongEntityItem::onDoubleClicked(Controller *controller)
{
    // TODO: This is duplicated with the same function in AlbumSongItem!
    if (!controller) {
        return;
    }

    if (!parent()) {
        return;
    }

    QVector<QString> uris;

    for (Item *item : parent()->children()) {
        if (!item) {
            continue;
        }

        uris.append(item->uri());
    }

    controller->playAlbum(uris, uri());
}
