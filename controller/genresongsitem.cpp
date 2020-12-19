#include "genresongsitem.h"
#include "controller.h"
#include "iconnames.h"
#include "unorderedmpdsongitem.h"

GenreSongsItem::GenreSongsItem(Item *parent)
    : Item(QIcon(IconNames::Database), Qt::ItemIsEnabled, true, true, parent)
{}

QVector<Item *> GenreSongsItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    QVector<QPair<mpd_tag_type, QString>> tags{{MPD_TAG_GENRE, parent()->text(0)}};
    for (auto song : controller->searchSongs(tags)) {
        items.append(new UnorderedMPDSongItem(song));
    }
    return items;
}

QString GenreSongsItem::text(int column) const
{
    if (0 == column) {
        return "All Songs";
    }

    return QString();
}
