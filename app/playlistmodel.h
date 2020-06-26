#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include "itemmodel.h"

class PlaylistModel : public ItemModel
{
    Q_OBJECT

public:
    explicit PlaylistModel(Controller *, Item *, QObject *parent = nullptr);

    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
};

#endif // PLAYLISTMODEL_H
