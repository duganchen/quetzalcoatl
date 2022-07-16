#ifndef UNORDEREDMPDSONGITEM_H
#define UNORDEREDMPDSONGITEM_H

#include "unorderedsongitem.h"

class UnorderedMPDSongItem : public UnorderedSongItem
{
public:
    explicit UnorderedMPDSongItem(std::vector<QString> labels, mpd_song *);
    ~UnorderedMPDSongItem();

protected:
    const mpd_song *song() const override;

private:
    mpd_song *m_song{};
};

#endif // UNORDEREDMPDSONGITEM_H
