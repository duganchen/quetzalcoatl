#include "directoryitem.h"
#include "controller.h"
#include "iconnames.h"
#include "strformats.h"
#include "unorderedentitysongitem.h"

DirectoryItem::DirectoryItem(const std::vector<QString> &labels, mpd_entity *entity)
    : Item(labels, QIcon::fromTheme(IconNames::Harddisk), Qt::ItemIsEnabled, true, true)
    , m_entity(entity)
{}

DirectoryItem::~DirectoryItem()
{
    if (m_entity) {
        mpd_entity_free(m_entity);
    }
}

QVector<Item *> DirectoryItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    for (auto entity : controller->listDir(m_entity)) {
        if (mpd_entity_get_type(entity) == MPD_ENTITY_TYPE_DIRECTORY) {
            items.append(
                new DirectoryItem({mpd_directory_get_path(mpd_entity_get_directory(entity))},
                                  entity));
        } else {
            items.append(
                new UnorderedEntitySongItem({songLabel(mpd_entity_get_song(entity))}, entity));
        }
    }
    return items;
}
