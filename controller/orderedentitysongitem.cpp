#include "orderedentitysongitem.h"
#include "iconnames.h"

OrderedEntitySongItem::OrderedEntitySongItem(mpd_entity *entity, Item *parent)
    : OrderedSongItem(QIcon::fromTheme(IconNames::Audio),
                      Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled,
                      false,
                      false,
                      parent)
    , m_entity(entity)
{}

OrderedEntitySongItem::~OrderedEntitySongItem()
{
    if (m_entity) {
        mpd_entity_free(m_entity);
        m_entity = nullptr;
    }
}

const mpd_song *OrderedEntitySongItem::song() const
{
    return mpd_entity_get_song(m_entity);
}
