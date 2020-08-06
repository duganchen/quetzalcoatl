#include "genreartistitem.h"
#include "controller.h"
#include "genreartistalbumitem.h"
#include "genreartistitem.h"
#include "genreartistsongsitem.h"

#include <QDebug>

GenreArtistItem::GenreArtistItem(QString genre, QString artist, Item *parent)
    : Item(QIcon::fromTheme("server-database"), Qt::ItemIsEnabled, true, true, parent)

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
        items.append(new GenreArtistAlbumItem(m_genre, m_artist, album));
    }
    return items;
}

QString GenreArtistItem::text(int column) const
{
    if (0 == column) {
        return m_artist;
    }

    return QString();
}
