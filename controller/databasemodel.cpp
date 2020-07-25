#include "databasemodel.h"
#include "albumsitem.h"
#include "artistsitem.h"
#include "compilationsitem.h"
#include "composersitem.h"
#include "directoryitem.h"
#include "genresitem.h"
#include "playlistsitem.h"
#include "songsitem.h"

DatabaseModel::DatabaseModel(Controller *controller, QObject *parent)
    : ItemModel(controller, parent)
{
    auto dbRootItem = new Item(QIcon(), Qt::NoItemFlags, true, false);
    dbRootItem->append(new PlaylistsItem());
    dbRootItem->append(new ArtistsItem());
    dbRootItem->append(new AlbumsItem());
    dbRootItem->append(new CompilationsItem());
    dbRootItem->append(new SongsItem());
    dbRootItem->append(new GenresItem());
    dbRootItem->append(new ComposersItem);
    dbRootItem->append(new DirectoryItem(nullptr));

    setRootItem(dbRootItem);
}

int DatabaseModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}
