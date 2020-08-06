#include "composeritem.h"
#include "composeralbumitem.h"
#include "composersongsitem.h"

ComposerItem::ComposerItem(QString composer, Item *parent)
    : Item(QIcon::fromTheme("server-database"), Qt::ItemIsEnabled, true, true, parent)

    , m_composer(composer)
{}

QVector<Item *> ComposerItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    items.append(new ComposerSongsItem(m_composer));

    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_COMPOSER, m_composer}};
    for (auto album : controller->searchTags(MPD_TAG_ALBUM, filter)) {
        items.append(new ComposerAlbumItem(m_composer, album));
    }
    return items;
}

QString ComposerItem::text(int column) const
{
    if (0 == column) {
        return m_composer;
    }

    return QString();
}
