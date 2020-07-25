#include "playlistitem.h"
#include "songentityitem.h"
#include <QDebug>

PlaylistItem::PlaylistItem(mpd_playlist *playlist, Item *parent)
    : Item(QIcon(":/icons/view-media-playlist.svg"), Qt::ItemIsEnabled, true, true, parent)
    , m_playlist(playlist)
{}

PlaylistItem::~PlaylistItem()
{
    if (m_playlist) {
        mpd_playlist_free(m_playlist);
    }
}

QVector<Item *> PlaylistItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;

    for (auto entity : controller->listPlaylist(m_playlist)) {
        items.append(new SongEntityItem(entity));
    }
    return items;
}

QString PlaylistItem::text(int column) const
{
    if (0 == column && m_playlist) {
        auto path = mpd_playlist_get_path(m_playlist);
        return path;
    }
    return QString();
}
