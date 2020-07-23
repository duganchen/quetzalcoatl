#include "playlistmodel.h"
#include "queueditem.h"
#include <QByteArray>
#include <QDebug>
#include <QFont>

PlaylistModel::PlaylistModel(Controller *controller, QObject *parent)
    : ItemModel(controller, parent)
    , m_songId(-1)
{
    setRootItem(new Item(QIcon(), Qt::NoItemFlags, true, false));

    connect(controller, &Controller::queueChanged, this, &PlaylistModel::setQueue);
    connect(controller, &Controller::songId, this, &PlaylistModel::setSongId);
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
            return static_cast<QueuedItem *>(index.internalPointer())->flags();
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
        stream << static_cast<unsigned>(index.row());
        auto songItem = static_cast<QueuedItem *>(index.internalPointer());
        stream << songItem->id();
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

    if (!controller()) {
        return false;
    }

    // source row, song id
    QVector<QPair<unsigned, unsigned>> sources;

    if (data->hasFormat("x-application/vnd.mpd.songids")) {
        QByteArray encodedData = data->data("x-application/vnd.mpd.songids");
        QDataStream stream(&encodedData, QIODevice::ReadOnly);
        QVector<unsigned> songs;

        while (!stream.atEnd()) {
            QPair<int, unsigned> source;
            stream >> source.first;
            stream >> source.second;
            sources.append(source);
        }

        controller()->moveSongs(sources, row);
    }

    return true;
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    // qDebug() << "m_songId is " << m_songId;
    if (Qt::FontRole == role) {
        if (m_songId != -1) {
            unsigned playingId = static_cast<unsigned>(m_songId);
            auto rowId = static_cast<QueuedItem *>(index.internalPointer())->id();
            if (rowId == playingId) {
                QFont font;
                font.setBold(true);
                return font;
            }
        }
    }

    return ItemModel::data(index, role);
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

void PlaylistModel::setSongId(int songId)
{
    // Can this be optimized by being changed to a dataChanged() signal? Yes.
    // This is an optimization worth pursuing later.
    beginResetModel();
    m_songId = songId;
    endResetModel();
}
