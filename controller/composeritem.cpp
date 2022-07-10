#include "composeritem.h"
#include "composeralbumitem.h"
#include "composersongsitem.h"
#include "controller.h"
#include "iconnames.h"

#include <mpd/client.h>

ComposerItem::ComposerItem(const std::vector<QString> &labels, QString composer, Item *parent)
    : Item(labels, QIcon::fromTheme(IconNames::Database), Qt::ItemIsEnabled, true, true, parent)

    , m_composer(composer)
{}

QVector<Item *> ComposerItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    items.append(new ComposerSongsItem(m_composer));

    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_COMPOSER, m_composer}};
    for (auto album : controller->searchTags(MPD_TAG_ALBUM, filter)) {
        items.append(new ComposerAlbumItem({album}, m_composer, album));
    }
    return items;
}