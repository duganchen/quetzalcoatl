#include "songitem.h"
#include "strformats.h"
#include <QIcon>

SongItem::SongItem(
    QIcon icon, Qt::ItemFlags myFlags, bool hazChildren, bool couldFetchMore, Item *parent)
    : Item(icon, myFlags, hazChildren, couldFetchMore, parent)
{}

QString SongItem::text(int column) const
{
    if (!song()) {
        return QString();
    }

    switch (column) {
    case 0:
        return songLabel(song());
    case 1:
        // This just doesn't get executed if we only have one column.
        return songDuration(song());
    default:
        return QString();
    }
}

QVariant SongItem::tooltip()
{
    return songToolTip(song());
}

QString SongItem::uri()
{
    return mpd_song_get_uri(song());
}
