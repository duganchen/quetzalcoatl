#include "genresitem.h"
#include "controller.h"

#include <QDebug>

GenresItem::GenresItem(QIcon icon,
                       Qt::ItemFlags myFlags,
                       bool hazChildren,
                       bool couldFetchMore,
                       QString text,
                       Item *parent)
    : DBItem(icon, myFlags, hazChildren, couldFetchMore, text, parent)
{}

QVector<Item *> GenresItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    for (auto value : controller->searchTags(MPD_TAG_GENRE)) {
        items.append(new DBItem(QIcon(), Qt::ItemIsEnabled, true, true, value));
    }
    return items;
}
