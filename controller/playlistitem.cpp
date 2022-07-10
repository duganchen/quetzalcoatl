#include "playlistitem.h"
#include "controller.h"
#include "iconnames.h"
#include "orderedentitysongitem.h"
#include "strformats.h"

PlaylistItem::PlaylistItem(std::vector<QString> labels, QString path, Item *parent)
    : Item(labels,
           QIcon::fromTheme(IconNames::Playlist),
           Qt::ItemIsEnabled | Qt::ItemIsEditable,
           true,
           true,
           parent)
    , m_path(path)
{}

PlaylistItem::~PlaylistItem() {}

QVector<Item *> PlaylistItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;

    for (auto entity : controller->listPlaylist(m_path)) {
        items.append(new OrderedEntitySongItem({songLabel(mpd_entity_get_song(entity))}, entity));
    }
    return items;
}