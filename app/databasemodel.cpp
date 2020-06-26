#include "databasemodel.h"

DatabaseModel::DatabaseModel(Controller *controller, Item *rootItem, QObject *parent)
    : ItemModel(controller, rootItem, parent)
{}

int DatabaseModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}
