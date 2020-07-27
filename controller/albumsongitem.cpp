#include "albumsongitem.h"
#include "controller.h"

AlbumSongItem::AlbumSongItem(mpd_song *song, Item *parent)
    : SongItem(song, parent)
{}

void AlbumSongItem::onDoubleClicked(Controller *controller)
{
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
