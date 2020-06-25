#include "databasemodel.h"
#include <QDebug>

DatabaseModel::DatabaseModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new Item(QIcon(), "");
    rootItem->appendChild(new Item(QIcon(":/icons/folder-favorites.svg"), "Playlists"));
    auto artistsItem = new Item(QIcon(":/icons/server-database.svg"), "Artists");
    artistsItem->appendChild(new Item(QIcon(":/icons/server-database.svg"), "Johnny Cash"));
    rootItem->appendChild(artistsItem);
    rootItem->appendChild(new Item(QIcon(":/icons/server-database.svg"), "Albums"));
    rootItem->appendChild(new Item(QIcon(":/icons/server-database.svg"), "Compilations"));
    rootItem->appendChild(new Item(QIcon(":/icons/server-database.svg"), "Songs"));
    rootItem->appendChild(new Item(QIcon(":/icons/server-database.svg"), "Genres"));
    rootItem->appendChild(new Item(QIcon(":/icons/server-database.svg"), "Composers"));
    rootItem->appendChild(new Item(QIcon(":/icons/drive-harddisk"), "/"));
}

DatabaseModel::~DatabaseModel()
{
    delete rootItem;
}

int DatabaseModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

bool DatabaseModel::canFetchMore(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        auto item = static_cast<Item *>(parent.internalPointer());
        qDebug() << item->label();
    }
    return false;
}

QVariant DatabaseModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    Item *item = static_cast<Item *>(index.internalPointer());

    if (Qt::DisplayRole == role) {
        return item->label();
    }

    if (Qt::DecorationRole == role) {
        return item->icon();
    }

    return QVariant();
}

Qt::ItemFlags DatabaseModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QModelIndex DatabaseModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    Item *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<Item *>(parent.internalPointer());

    Item *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex DatabaseModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    Item *childItem = static_cast<Item *>(index.internalPointer());
    Item *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int DatabaseModel::rowCount(const QModelIndex &parent) const
{
    Item *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<Item *>(parent.internalPointer());

    return parentItem->childCount();
}
