#include "artistalbumitem.h"
#include "controller.h"
#include "iconnames.h"
#include "orderedmpdsongitem.h"
#include "strformats.h"

ArtistAlbumItem::ArtistAlbumItem(const std::vector<QString> &labels,
                                 QString artist,
                                 QString album,
                                 Item *parent)
    : Item(labels, QIcon::fromTheme(IconNames::OpticalAudio), Qt::ItemIsEnabled, true, true, parent)
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
        items.append(new OrderedMPDSongItem({songLabel(song)}, song));
    }

    return items;
}
