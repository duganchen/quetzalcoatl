#ifndef SONGITEM_H
#define SONGITEM_H

#include "item.h"
#include <mpd/client.h>

class SongItem : public Item
{
public:
    SongItem(const std::vector<QString> &, QIcon, Qt::ItemFlags, bool, bool, Item *parent = nullptr);
    QVariant tooltip() override;

    QString uri() override;

protected:
    virtual const mpd_song *song() const = 0;
};

#endif // SONGITEM_H
