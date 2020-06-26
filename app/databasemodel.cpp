#include "databasemodel.h"

DatabaseModel::DatabaseModel(ItemModelController *itemModelController, QObject *parent)
    : ItemModel(itemModelController, parent)
{}

int DatabaseModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}
