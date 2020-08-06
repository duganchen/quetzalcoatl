#include "composersongsitem.h"
#include "unorderedmpdsongitem.h"

ComposerSongsItem::ComposerSongsItem(QString composer, Item *parent)
    : Item(QIcon::fromTheme("server-database"), Qt::ItemIsEnabled, true, true, parent)
    , m_composer(composer)
{}

QVector<Item *> ComposerSongsItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    QVector<QPair<mpd_tag_type, QString>> tags{{MPD_TAG_COMPOSER, m_composer}};
    for (auto song : controller->searchSongs(tags)) {
        items.append(new UnorderedMPDSongItem(song));
    }
    return items;
}

QString ComposerSongsItem::text(int column) const
{
    if (0 == column) {
        return "All Songs";
    }

    return QString();
}
