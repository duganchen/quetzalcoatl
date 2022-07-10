#ifndef ORDEREDENTITYSONGITEM_H
#define ORDEREDENTITYSONGITEM_H

#include "orderedsongitem.h"
#include <mpd/client.h>

class OrderedEntitySongItem : public OrderedSongItem
{
public:
    explicit OrderedEntitySongItem(const std::vector<QString> &labels,
                                   mpd_entity *,
                                   Item *parent = nullptr);
    ~OrderedEntitySongItem();

protected:
    const mpd_song *song() const override;

private:
    mpd_entity *m_entity{};
};

#endif // ORDEREDENTITYSONGITEM_H
