#include "directoryitem.h"
#include "playlistsongitem.h"
#include <QDebug>

DirectoryItem::DirectoryItem(mpd_entity *entity, Item *parent)
    : Item(QIcon(":/icons/folder-favorites.svg"), Qt::ItemIsEnabled, true, true, parent)
    , m_entity(entity)
{}

DirectoryItem::~DirectoryItem()
{
    if (m_entity) {
        mpd_entity_free(m_entity);
    }
}

QString DirectoryItem::text(int column) const
{
    if (0 == column) {
        if (m_entity && mpd_entity_get_type(m_entity) == MPD_ENTITY_TYPE_DIRECTORY) {
            return mpd_directory_get_path(mpd_entity_get_directory(m_entity));
        }
        return "/";
    }
    return QString();
}

QVector<Item *> DirectoryItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    for (auto entity : controller->listDir(m_entity)) {
        if (mpd_entity_get_type(entity) == MPD_ENTITY_TYPE_DIRECTORY) {
            items.append(new DirectoryItem(entity));
        } else {
            items.append(new PlaylistSongItem(entity));
        }
    }
    return items;
}
