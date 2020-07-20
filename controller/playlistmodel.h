#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include "itemmodel.h"
#include <QMimeData>

class PlaylistModel : public ItemModel
{
    Q_OBJECT

public:
    explicit PlaylistModel(Controller *, QObject *parent = nullptr);

    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex &) const override;

    QStringList mimeTypes() const override;

    QMimeData *mimeData(const QModelIndexList &) const override;

    bool canDropMimeData(
        const QMimeData *, Qt::DropAction, int, int, const QModelIndex &) const override;

    bool dropMimeData(const QMimeData *, Qt::DropAction, int, int, const QModelIndex &) override;

public slots:
    void setQueue(const QVector<SongItem *> &);

signals:
    void songsToMove(QVector<unsigned>, unsigned);

private:
};

#endif // PLAYLISTMODEL_H
