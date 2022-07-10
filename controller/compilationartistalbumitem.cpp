#include "compilationartistalbumitem.h"
#include "controller.h"
#include "iconnames.h"
#include "orderedmpdsongitem.h"
#include "strformats.h"

CompilationArtistAlbumItem::CompilationArtistAlbumItem(const std::vector<QString> &labels,
                                                       QString albumArtist,
                                                       QString album,
                                                       Item *parent)
    : Item(labels, QIcon::fromTheme(IconNames::OpticalAudio), Qt::ItemIsEnabled, true, true, parent)
    , m_albumArtist(albumArtist)
    , m_album(album)
{}

QVector<Item *> CompilationArtistAlbumItem::fetchMore(Controller *controller)
{
    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_ALBUM_ARTIST, m_albumArtist},
                                                 {MPD_TAG_ALBUM, m_album}};
    auto songs = controller->searchSongs(filter);
    QVector<Item *> items;
    for (auto song : songs) {
        items.append(new OrderedMPDSongItem({songLabel(song)}, song));
    }

    return items;
}
