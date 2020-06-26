#include <QtGui/QIcon>
#include "itemmodelcontroller.h"

ItemModelController::ItemModelController(QObject *parent)
    : QObject(parent)
{
    m_rootItem = new Item(QIcon(), "");
}

ItemModelController::~ItemModelController()
{
    delete m_rootItem;
}

Item * ItemModelController::rootItem() const
{
    return m_rootItem;
}

