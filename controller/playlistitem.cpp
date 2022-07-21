#include "playlistitem.h"
#include "controller.h"
#include "iconnames.h"
#include "orderedentitysongitem.h"
#include "strformats.h"

PlaylistItem::PlaylistItem(std::vector<QString> labels)
    : Item(labels,
           QIcon::fromTheme(IconNames::Playlist),
           Qt::ItemIsEnabled | Qt::ItemIsEditable,
           true,
           true)
{}

PlaylistItem::~PlaylistItem() {}

QVector<Item *> PlaylistItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;

    for (auto entity : controller->listPlaylist(text(0))) {
        auto song = mpd_entity_get_song(entity);
        items.append(new OrderedEntitySongItem({songLabel(song)}, entity, songToolTip(song)));
    }
    return items;
}