#ifndef QUEUEMODEL_H
#define QUEUEMODEL_H

#include "itemmodel.h"

class QItemSelection;

class QueueModel : public ItemModel
{
    Q_OBJECT

public:
    explicit QueueModel(Controller *, QObject *parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex &) const override;

    QStringList mimeTypes() const override;

    QMimeData *mimeData(const QModelIndexList &) const override;

    bool canDropMimeData(
        const QMimeData *, Qt::DropAction, int, int, const QModelIndex &) const override;

    bool dropMimeData(const QMimeData *, Qt::DropAction, int, int, const QModelIndex &) override;

    QVariant data(const QModelIndex &index, int role) const override;

    void deleteIndexes(const QModelIndexList &);

    int songId() const;

public slots:
    void setQueue(const QVector<Item *> &);
    void setSongId(int);
    void onSelectionChanged(const QItemSelection &, const QItemSelection &);
signals:
    void hasSongs(bool);

    void songIndex(const QModelIndex &);

private:
    int m_songId{-1};
};

#endif // QUEUEMODEL_H
