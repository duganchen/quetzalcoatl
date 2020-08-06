#include "composersitem.h"
#include "composeritem.h"

ComposersItem::ComposersItem(Item *parent)
    : Item(QIcon::fromTheme("server-database"), Qt::ItemIsEnabled, true, true, parent)
{}

QString ComposersItem::text(int column) const
{
    if (0 == column) {
        return "Composers";
    }
    return QString();
}

QVector<Item *> ComposersItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    for (auto composer : controller->listTags(MPD_TAG_COMPOSER)) {
        items.append(new ComposerItem(composer));
    }
    return items;
}
