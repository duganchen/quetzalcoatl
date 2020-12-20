#ifndef ORDEREDMPDSONGITEM_H
#define ORDEREDMPDSONGITEM_H

#include "orderedsongitem.h"

class OrderedMPDSongItem : public OrderedSongItem
{
public:
    explicit OrderedMPDSongItem(mpd_song *, Item *parent = nullptr);
    ~OrderedMPDSongItem();

protected:
    const mpd_song *song() const override;

private:
    mpd_song *m_song{};
};

#endif // ORDEREDMPDSONGITEM_H
