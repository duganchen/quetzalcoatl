#include "orderedmpdsongitem.h"
#include "iconnames.h"

OrderedMPDSongItem ::OrderedMPDSongItem(std::vector<QString> labels, mpd_song *song, QString tooltip)
    : OrderedSongItem(labels,
                      QIcon::fromTheme(IconNames::Audio),
                      Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled,
                      false,
                      false,
                      tooltip)
    , m_song(song)
{}

OrderedMPDSongItem::~OrderedMPDSongItem()
{
    if (m_song) {
        mpd_song_free(m_song);
    }
}

const mpd_song *OrderedMPDSongItem::song() const
{
    return m_song;
}
