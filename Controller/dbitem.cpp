#include "dbitem.h"

DBItem::DBItem(QIcon icon, Qt::ItemFlags myFlags, QString label, Item *parent)
    : Item(icon, myFlags, parent)
    , m_label(label)
{}

QString DBItem::text(int column) const
{
    Q_UNUSED(column)
    return m_label;
}
