#include "unorderedmpdsongitem.h"
#include "iconnames.h"

UnorderedMPDSongItem::UnorderedMPDSongItem(std::vector<QString> labels,
                                           mpd_song *song,
                                           QString tooltip)
    : UnorderedSongItem(labels,
                        QIcon::fromTheme(IconNames::Audio),
                        Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled,
                        false,
                        false,
                        tooltip)
    , m_song(song)
{}

UnorderedMPDSongItem::~UnorderedMPDSongItem()
{
    if (m_song) {
        mpd_song_free(m_song);
    }
}

const mpd_song *UnorderedMPDSongItem::song() const
{
    return m_song;
}
