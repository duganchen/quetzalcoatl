#ifndef ALBUMITEM_H
#define ALBUMITEM_H

#include "item.h"

class AlbumItem : public Item
{
public:
    explicit AlbumItem(const std::vector<QString> &labels, QString, Item *parent = nullptr);
    QVector<Item *> fetchMore(Controller *) override;

private:
    QString m_album;
};
#endif // ALBUMITEM_H
