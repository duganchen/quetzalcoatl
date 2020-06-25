#include "itemmodel.h"
#include <QDebug>

ItemModel::ItemModel(Item *rootItem, QObject *parent)
    : QAbstractItemModel(parent)
    , m_rootItem(rootItem)
{}

ItemModel::~ItemModel()
{
    delete m_rootItem;
}

bool ItemModel::canFetchMore(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        auto item = static_cast<Item *>(parent.internalPointer());
        qDebug() << item->label();
    }
    return false;
}

QVariant ItemModel::data(const QModelIndex &index, int role) const
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

Qt::ItemFlags ItemModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return QAbstractItemModel::flags(index);
}

QModelIndex ItemModel::index(int row, int column, const QModelIndex &parent) const
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

QModelIndex ItemModel::parent(const QModelIndex &index) const
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

int ItemModel::rowCount(const QModelIndex &parent) const
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
