#include "compilationartistalbumitem.h"
#include "orderedmpdsongitem.h"

CompilationArtistAlbumItem::CompilationArtistAlbumItem(QString albumArtist,
                                                       QString album,
                                                       Item *parent)
    : Item(QIcon(":/icons/media-optical-audio.svg"), Qt::ItemIsEnabled, true, true, parent)
    , m_albumArtist(albumArtist)
    , m_album(album)
{}

QVector<Item *> CompilationArtistAlbumItem::fetchMore(Controller *controller)
{
    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_ARTIST, m_albumArtist},
                                                 {MPD_TAG_ALBUM, m_album}};
    auto songs = controller->searchSongs(filter);
    QVector<Item *> items;
    for (auto song : songs) {
        items.append(new OrderedMPDSongItem(song));
    }

    return items;
}

QString CompilationArtistAlbumItem::text(int column) const
{
    if (0 == column) {
        return m_album;
    }
    return QString();
}
