#include "orderedentitysongitem.h"
#include "iconnames.h"

OrderedEntitySongItem::OrderedEntitySongItem(const std::vector<QString> &labels,
                                             mpd_entity *entity,
                                             QString tooltip)
    : OrderedSongItem(labels,
                      QIcon::fromTheme(IconNames::Audio),
                      Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled,
                      false,
                      false,
                      tooltip)
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
