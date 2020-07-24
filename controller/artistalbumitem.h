#ifndef ARTISTALBUMITEM_H
#define ARTISTALBUMITEM_H

#include "controller.h"

class ArtistAlbumItem : public Item
{
public:
    explicit ArtistAlbumItem(QString, QString, Item *parent = nullptr);
    QVector<Item *> fetchMore(Controller *) override;
    QString text(int) const override;

private:
    QString m_artist;
    QString m_album;
};
#endif // ARTISTALBUMITEM_H
