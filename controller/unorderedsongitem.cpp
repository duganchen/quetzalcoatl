#include "unorderedsongitem.h"
#include "controller.h"

UnorderedSongItem::UnorderedSongItem(std::vector<QString> labels,
                                     QIcon icon,
                                     Qt::ItemFlags myFlags,
                                     bool hazChildren,
                                     bool couldFetchMore)
    : SongItem(labels, icon, myFlags, hazChildren, couldFetchMore)
{}

void UnorderedSongItem::onDoubleClicked(Controller *controller)
{
    controller->addAndPlaySong(uri());
}
