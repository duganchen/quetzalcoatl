#ifndef ARTISTITEM_H
#define ARTISTITEM_H

#include "item.h"

class ArtistItem : public Item
{
public:
    explicit ArtistItem(const std::vector<QString> &labels);

    QVector<Item *> fetchMore(Controller *) override;
};

#endif // ARTISTITEM_H
