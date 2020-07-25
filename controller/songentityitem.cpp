#include "songentityitem.h"
#include "strformats.h"
#include <QStringBuilder>

SongEntityItem::SongEntityItem(mpd_entity *entity, Item *parent)
    : Item(QIcon(":/icons/audio-x-generic.svg"), Qt::ItemIsEnabled, false, false, parent)
    , m_entity(entity)
{}

SongEntityItem::~SongEntityItem()
{
    if (m_entity) {
        mpd_entity_free(m_entity);
        m_entity = nullptr;
    }
}

QString SongEntityItem::text(int column) const
{
    if (!m_entity || column > 1) {
        return QString();
    }

    return songEntityLabel(m_entity);
}

QVariant SongEntityItem::tooltip()
{
    return songToolTip(song());
}

const mpd_song *SongEntityItem::song()
{
    return mpd_entity_get_song(m_entity);
}
