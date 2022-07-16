#include "compilationartistalbumitem.h"
#include "controller.h"
#include "iconnames.h"
#include "orderedmpdsongitem.h"
#include "strformats.h"

CompilationArtistAlbumItem::CompilationArtistAlbumItem(const std::vector<QString> &labels,
                                                       QString albumArtist)
    : Item(labels, QIcon::fromTheme(IconNames::OpticalAudio), Qt::ItemIsEnabled, true, true)
    , m_albumArtist(albumArtist)
{}

QVector<Item *> CompilationArtistAlbumItem::fetchMore(Controller *controller)
{
    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_ALBUM_ARTIST, m_albumArtist},
                                                 {MPD_TAG_ALBUM, text(0)}};
    auto songs = controller->searchSongs(filter);
    QVector<Item *> items;
    for (auto song : songs) {
        items.append(new OrderedMPDSongItem({songLabel(song)}, song));
    }

    return items;
}
