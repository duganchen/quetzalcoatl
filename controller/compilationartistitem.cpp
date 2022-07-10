#include "compilationartistitem.h"
#include "compilationartistalbumitem.h"
#include "controller.h"
#include "iconnames.h"

CompilationArtistItem::CompilationArtistItem(const std::vector<QString> &labels,
                                             QString albumArtist,
                                             Item *parent)
    : Item(labels, QIcon::fromTheme(IconNames::Database), Qt::ItemIsEnabled, true, true, parent)

    , m_albumArtist(albumArtist)
{}

QVector<Item *> CompilationArtistItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;

    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_ALBUM_ARTIST, m_albumArtist}};
    for (auto album : controller->searchTags(MPD_TAG_ALBUM, filter)) {
        items.append(new CompilationArtistAlbumItem({album}, m_albumArtist, album));
    }
    return items;
}
