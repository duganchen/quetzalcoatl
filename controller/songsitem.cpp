#include "songsitem.h"
#include "controller.h"
#include "iconnames.h"
#include "strformats.h"
#include "unorderedentitysongitem.h"

SongsItem::SongsItem()
    : Item({"Songs"}, QIcon::fromTheme(IconNames::Database), Qt::ItemIsEnabled, true, true)
{}

QVector<Item *> SongsItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    for (auto entity : controller->listSongs()) {
        auto song = mpd_entity_get_song(entity);
        items.append(new UnorderedEntitySongItem({songLabel(song)}, entity, songToolTip(song)));
    }
    return items;
}
