#include "genreitem.h"
#include "controller.h"
#include "genreartistitem.h"
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
    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_GENRE, text(0)}};
    for (auto artist : controller->searchTags(MPD_TAG_ARTIST, filter)) {
        items.append(new GenreArtistItem(QIcon(":/icons/server-database.svg"),
                                         Qt::ItemIsEnabled,
                                         true,
                                         true,
                                         text(0),
                                         artist));
    }

    return items;
}
