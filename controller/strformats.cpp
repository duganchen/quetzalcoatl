#include "strformats.h"
#include <QStringBuilder>
#include <QStringList>

QString timeStr(unsigned duration)
{
    // Can't remember where I got this time format from. It might have been Foobar2000.

    unsigned seconds = duration % 60;
    unsigned minutes = duration % 3600 / 60;
    unsigned hours = duration / 3600;

    // For the string formatting:
    // Re: https://stackoverflow.com/a/2618444/240515

    if (!hours && !minutes) {
        return QStringLiteral("0:%1").arg(seconds, 2, 10, QLatin1Char('0'));
    }

    if (!hours) {
        return QStringLiteral("%1:%2").arg(QString::number(minutes), leftPad(seconds));
    }

    return QStringLiteral("%1:%2:%3").arg(QString::number(hours), leftPad(minutes), leftPad(seconds));
}

QString leftPad(unsigned i)
{
    // Left-zero-pad to two digits.
    return QStringLiteral("%1").arg(i, 2, 10, QLatin1Char('0'));
}

QString songLabel(const mpd_song *song)
{
    if (!song) {
        return QString();
    }

    const char *title = mpd_song_get_tag(song, MPD_TAG_TITLE, 0);
    if (!title) {
        // You're supposed to have a title. This is just a fallback to let you know to fix the tags.
        title = mpd_song_get_uri(song);
    }

    return title;
}

QString songEntityLabel(mpd_entity *entity)
{
    if (!entity) {
        return QString();
    }

    if (mpd_entity_get_type(entity) != MPD_ENTITY_TYPE_SONG) {
        return QString();
    }

    auto song = mpd_entity_get_song(entity);
    return songLabel(song);
}

QString dirEntryLabel(mpd_entity *entity)
{
    if (!entity) {
        return QString();
    }

    if (mpd_entity_get_type(entity) == MPD_ENTITY_TYPE_DIRECTORY) {
        return mpd_directory_get_path(mpd_entity_get_directory(entity));
    }

    return songEntityLabel(entity);
}

QString songToolTip(mpd_song *song)
{
    if (!song) {
        return QString();
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
        tagValue = mpd_song_get_tag(song, tagType, 0);
        if (tagValue.isEmpty()) {
            continue;
        }

        QString tagName = mpd_tag_name(tagType);
        metadata.append(tagName % ": " % tagValue);
    }

    return metadata.join("\n");
}

QString songDuration(mpd_song *song)
{
    unsigned duration = mpd_song_get_duration(song);
    if (!duration) {
        // I have not personally encountered this.
        return "0:00";
    }

    return timeStr(duration);
}
