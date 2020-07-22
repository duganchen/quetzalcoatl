#ifndef SONGITEM_H
#define SONGITEM_H

#include "item.h"
#include <mpd/client.h>

class SongItem : public Item
{
public:
    SongItem(QIcon, Qt::ItemFlags, bool, mpd_song *, Item *parent = nullptr);
    ~SongItem();

    QString text(int column) const override;

    QVariant tooltip() override;

private:
    mpd_song *m_song;
};

#endif // SONGITEM_H
