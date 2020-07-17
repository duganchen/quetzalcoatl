#ifndef SONGITEM_H
#define SONGITEM_H

#include "abstractitem.h"
#include <mpd/client.h>

class SongItem : public AbstractItem
{
public:
    SongItem(QIcon, Qt::ItemFlags, mpd_entity *, AbstractItem *parent = nullptr);
    ~SongItem();

    QString text(int column) const override;

    QVariant tooltip() override;

    unsigned id();

private:
    mpd_entity *m_entity;
};

#endif // SONGITEM_H
