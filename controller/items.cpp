#include "items.h"
#include <QtGui/QIcon>

Items::Items(Item *rootItem, QObject *parent)
    : QObject(parent)
    , m_rootItem(rootItem)
{}

Items::~Items()
{
    delete m_rootItem;
}

Item *Items::rootItem() const
{
    return m_rootItem;
}
