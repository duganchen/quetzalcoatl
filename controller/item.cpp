#include "item.h"
#include <QDebug>

Item::Item(QIcon icon, Qt::ItemFlags myFlags, bool hazChildren, bool couldFetchMore, Item *parent)
    : m_icon(icon)
    , m_parent(parent)
    , m_flags(myFlags)
    , m_hasChildren(hazChildren)
    , m_canFetchMore(couldFetchMore)
{}

Item::~Item()
{
    clear();
}

void Item::clear()
{
    qDeleteAll(m_children);
    m_children.clear();
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

QString Item::text(int column) const
{
    Q_UNUSED(column)
    return QString();
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
    if (m_parent) {
        return m_parent->m_children.indexOf(const_cast<Item *>(this));
    }

    return 0;
}

QVariant Item::tooltip()
{
    return QVariant();
}

Qt::ItemFlags Item::flags() const
{
    return m_flags;
}

bool Item::canFetchMore()
{
    return m_canFetchMore;
}

void Item::setCanFetchMore(bool value)
{
    m_canFetchMore = value;
}

QVector<Item *> Item::fetchMore(Controller *controller)
{
    // This is a stub. It doesn't get called.
    Q_UNUSED(controller)
    QVector<Item *> items;
    return items;
}

bool Item::hasChildren()
{
    return m_hasChildren;
}
