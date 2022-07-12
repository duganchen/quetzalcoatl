#include "composeritem.h"
#include "composeralbumitem.h"
#include "composersongsitem.h"
#include "controller.h"
#include "iconnames.h"

#include <mpd/client.h>

ComposerItem::ComposerItem(const std::vector<QString> &labels, Item *parent)
    : Item(labels, QIcon::fromTheme(IconNames::Database), Qt::ItemIsEnabled, true, true, parent)
{}

QVector<Item *> ComposerItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    items.append(new ComposerSongsItem(text(0)));

    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_COMPOSER, text(0)}};
    for (auto album : controller->searchTags(MPD_TAG_ALBUM, filter)) {
        items.append(new ComposerAlbumItem({album}, text(0)));
    }
    return items;
}