#include "genreartistitem.h"
#include "controller.h"
#include "genreartistalbumitem.h"
#include "genreartistitem.h"
#include "genreartistsongsitem.h"
#include "iconnames.h"

GenreArtistItem::GenreArtistItem(const std::vector<QString> &labels,
                                 QString genre,
                                 QString artist,
                                 Item *parent)
    : Item(labels, QIcon::fromTheme(IconNames::Database), Qt::ItemIsEnabled, true, true, parent)

    , m_genre(genre)
    , m_artist(artist)
{}

QVector<Item *> GenreArtistItem::fetchMore(Controller *controller)
{
    Q_UNUSED(controller);
    QVector<Item *> items;
    items.append(new GenreArtistSongsItem(m_genre, m_artist));

    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_GENRE, m_genre},
                                                 {MPD_TAG_ARTIST, text(0)}};
    for (auto album : controller->searchTags(MPD_TAG_ALBUM, filter)) {
        items.append(new GenreArtistAlbumItem({album}, m_genre, m_artist, album));
    }
    return items;
}
