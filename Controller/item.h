#ifndef ITEM_H
#define ITEM_H

#include <QVariant>
#include <QVector>
#include <QtGui/QIcon>

class Item
{
public:
    explicit Item(QIcon icon, QString label, Item *parent = nullptr);
    ~Item();

    void append(Item *at);

    Item *at(int row);
    int count() const;
    QIcon icon() const;
    QString label() const;
    int row() const;
    Item *parent();
    void setParent(Item *parent);

private:
    QVector<Item *> m_children;
    QIcon m_icon;
    QString m_label;
    Item *m_parent;
};

#endif // ITEM_H
