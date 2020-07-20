#include "genresitem.h"
#include "controller.h"

#include <QDebug>

GenresItem::GenresItem(
    QIcon icon, Qt::ItemFlags myFlags, bool hazChildren, QString text, Item *parent)
    : DBItem(icon, myFlags, hazChildren, text, parent)
{}

QVector<Item *> GenresItem::fetchMore(Controller *controller)
{
    setCanfetchMore(false);
    QVector<Item *> items;
    for (auto value : controller->searchTags(MPD_TAG_GENRE)) {
        items.append(new DBItem(QIcon(), Qt::ItemIsEnabled, true, value));
    }
    return items;
}
