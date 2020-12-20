#ifndef COMPOSERSITEM_H
#define COMPOSERSITEM_H

#include "item.h"

class ComposersItem : public Item
{
public:
    ComposersItem(Item *parent = nullptr);

    QString text(int) const override;
    QVector<Item *> fetchMore(Controller *) override;
};

#endif // COMPOSERSITEM_H
