#include "playlistitem.h"
#include "controller.h"
#include "iconnames.h"
#include "orderedentitysongitem.h"

PlaylistItem::PlaylistItem(QString path, Item *parent)
    : Item(QIcon::fromTheme(IconNames::Playlist),
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
        items.append(new OrderedEntitySongItem(entity));
    }
    return items;
}

QString PlaylistItem::text(int column) const
{
    if (0 == column) {
        return m_path;
    }
    return QString();
}
