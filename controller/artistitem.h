#ifndef ARTISTITEM_H
#define ARTISTITEM_H

#include "item.h"

class ArtistItem : public Item
{
public:
    explicit ArtistItem(const std::vector<QString> &labels, QString, Item *parent = nullptr);

    QVector<Item *> fetchMore(Controller *) override;

private:
    QString m_artist;
};

#endif // ARTISTITEM_H
