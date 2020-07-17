#include "playlistmodel.h"
#include "songitem.h"

PlaylistModel::PlaylistModel(Items *items, QObject *parent)
    : ItemModel(items, parent)
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

Qt::ItemFlags PlaylistModel::flags(const QModelIndex &index) const
{
    if (index.isValid()) {
        if (index.column() == 0) {
            return static_cast<SongItem *>(index.internalPointer())->flags();
        }

        return Qt::ItemIsEnabled;
    }

    // The drop target is between rows.
    return Qt::ItemIsDropEnabled;
}
