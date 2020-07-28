#include "abstractsongitem.h"
#include "strformats.h"
#include <QIcon>

AbstractSongItem::AbstractSongItem(
    QIcon icon, Qt::ItemFlags myFlags, bool hazChildren, bool couldFetchMore, Item *parent)
    : Item(icon, myFlags, hazChildren, couldFetchMore, parent)
{}

QString AbstractSongItem::text(int column) const
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

QVariant AbstractSongItem::tooltip()
{
    return songToolTip(song());
}

QString AbstractSongItem::uri()
{
    return mpd_song_get_uri(song());
}
