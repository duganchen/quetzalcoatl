#ifndef ABSTRACTSONGITEM_H
#define ABSTRACTSONGITEM_H

#include "item.h"
#include <mpd/client.h>

class AbstractSongItem : public Item
{
public:
    explicit AbstractSongItem(QIcon, Qt::ItemFlags, bool, bool, Item *parent = nullptr);
    QString text(int column) const override;

    QVariant tooltip() override;

    QString uri() override;

protected:
    virtual const mpd_song *song() const = 0;
};

#endif // ABSTRACTSONGITEM_H
