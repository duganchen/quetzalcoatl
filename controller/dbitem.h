#ifndef DBITEM_H
#define DBITEM_H

#include "item.h"

class DBItem : public Item
{
public:
    explicit DBItem(QIcon, Qt::ItemFlags, bool, bool, QString, Item *parent = nullptr);
    QString text(int) const override;

private:
    QString m_label;
    bool m_canFetchMore;
};

#endif // DBITEM_H
