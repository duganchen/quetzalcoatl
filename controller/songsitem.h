#ifndef SONGSITEM_H
#define SONGSITEM_H

#include "controller.h"
#include "item.h"

class SongsItem : public Item
{
public:
    explicit SongsItem(Item *parent = nullptr);

    QString text(int) const override;
    QVector<Item *> fetchMore(Controller *) override;
};

#endif // SONGSITEM_H
