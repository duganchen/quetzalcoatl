#include "songitem.h"
#include "strformats.h"
#include <QIcon>

SongItem::SongItem(const std::vector<QString> &labels,
                   QIcon icon,
                   Qt::ItemFlags myFlags,
                   bool hazChildren,
                   bool couldFetchMore,
                   QString tooltip)
    : Item(labels, icon, myFlags, hazChildren, couldFetchMore, tooltip)
{}

QString SongItem::uri()
{
    return mpd_song_get_uri(song());
}
