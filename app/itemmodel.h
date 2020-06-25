#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include "item.h"
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class ItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit ItemModel(Item *rootItem, QObject *parent = nullptr);
    ~ItemModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    bool canFetchMore(const QModelIndex &parent) const override;

private:
    Item *m_rootItem;
};

#endif // ITEMMODEL_H
