#include "unorderedsongitem.h"
#include "controller.h"

UnorderedSongItem::UnorderedSongItem(std::vector<QString> labels,
                                     QIcon icon,
                                     Qt::ItemFlags myFlags,
                                     bool hazChildren,
                                     bool couldFetchMore,
                                     Item *parent)
    : SongItem(labels, icon, myFlags, hazChildren, couldFetchMore, parent)
{}

void UnorderedSongItem::onDoubleClicked(Controller *controller)
{
    controller->addAndPlaySong(uri());
}
