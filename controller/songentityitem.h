#ifndef SONGENTITYITEM_H
#define SONGENTITYITEM_H

#include "item.h"
#include <mpd/client.h>

class SongEntityItem : public Item
{
public:
    SongEntityItem(mpd_entity *, Item *parent = nullptr);
    ~SongEntityItem();

    QString text(int column) const override;

    QVariant tooltip() override;

    QString uri() override;

    void onDoubleClicked(Controller *) override;

protected:
    const mpd_song *song();

private:
    mpd_entity *m_entity;
};

#endif // SONGENTITYITEM_H
