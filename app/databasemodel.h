#ifndef DATABASEMODEL_H
#define DATABASEMODEL_H

#include "item.h"
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class DatabaseModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit DatabaseModel(QObject *parent = nullptr);
    ~DatabaseModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
    Item *rootItem;
};

#endif // DATABASEMODEL_H
