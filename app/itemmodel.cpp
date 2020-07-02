#include "itemmodel.h"
#include <QDebug>

ItemModel::ItemModel(ItemModelController *itemModelController, QObject *parent)
    : QAbstractItemModel(parent)
    , m_itemModelController(itemModelController)
{
    m_itemModelController->setParent(this);

    connect(m_itemModelController,
            &ItemModelController::rowsAboutToBeInserted,
            this,
            &ItemModel::beginInsertRows);
    connect(m_itemModelController,
            &ItemModelController::rowsInserted,
            this,
            &ItemModel::endInsertRows);
    connect(m_itemModelController,
            &ItemModelController::rowsAboutToBeMoved,
            this,
            &ItemModel::beginMoveRows);
    connect(m_itemModelController, &ItemModelController::rowsMoved, this, &ItemModel::endMoveRows);
    connect(m_itemModelController,
            &ItemModelController::rowsAboutToBeRemoved,
            this,
            &ItemModel::beginRemoveRows);
    connect(m_itemModelController,
            &ItemModelController::rowsRemoved,
            this,
            &ItemModel::endRemoveRows);
    connect(m_itemModelController,
            &ItemModelController::modelAboutToBeReset,
            this,
            &ItemModel::beginResetModel);
    connect(m_itemModelController,
            &ItemModelController::modelReset,
            this,
            &ItemModel::endResetModel);
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

    auto parentItem = parent.isValid() ? static_cast<AbstractItem *>(parent.internalPointer())
                                       : m_itemModelController->rootItem();
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

    if (m_itemModelController->rootItem() == parentItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int ItemModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0) {
        return 0;
    }

    auto parentItem = parent.isValid() ? static_cast<AbstractItem *>(parent.internalPointer())
                                       : m_itemModelController->rootItem();
    if (!parentItem) {
        return 0;
    }
    return parentItem->count();
}
