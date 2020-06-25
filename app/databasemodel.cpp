#include "databasemodel.h"
#include <QDebug>

DatabaseModel::DatabaseModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    m_rootItem = new Item(QIcon(), "");
    m_rootItem->append(new Item(QIcon(":/icons/folder-favorites.svg"), "Playlists"));
    auto artistsItem = new Item(QIcon(":/icons/server-database.svg"), "Artists");
    artistsItem->append(new Item(QIcon(":/icons/server-database.svg"), "Johnny Cash"));
    m_rootItem->append(artistsItem);
    m_rootItem->append(new Item(QIcon(":/icons/server-database.svg"), "Albums"));
    m_rootItem->append(new Item(QIcon(":/icons/server-database.svg"), "Compilations"));
    m_rootItem->append(new Item(QIcon(":/icons/server-database.svg"), "Songs"));
    m_rootItem->append(new Item(QIcon(":/icons/server-database.svg"), "Genres"));
    m_rootItem->append(new Item(QIcon(":/icons/server-database.svg"), "Composers"));
    m_rootItem->append(new Item(QIcon(":/icons/drive-harddisk"), "/"));
}

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
    return parentItem->count();
}
