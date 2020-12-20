#ifndef DATABASEMODEL_H
#define DATABASEMODEL_H

#include "controller.h"
#include "itemmodel.h"

class DatabaseModel : public ItemModel
{
    Q_OBJECT

public:
    explicit DatabaseModel(Controller *, QObject *parent = nullptr);
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void renamePlaylist(const QModelIndex &, QString);
public slots:
    void setPlaylists(const QVector<Item *> &);
    void onConnectionChanged(Controller::ConnectionState);

    void reset();
};

#endif // DATABASEMODEL_H
