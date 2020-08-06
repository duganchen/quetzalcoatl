#include "compilationartistitem.h"
#include "compilationartistalbumitem.h"

CompilationArtistItem::CompilationArtistItem(QString albumArtist, Item *parent)
    : Item(QIcon::fromTheme("server-database"), Qt::ItemIsEnabled, true, true, parent)

    , m_albumArtist(albumArtist)
{}

QVector<Item *> CompilationArtistItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;

    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_ALBUM_ARTIST, m_albumArtist}};
    for (auto album : controller->searchTags(MPD_TAG_ALBUM, filter)) {
        items.append(new CompilationArtistAlbumItem(m_albumArtist, album));
    }
    return items;
}

QString CompilationArtistItem::text(int column) const
{
    if (0 == column) {
        return m_albumArtist;
    }

    return QString();
}
