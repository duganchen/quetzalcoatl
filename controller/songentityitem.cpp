#include "songentityitem.h"
#include "controller.h"
#include "strformats.h"
#include <QStringBuilder>

SongEntityItem::SongEntityItem(mpd_entity *entity, Item *parent)
    : Item(QIcon(":/icons/audio-x-generic.svg"),
           Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled,
           false,
           false,
           parent)
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

QString SongEntityItem::uri()
{
    return mpd_song_get_uri(mpd_entity_get_song(m_entity));
}

const mpd_song *SongEntityItem::song()
{
    return mpd_entity_get_song(m_entity);
}
