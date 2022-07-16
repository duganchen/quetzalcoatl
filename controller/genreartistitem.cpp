#include "genreartistitem.h"
#include "controller.h"
#include "genreartistalbumitem.h"
#include "genreartistitem.h"
#include "genreartistsongsitem.h"
#include "iconnames.h"

GenreArtistItem::GenreArtistItem(const std::vector<QString> &labels, QString genre)
    : Item(labels, QIcon::fromTheme(IconNames::Database), Qt::ItemIsEnabled, true, true)

    , m_genre(genre)
{}

QVector<Item *> GenreArtistItem::fetchMore(Controller *controller)
{
    Q_UNUSED(controller);
    QVector<Item *> items;
    items.append(new GenreArtistSongsItem(m_genre, text(0)));

    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_GENRE, m_genre},
                                                 {MPD_TAG_ARTIST, text(0)}};
    for (auto album : controller->searchTags(MPD_TAG_ALBUM, filter)) {
        items.append(new GenreArtistAlbumItem({album}, m_genre, text(0)));
    }
    return items;
}
