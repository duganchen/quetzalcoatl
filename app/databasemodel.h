#ifndef DATABASEMODEL_H
#define DATABASEMODEL_H

#include "itemmodel.h"

class DatabaseModel : public ItemModel
{
    Q_OBJECT

public:
    explicit DatabaseModel(Item *rootItem, QObject *parent = nullptr);
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
};

#endif // DATABASEMODEL_H
