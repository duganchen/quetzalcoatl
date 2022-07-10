#include "genresongsitem.h"
#include "controller.h"
#include "iconnames.h"
#include "strformats.h"
#include "unorderedmpdsongitem.h"

GenreSongsItem::GenreSongsItem(Item *parent)
    : Item({"All Songs"}, QIcon(IconNames::Database), Qt::ItemIsEnabled, true, true, parent)
{}

QVector<Item *> GenreSongsItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    QVector<QPair<mpd_tag_type, QString>> tags{{MPD_TAG_GENRE, parent()->text(0)}};
    for (auto song : controller->searchSongs(tags)) {
        items.append(new UnorderedMPDSongItem({songLabel(song)}, song));
    }
    return items;
}