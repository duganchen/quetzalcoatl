#ifndef QUEUEDITEM_H
#define QUEUEDITEM_H

#include "songentityitem.h"
#include <mpd/client.h>

class QueuedItem : public Item
{
public:
    QueuedItem(mpd_entity *, Item *parent = nullptr);
    ~QueuedItem();

    QString text(int column) const override;

    QVariant tooltip() override;

    unsigned id() override;

    void onDoubleClicked(Controller *) override;

private:
    mpd_entity *m_entity;
};

#endif // QUEUEDITEM_H
