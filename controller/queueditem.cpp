#include "queueditem.h"
#include "strformats.h"
#include <QDebug>
#include <QStringBuilder>
#include <QStringLiteral>

QueuedItem::QueuedItem(mpd_entity *entity, Item *parent)
    : Item(QIcon(":/icons/audio-x-generic.svg"),
           Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled,
           false,
           false,
           parent)
    , m_entity(entity)
{}

QueuedItem::~QueuedItem()
{
    if (m_entity) {
        mpd_entity_free(m_entity);
        m_entity = nullptr;
    }
}

QString QueuedItem::text(int column) const
{
    if (!m_entity) {
        return QString();
    }

    switch (column) {
    case 0:
        return songEntityLabel(m_entity);
    case 1:
        return songDuration(mpd_entity_get_song(m_entity));
    default:
        return QString();
    }
}

QVariant QueuedItem::tooltip()
{
    return songToolTip(mpd_entity_get_song(m_entity));
}

unsigned QueuedItem::id()
{
    if (!m_entity) {
        // This should obviously never happen.
        return UINT_MAX;
    }

    return mpd_song_get_id(mpd_entity_get_song(m_entity));
}
