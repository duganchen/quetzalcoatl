#ifndef DATABASEMODEL_H
#define DATABASEMODEL_H

#include "connectionstate.h"
#include "controller.h"
#include "itemmodel.h"

class DatabaseModel : public ItemModel
{
    Q_OBJECT

public:
    explicit DatabaseModel(Controller *, QObject *parent = nullptr);

    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    void renamePlaylist(const QModelIndex &, QString);
public slots:
    void setPlaylists(const QVector<Item *> &);
    void onConnectionChanged(MPDConnection::State);

    void reset();
};

#endif // DATABASEMODEL_H
