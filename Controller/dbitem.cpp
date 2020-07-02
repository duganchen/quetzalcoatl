#include "dbitem.h"

DBItem::DBItem(QIcon icon, QString label, AbstractItem *parent)
    : AbstractItem(icon, parent)
    , m_label(label)
{}

QString DBItem::text(int column) const
{
    Q_UNUSED(column)
    return m_label;
}
