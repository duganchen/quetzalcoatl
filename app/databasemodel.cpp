#include "databasemodel.h"
#include <QDebug>

DatabaseModel::DatabaseModel(Item *rootItem, QObject *parent)
    : QAbstractItemModel(parent)
    , m_rootItem(rootItem)
{}

DatabaseModel::~DatabaseModel()
{
    delete m_rootItem;
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

    auto item = static_cast<Item *>(index.internalPointer());

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
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return QAbstractItemModel::flags(index);
}

QModelIndex DatabaseModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    auto parentItem = parent.isValid() ? static_cast<Item *>(parent.internalPointer()) : m_rootItem;
    if (!parentItem) {
        return QModelIndex();
    }

    auto childItem = parentItem->at(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    }
    return QModelIndex();
}

QModelIndex DatabaseModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    auto childItem = static_cast<Item *>(index.internalPointer());
    auto parentItem = childItem->parent();

    if (parentItem == m_rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int DatabaseModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0) {
        return 0;
    }

    auto parentItem = parent.isValid() ? static_cast<Item *>(parent.internalPointer()) : m_rootItem;
    if (!parentItem) {
        return 0;
    }
    return parentItem->count();
}
