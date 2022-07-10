#include "songsitem.h"
#include "controller.h"
#include "iconnames.h"
#include "strformats.h"
#include "unorderedentitysongitem.h"

SongsItem::SongsItem(Item *parent)
    : Item({"Songs"}, QIcon::fromTheme(IconNames::Database), Qt::ItemIsEnabled, true, true, parent)
{}

QVector<Item *> SongsItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    for (auto entity : controller->listSongs()) {
        items.append(
            new UnorderedEntitySongItem({songLabel(mpd_entity_get_song(entity))}, entity, nullptr));
    }
    return items;
}
