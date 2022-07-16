#ifndef ARTISTSONGSITEM_H
#define ARTISTSONGSITEM_H

#include "item.h"

class ArtistSongsItem : public Item
{
public:
    explicit ArtistSongsItem(QString);

    QVector<Item *> fetchMore(Controller *) override;

private:
    QString m_artist;
};

#endif // ARTISTSONGSITEM_H
