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

    QVariant data(const QModelIndex &index, int role) const override;

public slots:
    void setQueue(const QVector<Item *> &);
    void setSongId(int);

private:
    int m_songId;
};

#endif // PLAYLISTMODEL_H
