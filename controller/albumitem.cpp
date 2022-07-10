#include "albumitem.h"
#include "controller.h"
#include "iconnames.h"
#include "orderedmpdsongitem.h"
#include "strformats.h"

AlbumItem::AlbumItem(const std::vector<QString> &labels, QString album, Item *parent)
    : Item(labels, QIcon::fromTheme(IconNames::OpticalAudio), Qt::ItemIsEnabled, true, true, parent)
    , m_album(album)
{}

QVector<Item *> AlbumItem::fetchMore(Controller *controller)
{
    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_ALBUM, m_album}};
    auto songs = controller->searchSongs(filter);
    QVector<Item *> items;
    for (auto song : songs) {
        items.append(new OrderedMPDSongItem({songLabel(song)}, song));
    };
    return items;
}
