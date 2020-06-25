#include "item.h"

Item::Item(QIcon icon, QString label, Item *parent)
    : m_icon(icon)
    , m_label(label)
    , m_parentItem(nullptr)
{}

Item::~Item()
{
    qDeleteAll(m_childItems);
}

void Item::appendChild(Item *item)
{
    item->setParent(this);
    m_childItems.append(item);
}

Item *Item::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int Item::childCount() const
{
    return m_childItems.count();
}

QIcon Item::icon() const
{
    return m_icon;
}

QString Item::label() const
{
    return m_label;
}

Item *Item::parentItem()
{
    return m_parentItem;
}

void Item::setParent(Item *parent)
{
    m_parentItem = parent;
}

int Item::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<Item *>(this));

    return 0;
}
