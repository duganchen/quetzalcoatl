#include "unorderedentitysongitem.h"
#include "iconnames.h"

UnorderedEntitySongItem::UnorderedEntitySongItem(std::vector<QString> labels,
                                                 mpd_entity *entity,
                                                 QString tooltip)
    : UnorderedSongItem(labels,
                        QIcon::fromTheme(IconNames::Audio),
                        Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled,
                        false,
                        false,
                        tooltip)
    , m_entity(entity)
{}

UnorderedEntitySongItem::~UnorderedEntitySongItem()
{
    if (m_entity) {
        mpd_entity_free(m_entity);
        m_entity = nullptr;
    }
}

const mpd_song *UnorderedEntitySongItem::song() const
{
    return mpd_entity_get_song(m_entity);
}
