#include "itemmodel.h"
#include <QDebug>

ItemModel::ItemModel(Items *items, QObject *parent)
    : QAbstractItemModel(parent)
    , m_items(items)
{
    m_items->setParent(this);

    connect(m_items, &Items::modelAboutToBeReset, this, &ItemModel::beginResetModel);
    connect(m_items, &Items::modelReset, this, [=]() {
        endResetModel();
        emit columnResized(0);
        emit columnResized(1);
    });
}

bool ItemModel::canFetchMore(const QModelIndex &parent) const
{
    return false;
}

QVariant ItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    auto item = static_cast<AbstractItem *>(index.internalPointer());

    if (Qt::DisplayRole == role) {
        return item->text(index.column());
    }

    if (Qt::DecorationRole == role && index.column() == 0) {
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

    auto parentItem = parent.isValid() ? static_cast<AbstractItem *>(parent.internalPointer())
                                       : m_items->rootItem();
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

    auto childItem = static_cast<AbstractItem *>(index.internalPointer());
    auto parentItem = childItem->parent();

    if (m_items->rootItem() == parentItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int ItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0) {
        return 0;
    }

    auto parentItem = parent.isValid() ? static_cast<AbstractItem *>(parent.internalPointer())
                                       : m_items->rootItem();
    if (!parentItem) {
        return 0;
    }
    return parentItem->count();
}
