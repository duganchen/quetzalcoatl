#include "songitem.h"
#include "strformats.h"
#include <QDebug>
#include <QStringBuilder>
#include <QStringLiteral>

SongItem::SongItem(mpd_song *song, Item *parent)
    : Item(QIcon(":/icons/audio-x-generic.svg"),
           Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled,
           false,
           false,
           parent)
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
    if (!m_song || column > 0) {
        return QString();
    }
    return songLabel(m_song);
}

QVariant SongItem::tooltip()
{
    return songToolTip(m_song);
}

QString SongItem::uri()
{
    return mpd_song_get_uri(m_song);
}
