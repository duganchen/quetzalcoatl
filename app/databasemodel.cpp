#include "databasemodel.h"

DatabaseModel::DatabaseModel(Item *rootItem, QObject *parent)
    : ItemModel(rootItem, parent)
{}

int DatabaseModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}
