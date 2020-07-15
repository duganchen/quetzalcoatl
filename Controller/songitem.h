#ifndef SONGITEM_H
#define SONGITEM_H

#include "abstractitem.h"
#include <mpd/client.h>

class SongItem : public AbstractItem
{
public:
    SongItem(QIcon, mpd_entity *, AbstractItem *parent = nullptr);
    ~SongItem();

    QString text(int column) const override;

private:
    mpd_entity *m_entity;
};

#endif // SONGITEM_H
