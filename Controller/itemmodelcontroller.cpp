#include "itemmodelcontroller.h"
#include <QtGui/QIcon>

ItemModelController::ItemModelController(AbstractItem *rootItem, QObject *parent)
    : QObject(parent)
    , m_rootItem(rootItem)
{}

ItemModelController::~ItemModelController()
{
    delete m_rootItem;
}

AbstractItem *ItemModelController::rootItem() const
{
    return m_rootItem;
}
