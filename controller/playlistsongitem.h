#ifndef PLAYLISTSONGITEM_H
#define PLAYLISTSONGITEM_H

#include "item.h"
#include <mpd/client.h>

class PlaylistSongItem : public Item
{
public:
    PlaylistSongItem(mpd_entity *, Item *parent = nullptr);
    ~PlaylistSongItem();

    QString text(int column) const override;

    QVariant tooltip() override;

private:
    mpd_entity *m_entity;
};

#endif // PLAYLISTSONGITEM_H
