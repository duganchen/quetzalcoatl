#include "randomsongentityitem.h"
#include "controller.h"

RandomSongEntityItem::RandomSongEntityItem(mpd_entity *entity, Item *parent)
    : SongEntityItem(entity, parent)
{}

void RandomSongEntityItem::onDoubleClicked(Controller *controller)
{
    controller->addAndPlaySong(uri());
}
