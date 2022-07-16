#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

#include "item.h"
#include <mpd/client.h>

class PlaylistItem : public Item
{
public:
    explicit PlaylistItem(std::vector<QString>);
    ~PlaylistItem();
    QVector<Item *> fetchMore(Controller *) override;
};

#endif // PLAYLISTITEM_H
