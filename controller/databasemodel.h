#ifndef DATABASEMODEL_H
#define DATABASEMODEL_H

#include "itemmodel.h"

class DatabaseModel : public ItemModel
{
    Q_OBJECT

public:
    explicit DatabaseModel(Controller *, QObject *parent = nullptr);
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &) const;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
};

#endif // DATABASEMODEL_H
