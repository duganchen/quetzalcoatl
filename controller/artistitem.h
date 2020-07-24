#ifndef ARTISTITEM_H
#define ARTISTITEM_H

#include "controller.h"

class ArtistItem : public Item
{
public:
    explicit ArtistItem(QString, Item *parent = nullptr);

    QVector<Item *> fetchMore(Controller *) override;

    QString text(int) const override;

private:
    QString m_artist;
};

#endif // ARTISTITEM_H
