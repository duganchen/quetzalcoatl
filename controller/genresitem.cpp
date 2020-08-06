#include "genresitem.h"
#include "controller.h"
#include "genreitem.h"

#include <QDebug>

GenresItem::GenresItem(Item *parent)
    : Item(QIcon::fromTheme("server-database"), Qt::ItemIsEnabled, true, true, parent)
{}

QString GenresItem::text(int column) const
{
    if (0 == column) {
        return "Genres";
    }
    return QString();
}

QVector<Item *> GenresItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    for (auto genre : controller->listTags(MPD_TAG_GENRE)) {
        items.append(new GenreItem(genre));
    }
    return items;
}
