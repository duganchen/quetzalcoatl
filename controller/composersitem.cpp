#include "composersitem.h"
#include "composeritem.h"
#include "controller.h"
#include "iconnames.h"

ComposersItem::ComposersItem()
    : Item({"Composers"}, QIcon::fromTheme(IconNames::Database), Qt::ItemIsEnabled, true, true)
{}

QVector<Item *> ComposersItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    for (auto composer : controller->listTags(MPD_TAG_COMPOSER)) {
        items.append(new ComposerItem({composer}));
    }
    return items;
}
