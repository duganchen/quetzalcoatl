#include "songitem.h"
#include "timeformat.h"
#include <QDebug>
#include <QStringBuilder>
#include <QStringLiteral>

SongItem::SongItem(
    QIcon icon, Qt::ItemFlags myFlags, bool hazChildren, mpd_entity *entity, Item *parent)
    : Item(icon, myFlags, hazChildren, parent)
    , m_entity(entity)
{}

SongItem::~SongItem()
{
    if (m_entity) {
        mpd_entity_free(m_entity);
        m_entity = nullptr;
    }
}

QString SongItem::text(int column) const
{
    if (!m_entity || column > 1) {
        return QString();
    }

    const mpd_song *song = mpd_entity_get_song(m_entity);

    if (!song) {
        // Not really sure how this could happen, but it seems to...
        return QString();
    }

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

QVariant SongItem::tooltip()
{
    if (!m_entity) {
        return QVariant();
    }

    QStringList metadata;
    auto song = mpd_entity_get_song(m_entity);

    QVector<mpd_tag_type> tagTypes{MPD_TAG_TITLE,
                                   MPD_TAG_TRACK,
                                   MPD_TAG_ALBUM,
                                   MPD_TAG_DISC,
                                   MPD_TAG_ARTIST,
                                   MPD_TAG_ALBUM_ARTIST,
                                   MPD_TAG_COMPOSER,
                                   MPD_TAG_GENRE};

    QString tagValue;
    QString tagName;
    for (auto tagType : tagTypes) {
        tagValue = mpd_song_get_tag(song, tagType, 0);
        if (tagValue.isEmpty()) {
            continue;
        }

        QString tagName = mpd_tag_name(tagType);
        metadata.append(tagName % ": " % tagValue);
    }

    return metadata.join("\n");
}

unsigned SongItem::id()
{
    if (!m_entity) {
        return -1;
    }

    return mpd_song_get_id(mpd_entity_get_song(m_entity));
}
