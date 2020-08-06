#include "composeralbumitem.h"
#include "orderedmpdsongitem.h"

ComposerAlbumItem::ComposerAlbumItem(QString composer, QString album, Item *parent)
    : Item(QIcon::fromTheme("media-optical-audio"), Qt::ItemIsEnabled, true, true, parent)
    , m_composer(composer)
    , m_album(album)
{}

QVector<Item *> ComposerAlbumItem::fetchMore(Controller *controller)
{
    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_COMPOSER, m_composer},
                                                 {MPD_TAG_ALBUM, m_album}};
    auto songs = controller->searchSongs(filter);
    QVector<Item *> items;
    for (auto song : songs) {
        items.append(new OrderedMPDSongItem(song));
    }

    return items;
}

QString ComposerAlbumItem::text(int column) const
{
    if (0 == column) {
        return m_album;
    }
    return QString();
}
