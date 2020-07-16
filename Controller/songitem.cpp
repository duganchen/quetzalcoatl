#include "songitem.h"
#include "timeformat.h"
#include <QDebug>

SongItem::SongItem(QIcon icon, mpd_entity *entity, AbstractItem *parent)
    : AbstractItem(icon, parent)
    , m_entity(entity)
{}

SongItem::~SongItem()
{
    if (m_entity) {
        mpd_entity_free(m_entity);
    }
}

QString SongItem::text(int column) const
{
    if (!m_entity || column > 1) {
        return QString();
    }

    const mpd_song *song = mpd_entity_get_song(m_entity);

    if (column == 0) {
        const char *title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
        if (!title) {
            // You're supposed to have a title. This is just a fallback to let you know to fix the tags.
            title = mpd_song_get_uri(song);
        }
        return title;
    }

    unsigned duration = mpd_song_get_duration(song);
    if (!duration) {
        // I have not personally encountered this.
        return "0:00";
    }

    return timeStr(duration);
}
