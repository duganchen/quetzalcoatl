#ifndef COMPILATIONSITEM_H
#define COMPILATIONSITEM_H

#include "item.h"

class CompilationsItem : public Item
{
public:
    CompilationsItem(Item *parent = nullptr);

    QVector<Item *> fetchMore(Controller *) override;
};

#endif // COMPILATIONSITEM_H
