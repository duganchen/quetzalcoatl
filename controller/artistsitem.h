#ifndef ARTISTSITEM_H
#define ARTISTSITEM_H

#include "controller.h"

class ArtistsItem : public Item
{
public:
    explicit ArtistsItem(Item *parent = nullptr);

    QString text(int) const override;
    QVector<Item *> fetchMore(Controller *) override;
};

#endif // ARTISTSITEM_H
