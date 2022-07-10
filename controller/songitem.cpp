#include "songitem.h"
#include "strformats.h"
#include <QIcon>

SongItem::SongItem(const std::vector<QString> &labels,
                   QIcon icon,
                   Qt::ItemFlags myFlags,
                   bool hazChildren,
                   bool couldFetchMore,
                   Item *parent)
    : Item(labels, icon, myFlags, hazChildren, couldFetchMore, parent)
{}

QVariant SongItem::tooltip()
{
    return songToolTip(song());
}

QString SongItem::uri()
{
    return mpd_song_get_uri(song());
}
