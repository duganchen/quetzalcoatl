#ifndef QUEUEDITEM_H
#define QUEUEDITEM_H

#include "item.h"
#include <mpd/client.h>

class QueuedItem : public Item
{
public:
    QueuedItem(QIcon, Qt::ItemFlags, bool, mpd_entity *, Item *parent = nullptr);
    ~QueuedItem();

    QString text(int column) const override;

    QVariant tooltip() override;

    unsigned id();

private:
    mpd_entity *m_entity;
};

#endif // QUEUEDITEM_H
