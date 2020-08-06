#include "genreitem.h"
#include "controller.h"
#include "genreartistitem.h"
#include "genresongsitem.h"

#include <QDebug>

GenreItem::GenreItem(QString genre, Item *parent)
    : Item(QIcon::fromTheme(IconNames::Database), Qt::ItemIsEnabled, true, true, parent)
    , m_genre(genre)
{}

QVector<Item *> GenreItem::fetchMore(Controller *controller)
{
    Q_UNUSED(controller);
    QVector<Item *> items{new GenreSongsItem()};
    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_GENRE, text(0)}};
    for (auto artist : controller->searchTags(MPD_TAG_ARTIST, filter)) {
        items.append(new GenreArtistItem(m_genre, artist));
    }

    return items;
}

QString GenreItem::text(int column) const
{
    if (0 == column) {
        return m_genre;
    }

    return QString();
}
