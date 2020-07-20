#include "dbitem.h"
#include <QDebug>

DBItem::DBItem(QIcon icon, Qt::ItemFlags myFlags, bool hazChildren, QString label, Item *parent)
    : Item(icon, myFlags, hazChildren, parent)
    , m_label(label)
    , m_canFetchMore(true)
{}

QString DBItem::text(int column) const
{
    Q_UNUSED(column)
    return m_label;
}

bool DBItem::canFetchMore()
{
    return m_canFetchMore;
}
