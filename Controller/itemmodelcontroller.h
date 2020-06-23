#ifndef ITEMMODELCONTROLLER_H
#define ITEMMODELCONTROLLER_H

#include "abstractitem.h"
#include <QModelIndex>
#include <QObject>

class ItemModelController : public QObject
{
    Q_OBJECT
public:
    explicit ItemModelController(QObject *parent = nullptr);

    void beginRemoveRows(int, int);
    void endRemoveRows();

    QVector<AbstractItem *> items;

    ~ItemModelController();

    void clear();

signals:
    void aboutToBeReset();
    void reset();
    void rowsAboutToBeRemoved(const QModelIndex &, int, int);
    void rowsRemoved();
    void rowsAboutToBeInserted(const QModelIndex &, int, int);
    void rowsInserted();
    void rowsAboutToBeMoved(const QModelIndex, int, int, const QModelIndex &, int);
    void rowsMoved();
};

#endif // ITEMMODELCONTROLLER_H
