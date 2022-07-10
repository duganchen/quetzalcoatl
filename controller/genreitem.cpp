#include "genreitem.h"
#include "controller.h"
#include "genreartistitem.h"
#include "genresongsitem.h"
#include "iconnames.h"

GenreItem::GenreItem(const std::vector<QString> &labels, QString genre, Item *parent)
    : Item(labels, QIcon::fromTheme(IconNames::Database), Qt::ItemIsEnabled, true, true, parent)
    , m_genre(genre)
{}

QVector<Item *> GenreItem::fetchMore(Controller *controller)
{
    Q_UNUSED(controller);
    QVector<Item *> items{new GenreSongsItem()};
    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_GENRE, text(0)}};
    for (auto artist : controller->searchTags(MPD_TAG_ARTIST, filter)) {
        items.append(new GenreArtistItem({artist}, m_genre, artist, nullptr));
    }

    return items;
}