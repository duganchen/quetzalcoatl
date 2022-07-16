#include "compilationartistitem.h"
#include "compilationartistalbumitem.h"
#include "controller.h"
#include "iconnames.h"

CompilationArtistItem::CompilationArtistItem(const std::vector<QString> &labels)
    : Item(labels, QIcon::fromTheme(IconNames::Database), Qt::ItemIsEnabled, true, true)

{}

QVector<Item *> CompilationArtistItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;

    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_ALBUM_ARTIST, text(0)}};
    for (auto album : controller->searchTags(MPD_TAG_ALBUM, filter)) {
        items.append(new CompilationArtistAlbumItem({album}, text(0)));
    }
    return items;
}
