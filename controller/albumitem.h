#ifndef ALBUMITEM_H
#define ALBUMITEM_H

#include "item.h"

class AlbumItem : public Item
{
public:
    explicit AlbumItem(const std::vector<QString> &labels, Item *parent = nullptr);
    QVector<Item *> fetchMore(Controller *) override;
};
#endif // ALBUMITEM_H
