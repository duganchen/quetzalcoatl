#include "databasemodel.h"
#include "albumsitem.h"
#include "dbitem.h"
#include "genresitem.h"

DatabaseModel::DatabaseModel(Controller *controller, QObject *parent)
    : ItemModel(controller, parent)
{
    auto dbRootItem = new Item(QIcon(), Qt::NoItemFlags, true, false);
    dbRootItem->append(new DBItem(QIcon(":/icons/folder-favorites.svg"),
                                  Qt::ItemIsEnabled,
                                  true,
                                  true,
                                  QStringLiteral("Playlists")));
    dbRootItem->append(new AlbumsItem());
    dbRootItem->append(new DBItem(QIcon(":/icons/server-database.svg"),
                                  Qt::ItemIsEnabled,
                                  true,
                                  true,
                                  QStringLiteral("Compilations")));
    dbRootItem->append(new DBItem(QIcon(":/icons/server-database.svg"),
                                  Qt::ItemIsEnabled,
                                  true,
                                  true,
                                  QStringLiteral("Songs")));
    dbRootItem->append(new GenresItem());
    dbRootItem->append(
        new DBItem(QIcon(":/icons/server-database.svg"), Qt::ItemIsEnabled, true, true, "Composers"));
    dbRootItem->append(new DBItem(QIcon(":/icons/drive-harddisk"),
                                  Qt::ItemIsEnabled,
                                  true,
                                  true,
                                  QStringLiteral("/")));

    setRootItem(dbRootItem);
}

int DatabaseModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}
