#include "itemmodel.h"

#include "item.h"

ItemModel::ItemModel(Controller *myController, QObject *parent)
    : QAbstractItemModel(parent)
    , m_controller(myController)
{}

ItemModel::~ItemModel()
{
    delete m_rootItem;
}

bool ItemModel::canFetchMore(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return false;
    }
    return static_cast<Item *>(parent.internalPointer())->canFetchMore();
}

void ItemModel::fetchMore(const QModelIndex &parent)
{
    if (!parent.isValid()) {
        return;
    }
    auto items = static_cast<Item *>(parent.internalPointer())->fetchMore(m_controller);
    auto parentItem = static_cast<Item *>(parent.internalPointer());
    beginInsertRows(parent, 0, items.count() - 1);
    for (auto item : items) {
        parentItem->append(item);
    }
    endInsertRows();
    parentItem->setCanFetchMore(false);
}

bool ItemModel::hasChildren(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return true;
    }

    return static_cast<Item *>(parent.internalPointer())->hasChildren();
}

QVariant ItemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    auto item = static_cast<Item *>(index.internalPointer());

    if (Qt::DisplayRole == role) {
        return item->text(index.column());
    }

    if (Qt::DecorationRole == role && index.column() == 0) {
        return item->icon();
    }

    if (Qt::ToolTipRole == role) {
        return item->tooltip();
    }

    return QVariant();
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

    if (m_rootItem == parentItem)
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

void ItemModel::setRootItem(Item *rootItem)
{
    m_rootItem = rootItem;
}

Item *ItemModel::rootItem() const
{
    return m_rootItem;
}

Controller *ItemModel::controller() const
{
    return m_controller;
}

void ItemModel::onDoubleClicked(const QModelIndex &index)
{
    static_cast<Item *>(index.internalPointer())->onDoubleClicked(m_controller);
}
