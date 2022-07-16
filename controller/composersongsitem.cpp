#include "composersongsitem.h"
#include "controller.h"
#include "iconnames.h"
#include "strformats.h"
#include "unorderedmpdsongitem.h"

ComposerSongsItem::ComposerSongsItem(QString composer)
    : Item({"All Songs"}, QIcon::fromTheme(IconNames::Database), Qt::ItemIsEnabled, true, true)
    , m_composer(composer)
{}

QVector<Item *> ComposerSongsItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    QVector<QPair<mpd_tag_type, QString>> tags{{MPD_TAG_COMPOSER, m_composer}};
    for (auto song : controller->searchSongs(tags)) {
        items.append(new UnorderedMPDSongItem({songLabel(song)}, song));
    }
    return items;
}
