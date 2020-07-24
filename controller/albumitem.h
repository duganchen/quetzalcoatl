#ifndef ALBUMITEM_H
#define ALBUMITEM_H

#include "item.h"

class AlbumItem : public Item
{
public:
    explicit AlbumItem(QString, Item *parent = nullptr);
    QVector<Item *> fetchMore(Controller *) override;
    QString text(int) const override;

private:
    QString m_album;
};
#endif // ALBUMITEM_H
