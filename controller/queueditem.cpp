#include "queueditem.h"

#include "controller.h"
#include "iconnames.h"

QueuedItem::QueuedItem(std::vector<QString> labels, mpd_entity *entity)
    : SongItem(labels,
               QIcon::fromTheme(IconNames::Audio),
               Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled,
               false,
               false)
    , m_entity(entity)
{}

QueuedItem::~QueuedItem()
{
    if (m_entity) {
        mpd_entity_free(m_entity);
        m_entity = nullptr;
    }
}

unsigned QueuedItem::id()
{
    if (!m_entity) {
        // This should obviously never happen.
        return UINT_MAX;
    }

    return mpd_song_get_id(mpd_entity_get_song(m_entity));
}

void QueuedItem::onDoubleClicked(Controller *controller)
{
    controller->playSongEntity(m_entity);
}

const mpd_song *QueuedItem::song() const
{
    return mpd_entity_get_song(m_entity);
}

unsigned QueuedItem::duration()
{
    return mpd_song_get_duration(mpd_entity_get_song(m_entity));
}
