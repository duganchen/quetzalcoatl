#include "databasemodel.h"
#include "albumsitem.h"
#include "artistsitem.h"
#include "compilationsitem.h"
#include "composersitem.h"
#include "directoryitem.h"
#include "genresitem.h"
#include "playlistsitem.h"
#include "songsitem.h"
#include <QMimeData>

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

QStringList DatabaseModel::mimeTypes() const
{
    return {"x-application/vnd.mpd.uri"};
}

QMimeData *DatabaseModel::mimeData(const QModelIndexList &indexes) const
{
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    for (auto index : indexes) {
        stream << static_cast<Item *>(index.internalPointer())->uri();
    }

    QMimeData *mimeData = new QMimeData();
    mimeData->setData("x-application/vnd.mpd.uris", encodedData);
    return mimeData;
}

Qt::ItemFlags DatabaseModel::flags(const QModelIndex &index) const
{
    if (index.isValid()) {
        return static_cast<Item *>(index.internalPointer())->flags();
    }
    return Qt::NoItemFlags;
}
