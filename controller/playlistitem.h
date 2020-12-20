#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

#include "item.h"
#include <mpd/client.h>

class PlaylistItem : public Item
{
public:
    explicit PlaylistItem(mpd_playlist *, Item *parent = nullptr);
    ~PlaylistItem();
    QVector<Item *> fetchMore(Controller *) override;
    QString text(int) const override;

private:
    mpd_playlist *m_playlist{};
};

#endif // PLAYLISTITEM_H
