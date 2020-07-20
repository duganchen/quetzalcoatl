#include "dbitem.h"
#include <QDebug>

DBItem::DBItem(QIcon icon,
               Qt::ItemFlags myFlags,
               bool hazChildren,
               bool couldFetchMore,
               QString label,
               Item *parent)
    : Item(icon, myFlags, hazChildren, couldFetchMore, parent)
    , m_label(label)
    , m_canFetchMore(true)
{}

QString DBItem::text(int column) const
{
    Q_UNUSED(column)
    return m_label;
}
