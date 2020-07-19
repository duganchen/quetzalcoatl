#ifndef SONGITEM_H
#define SONGITEM_H

#include "item.h"
#include <mpd/client.h>

class SongItem : public Item
{
public:
    SongItem(QIcon, Qt::ItemFlags, bool, mpd_entity *, Item *parent = nullptr);
    ~SongItem();

    QString text(int column) const override;

    QVariant tooltip() override;

    unsigned id();

private:
    mpd_entity *m_entity;
};

#endif // SONGITEM_H
