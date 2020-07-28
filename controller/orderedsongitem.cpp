#include "orderedsongitem.h"
#include "controller.h"

OrderedSongItem::OrderedSongItem(
    QIcon icon, Qt::ItemFlags myFlags, bool hazChildren, bool couldFetchMore, Item *parent)
    : SongItem(icon, myFlags, hazChildren, couldFetchMore, parent)
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
