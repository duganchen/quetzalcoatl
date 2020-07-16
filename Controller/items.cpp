#include "items.h"
#include <QtGui/QIcon>

Items::Items(AbstractItem *rootItem, QObject *parent)
    : QObject(parent)
    , m_rootItem(rootItem)
{}

Items::~Items()
{
    delete m_rootItem;
}

AbstractItem *Items::rootItem() const
{
    return m_rootItem;
}
