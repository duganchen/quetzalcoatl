#include "artistalbumitem.h"
#include "orderedmpdsongitem.h"

ArtistAlbumItem::ArtistAlbumItem(QString artist, QString album, Item *parent)
    : Item(QIcon::fromTheme(IconNames::OpticalAudio), Qt::ItemIsEnabled, true, true, parent)
    , m_artist(artist)
    , m_album(album)
{}

QVector<Item *> ArtistAlbumItem::fetchMore(Controller *controller)
{
    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_ARTIST, m_artist},
                                                 {MPD_TAG_ALBUM, m_album}};
    auto songs = controller->searchSongs(filter);
    QVector<Item *> items;
    for (auto song : songs) {
        items.append(new OrderedMPDSongItem(song));
    }

    return items;
}

QString ArtistAlbumItem::text(int column) const
{
    if (0 == column) {
        return m_album;
    }
    return QString();
}
