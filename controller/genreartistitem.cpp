#include "genreartistitem.h"
#include "controller.h"
#include "genreartistalbumitem.h"
#include "genreartistitem.h"
#include "genreartistsongsitem.h"

#include <QDebug>

GenreArtistItem::GenreArtistItem(QIcon icon,
                                 Qt::ItemFlags myFlags,
                                 bool hazChildren,
                                 bool couldFetchMore,
                                 QString genre,
                                 QString text,
                                 Item *parent)
    : DBItem(icon, myFlags, hazChildren, couldFetchMore, text, parent)
    , m_genre(genre)
{}

QVector<Item *> GenreArtistItem::fetchMore(Controller *controller)
{
    Q_UNUSED(controller);
    QVector<Item *> items;
    items.append(new GenreArtistSongsItem(QIcon(":/icons/server-database.svg"),
                                          Qt::ItemIsEnabled,
                                          true,
                                          true,
                                          m_genre,
                                          text(0),
                                          "All Songs",
                                          this));

    QVector<QPair<mpd_tag_type, QString>> filter{{MPD_TAG_GENRE, m_genre},
                                                 {MPD_TAG_ARTIST, text(0)}};
    for (auto album : controller->searchTags(MPD_TAG_ALBUM, filter)) {
        items.append(new GenreArtistAlbumItem(QIcon(":/icons/media-optical-audio.svg"),
                                              Qt::ItemIsEnabled,
                                              true,
                                              true,
                                              m_genre,
                                              text(0),
                                              album));
    }
    return items;
}
