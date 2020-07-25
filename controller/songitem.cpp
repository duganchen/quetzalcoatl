#include "songitem.h"
#include "strformats.h"
#include <QDebug>
#include <QStringBuilder>
#include <QStringLiteral>

SongItem::SongItem(mpd_song *song, Item *parent)
    : Item(QIcon(":/icons/audio-x-generic.svg"), Qt::ItemIsEnabled, false, false, parent)
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
        return songLabel(m_song);
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
