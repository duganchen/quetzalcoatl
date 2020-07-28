#include "unorderedsongitem.h"
#include "controller.h"

UnorderedSongItem::UnorderedSongItem(
    QIcon icon, Qt::ItemFlags myFlags, bool hazChildren, bool couldFetchMore, Item *parent)
    : AbstractSongItem(icon, myFlags, hazChildren, couldFetchMore, parent)
{}

void UnorderedSongItem::onDoubleClicked(Controller *controller)
{
    controller->addAndPlaySong(uri());
}
