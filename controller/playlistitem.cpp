#include "playlistitem.h"
#include "controller.h"
#include "iconnames.h"
#include "orderedentitysongitem.h"

PlaylistItem::PlaylistItem(mpd_playlist *playlist, Item *parent)
    : Item(QIcon::fromTheme(IconNames::Playlist),
           Qt::ItemIsEnabled | Qt::ItemIsEditable,
           true,
           true,
           parent)
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
        items.append(new OrderedEntitySongItem(entity));
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
