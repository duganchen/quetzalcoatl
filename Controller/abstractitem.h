#ifndef ABSTRACTITEM_H
#define ABSTRACTITEM_H

#include <QVariant>
#include <QVector>
#include <QtGui/QIcon>

class AbstractItem
{
public:
    explicit AbstractItem(QIcon icon, AbstractItem *parent = nullptr);
    virtual ~AbstractItem();

    void append(AbstractItem *at);

    AbstractItem *at(int row);
    int count() const;
    QIcon icon() const;
    virtual QString text(int) const = 0;
    int row() const;
    AbstractItem *parent();
    void setParent(AbstractItem *parent);

private:
    QVector<AbstractItem *> m_children;
    QIcon m_icon;
    AbstractItem *m_parent;
};

#endif // ABSTRACTITEM_H
