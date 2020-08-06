#include "orderedmpdsongitem.h"

OrderedMPDSongItem ::OrderedMPDSongItem(mpd_song *song, Item *parent)
    : OrderedSongItem(QIcon::fromTheme(IconNames::Audio),
                      Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled,
                      false,
                      false,
                      parent)
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
