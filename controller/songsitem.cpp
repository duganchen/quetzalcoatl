#include "songsitem.h"
#include "controller.h"
#include "iconnames.h"
#include "unorderedentitysongitem.h"

SongsItem::SongsItem(Item *parent)
    : Item(QIcon::fromTheme(IconNames::Database), Qt::ItemIsEnabled, true, true, parent)
{}

QString SongsItem::text(int column) const
{
    if (0 == column) {
        return "Songs";
    }
    return QString();
}

QVector<Item *> SongsItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    for (auto entity : controller->listSongs()) {
        items.append(new UnorderedEntitySongItem(entity));
    }
    return items;
}
