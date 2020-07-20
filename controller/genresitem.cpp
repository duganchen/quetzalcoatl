#include "genresitem.h"

#include <QDebug>

GenresItem::GenresItem(
    QIcon icon, Qt::ItemFlags myFlags, bool hazChildren, QString text, Item *parent)
    : DBItem(icon, myFlags, hazChildren, text, parent)
{}

QVector<Item *> GenresItem::fetchMore(Controller *controller)
{
    qDebug() << "Fetching genres";
    setCanfetchMore(false);
    Q_UNUSED(controller)
    QVector<Item *> items;
    return items;
}
