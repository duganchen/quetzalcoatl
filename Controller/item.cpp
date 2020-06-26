#include "item.h"

Item::Item(QIcon icon, QString label, Item *parent)
    : m_icon(icon)
    , m_label(label)
    , m_parent(parent)
{}

Item::~Item()
{
    qDeleteAll(m_children);
}

void Item::append(Item *item)
{
    item->setParent(this);
    m_children.append(item);
}

Item *Item::at(int row)
{
    if (row < 0 || row >= m_children.size()) {
        return nullptr;
    }
    return m_children.at(row);
}

int Item::count() const
{
    return m_children.count();
}

QIcon Item::icon() const
{
    return m_icon;
}

QString Item::label() const
{
    return m_label;
}

Item *Item::parent()
{
    return m_parent;
}

void Item::setParent(Item *parent)
{
    m_parent = parent;
}

int Item::row() const
{
    if (m_parent)
        return m_parent->m_children.indexOf(const_cast<Item *>(this));

    return 0;
}
