#ifndef ARTISTALBUMITEM_H
#define ARTISTALBUMITEM_H

#include "item.h"

class ArtistAlbumItem : public Item
{
public:
    ArtistAlbumItem(const std::vector<QString> &labels, QString, Item *parent = nullptr);
    QVector<Item *> fetchMore(Controller *) override;

private:
    QString m_artist;
    QString m_album;
};
#endif // ARTISTALBUMITEM_H
