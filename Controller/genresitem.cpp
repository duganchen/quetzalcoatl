#include "genresitem.h"

#include <QDebug>

GenresItem::GenresItem(
    QIcon icon, Qt::ItemFlags myFlags, bool hazChildren, QString text, Item *parent)
    : DBItem(icon, myFlags, hazChildren, text, parent)
{}

void GenresItem::fetchMore()
{
    qDebug() << "Fetching more genres";
    DBItem::fetchMore();
}
