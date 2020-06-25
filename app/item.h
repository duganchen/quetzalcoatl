#ifndef ITEM_H
#define ITEM_H

#include <QIcon>
#include <QList>
#include <QVariant>

class Item
{
public:
    explicit Item(QIcon icon, QString label, Item *parentItem = nullptr);
    ~Item();

    void appendChild(Item *child);

    Item *child(int row);
    int childCount() const;
    QIcon icon() const;
    QString label() const;
    int row() const;
    Item *parentItem();

    void setParent(Item *parent);

private:
    QList<Item *> m_childItems;
    QIcon m_icon;
    QString m_label;
    Item *m_parentItem;
};

#endif // ITEM_H
