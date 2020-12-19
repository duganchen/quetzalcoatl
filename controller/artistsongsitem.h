#ifndef ARTISTSONGSITEM_H
#define ARTISTSONGSITEM_H

#include "item.h"

class ArtistSongsItem : public Item
{
public:
    explicit ArtistSongsItem(QString, Item *parent = nullptr);

    QVector<Item *> fetchMore(Controller *) override;

    QString text(int) const override;

private:
    QString m_artist;
};

#endif // ARTISTSONGSITEM_H
