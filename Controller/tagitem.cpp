#include "tagitem.h"
#include <QDebug>

TagItem::TagItem(QString tag)
    : m_tag(tag)
{}

QString TagItem::data()
{
    return m_tag;
}
