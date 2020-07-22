#include "songitem.h"
#include "timeformat.h"
#include <QDebug>
#include <QStringBuilder>
#include <QStringLiteral>

SongItem::SongItem(
    QIcon icon, Qt::ItemFlags myFlags, bool hazChildren, mpd_song *song, Item *parent)
    : Item(icon, myFlags, hazChildren, parent)
    , m_song(song)
{}

SongItem::~SongItem()
{
    if (m_song) {
        mpd_song_free(m_song);
        m_song = nullptr;
    }
}

QString SongItem::text(int column) const
{
    if (!m_song || column > 1) {
        return QString();
    }

    if (column == 0) {
        const char *title = mpd_song_get_tag(m_song, MPD_TAG_TITLE, 0);
        if (!title) {
            // You're supposed to have a title. This is just a fallback to let you know to fix the tags.
            title = mpd_song_get_uri(m_song);
        }
        return title;
    }

    unsigned duration = mpd_song_get_duration(m_song);
    if (!duration) {
        // I have not personally encountered this.
        return "0:00";
    }

    return timeStr(duration);
}

QVariant SongItem::tooltip()
{
    if (!m_song) {
        return QVariant();
    }

    QStringList metadata;

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
        tagValue = mpd_song_get_tag(m_song, tagType, 0);
        if (tagValue.isEmpty()) {
            continue;
        }

        QString tagName = mpd_tag_name(tagType);
        metadata.append(tagName % ": " % tagValue);
    }

    return metadata.join("\n");
}
