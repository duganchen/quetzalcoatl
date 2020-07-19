#ifndef DBITEM_H
#define DBITEM_H

#include "item.h"

class DBItem : public Item
{
public:
    explicit DBItem(QIcon icon, Qt::ItemFlags, QString text, Item *parent = nullptr);
    QString text(int) const override;

private:
    QString m_label;
};

#endif // DBITEM_H
