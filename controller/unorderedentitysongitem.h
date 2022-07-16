#ifndef UNORDEREDENTITYSONGITEM_H
#define UNORDEREDENTITYSONGITEM_H

#include "unorderedsongitem.h"
#include <mpd/client.h>

class UnorderedEntitySongItem : public UnorderedSongItem
{
public:
    explicit UnorderedEntitySongItem(std::vector<QString>, mpd_entity *);
    ~UnorderedEntitySongItem();

protected:
    const mpd_song *song() const override;

private:
    mpd_entity *m_entity{};
};

#endif // UNORDEREDENTITYSONGITEM_H
