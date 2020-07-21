#include "genresongsitem.h"
#include "controller.h"
#include "dbsongitem.h"

#include <QDebug>

GenreSongsItem::GenreSongsItem(QIcon icon,
                               Qt::ItemFlags myFlags,
                               bool hazChildren,
                               bool couldFetchMore,
                               QString text,
                               Item *parent)
    : DBItem(icon, myFlags, hazChildren, couldFetchMore, text, parent)
{}

QVector<Item *> GenreSongsItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    QVector<QPair<mpd_tag_type, QString>> tags{{MPD_TAG_GENRE, parent()->text(0)}};
    for (auto song : controller->searchSongs(tags)) {
        items.append(
            new DBSongItem(QIcon(":/icons/audio-x-generic.svg"), Qt::ItemIsEnabled, false, song));
    }
    return items;
}
