#include "artistsitem.h"
#include "artistitem.h"
#include "controller.h"
#include "iconnames.h"

ArtistsItem::ArtistsItem(Item *parent)
    : Item({"Artists"}, QIcon::fromTheme(IconNames::Database), Qt::ItemIsEnabled, true, true, parent)
{}

QVector<Item *> ArtistsItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    for (auto artist : controller->listTags(MPD_TAG_ARTIST)) {
        items.append(new ArtistItem({artist}, artist));
    }
    return items;
}
