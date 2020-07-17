#ifndef DBITEM_H
#define DBITEM_H

#include "abstractitem.h"

class DBItem : public AbstractItem
{
public:
    explicit DBItem(QIcon icon, Qt::ItemFlags, QString text, AbstractItem *parent = nullptr);
    QString text(int) const override;

private:
    QString m_label;
};

#endif // DBITEM_H
