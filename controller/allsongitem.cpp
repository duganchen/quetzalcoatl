#include "allsongitem.h"
#include "controller.h"

AllSongItem::AllSongItem(mpd_song *song, Item *parent)
    : SongItem(song, parent)
{}

void AllSongItem::onDoubleClicked(Controller *controller)
{
    if (!controller) {
        return;
    }

    controller->addAndPlaySong(uri());
}
