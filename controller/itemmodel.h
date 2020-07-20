#ifndef ITEMMODEL_H
#define ITEMMODEL_H

// This closely follows the official Qt Simple Tree Model
// example.
// https://doc.qt.io/qt-5/qtwidgets-itemviews-simpletreemodel-example.html
// The nodes are in items.h.

#include "controller.h"
#include "items.h"
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class ItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit ItemModel(Items *, QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
signals:
    void columnResized(int);

private:
    Items *m_items;
};

#endif // ITEMMODEL_H
