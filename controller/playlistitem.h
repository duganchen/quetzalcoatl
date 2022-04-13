#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

#include "item.h"
#include <mpd/client.h>

class PlaylistItem : public Item
{
public:
    explicit PlaylistItem(QString, Item * = nullptr);
    ~PlaylistItem();
    QVector<Item *> fetchMore(Controller *) override;
    QString text(int) const override;

private:
    QString m_path;
};

#endif // PLAYLISTITEM_H
