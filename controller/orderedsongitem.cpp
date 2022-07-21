#include "orderedsongitem.h"
#include "controller.h"

OrderedSongItem::OrderedSongItem(std::vector<QString> labels,
                                 QIcon icon,
                                 Qt::ItemFlags myFlags,
                                 bool hazChildren,
                                 bool couldFetchMore,
                                 QString tooltip)
    : SongItem(labels, icon, myFlags, hazChildren, couldFetchMore, tooltip)
{}

void OrderedSongItem::onDoubleClicked(Controller *controller)
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
