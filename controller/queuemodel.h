#ifndef QUEUEMODEL_H
#define QUEUEMODEL_H

#include "itemmodel.h"
#include <QItemSelection>
#include <QMimeData>

class QueueModel : public ItemModel
{
    Q_OBJECT

public:
    explicit QueueModel(Controller *, QObject *parent = nullptr);

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
    void onSelectionChanged(const QItemSelection &, const QItemSelection &);

private:
    int m_songId;
};

#endif // QUEUEMODEL_H
