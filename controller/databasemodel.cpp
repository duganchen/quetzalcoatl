#include "databasemodel.h"

DatabaseModel::DatabaseModel(Items *items, QObject *parent)
    : ItemModel(items, parent)
{}

int DatabaseModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}
