#include "genreitem.h"
#include "controller.h"
#include "genresongsitem.h"

#include <QDebug>

GenreItem::GenreItem(QIcon icon,
                     Qt::ItemFlags myFlags,
                     bool hazChildren,
                     bool couldFetchMore,
                     QString text,
                     Item *parent)
    : DBItem(icon, myFlags, hazChildren, couldFetchMore, text, parent)
{}

QVector<Item *> GenreItem::fetchMore(Controller *controller)
{
    Q_UNUSED(controller);
    QVector<Item *> items{new GenreSongsItem(QIcon(":/icons/server-database.svg"),
                                             Qt::ItemIsEnabled,
                                             true,
                                             true,
                                             "All Songs")};
    return items;
}
