#include "abstractitem.h"

AbstractItem::AbstractItem(QIcon icon, AbstractItem *parent)
    : m_icon(icon)
    , m_parent(parent)
{}

AbstractItem::~AbstractItem()
{
    qDeleteAll(m_children);
}

void AbstractItem::append(AbstractItem *item)
{
    item->setParent(this);
    m_children.append(item);
}

AbstractItem *AbstractItem::at(int row)
{
    if (row < 0 || row >= m_children.size()) {
        return nullptr;
    }
    return m_children.at(row);
}

int AbstractItem::count() const
{
    return m_children.count();
}

QIcon AbstractItem::icon() const
{
    return m_icon;
}

AbstractItem *AbstractItem::parent()
{
    return m_parent;
}

void AbstractItem::setParent(AbstractItem *parent)
{
    m_parent = parent;
}

int AbstractItem::row() const
{
    if (m_parent) {
        return m_parent->m_children.indexOf(const_cast<AbstractItem *>(this));
    }

    return 0;
}
