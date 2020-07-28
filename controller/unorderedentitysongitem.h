#ifndef UNORDEREDENTITYSONGITEM_H
#define UNORDEREDENTITYSONGITEM_H

#include "unorderedsongitem.h"
#include <mpd/client.h>

class UnorderedEntitySongItem : public UnorderedSongItem
{
public:
    UnorderedEntitySongItem(mpd_entity *, Item *parent = nullptr);
    ~UnorderedEntitySongItem();

protected:
    const mpd_song *song() const override;

private:
    mpd_entity *m_entity;
};

#endif // UNORDEREDENTITYSONGITEM_H
