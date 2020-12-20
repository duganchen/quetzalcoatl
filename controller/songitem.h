#ifndef SONGITEM_H
#define SONGITEM_H

#include "item.h"
#include <mpd/client.h>

class SongItem : public Item
{
public:
    SongItem(QIcon, Qt::ItemFlags, bool, bool, Item *parent = nullptr);
    QString text(int column) const override;

    QVariant tooltip() override;

    QString uri() override;

protected:
    virtual const mpd_song *song() const = 0;
};

#endif // SONGITEM_H
