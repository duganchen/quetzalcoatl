#ifndef COMPILATIONSITEM_H
#define COMPILATIONSITEM_H

#include "controller.h"

class CompilationsItem : public Item
{
public:
    explicit CompilationsItem(Item *parent = nullptr);

    QString text(int) const override;
    QVector<Item *> fetchMore(Controller *) override;
};

#endif // COMPILATIONSITEM_H
