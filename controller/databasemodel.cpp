#include "databasemodel.h"
#include "genresitem.h"

DatabaseModel::DatabaseModel(Controller *controller, QObject *parent)
    : ItemModel(controller, parent)
{
    auto dbRootItem = new Item(QIcon(), Qt::NoItemFlags, true);
    dbRootItem->append(
        new DBItem(QIcon(":/icons/folder-favorites.svg"), Qt::ItemIsEnabled, true, "Playlists"));
    dbRootItem->append(
        new DBItem(QIcon(":/icons/server-database.svg"), Qt::ItemIsEnabled, true, "Albums"));
    dbRootItem->append(
        new DBItem(QIcon(":/icons/server-database.svg"), Qt::ItemIsEnabled, true, "Compilations"));
    dbRootItem->append(
        new DBItem(QIcon(":/icons/server-database.svg"), Qt::ItemIsEnabled, true, "Songs"));
    dbRootItem->append(
        new GenresItem(QIcon(":/icons/server-database.svg"), Qt::ItemIsEnabled, true, "Genres"));
    dbRootItem->append(
        new DBItem(QIcon(":/icons/server-database.svg"), Qt::ItemIsEnabled, true, "Composers"));
    dbRootItem->append(new DBItem(QIcon(":/icons/drive-harddisk"), Qt::ItemIsEnabled, true, "/"));

    setRootItem(dbRootItem);
}

int DatabaseModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}
