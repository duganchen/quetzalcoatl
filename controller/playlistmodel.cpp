#include "playlistmodel.h"
#include "songitem.h"
#include <QByteArray>
#include <QDebug>

PlaylistModel::PlaylistModel(Controller *controller, QObject *parent)
    : ItemModel(controller, parent)
{
    setRootItem(new Item(QIcon(), Qt::NoItemFlags, true));

    connect(controller, &Controller::queueChanged, this, &PlaylistModel::setQueue);
}

QVariant PlaylistModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QStringLiteral("Name");

        case 1:
            return QStringLiteral("Time");

        default:
            return QVariant();
        }
    }
    return QVariant();
}

int PlaylistModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 2;
}

Qt::ItemFlags PlaylistModel::flags(const QModelIndex &index) const
{
    if (index.isValid()) {
        if (index.column() == 0) {
            return static_cast<SongItem *>(index.internalPointer())->flags();
        }

        return Qt::ItemIsEnabled;
    }

    // The drop target is between rows.
    return Qt::ItemIsDropEnabled;
}

QStringList PlaylistModel::mimeTypes() const
{
    return QStringList{"x-application/vnd.mpd.songids", "x-application/vnd.mpd.uris"};
}

QMimeData *PlaylistModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;
    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    for (auto &index : indexes) {
        if (!index.isValid()) {
            continue;
        }

        stream << static_cast<SongItem *>(index.internalPointer())->id();
    }
    mimeData->setData("x-application/vnd.mpd.songids", encodedData);
    return mimeData;
}

bool PlaylistModel::canDropMimeData(const QMimeData *data,
                                    Qt::DropAction action,
                                    int row,
                                    int column,
                                    const QModelIndex &parent) const
{
    Q_UNUSED(action)
    Q_UNUSED(parent)

    if (row < 0) {
        return false;
    }

    if (!data->hasFormat("x-application/vnd.mpd.songids")
        && !data->hasFormat("x-application/vnd.mpd.uris")) {
        return false;
    }

    if (column > 0) {
        return false;
    }

    return true;
}

bool PlaylistModel::dropMimeData(
    const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(action)
    Q_UNUSED(column)
    Q_UNUSED(parent)

    if (data->hasFormat("x-application/vnd.mpd.songids")) {
        QByteArray encodedData = data->data("x-application/vnd.mpd.songids");
        QDataStream stream(&encodedData, QIODevice::ReadOnly);
        QVector<unsigned> songs;
        unsigned song;
        while (!stream.atEnd()) {
            stream >> song;
            songs.append(song);
        }

        qDebug() << "songs is " << songs;

        emit songsToMove(songs, row);
    }

    return true;
}

void PlaylistModel::setQueue(const QVector<Item *> &queue)
{
    beginResetModel();
    rootItem()->clear();
    for (auto songItem : queue) {
        rootItem()->append(songItem);
    }
    endResetModel();
    emit columnResized(0);
    emit columnResized(1);
}
