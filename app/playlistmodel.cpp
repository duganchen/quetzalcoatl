#include "playlistmodel.h"

PlaylistModel::PlaylistModel(ItemModelController *itemModelController, QObject *parent)
    : ItemModel(itemModelController, parent)
{}

QVariant PlaylistModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return "Name";

        case 1:
            return "Time";

        default:
            return QVariant();
        }
    }
    return QVariant();
}

int PlaylistModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}
