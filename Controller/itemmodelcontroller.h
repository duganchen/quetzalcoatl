#ifndef ITEMMODELCONTROLLER_H
#define ITEMMODELCONTROLLER_H

#include "dbitem.h"
#include <QModelIndex>
#include <QObject>

class ItemModelController : public QObject
{
    Q_OBJECT

public:
    explicit ItemModelController(AbstractItem *, QObject *parent = nullptr);
    ~ItemModelController();

public:
    AbstractItem *rootItem() const;
signals:

    void rowsAboutToBeInserted(int, int);
    void rowsInserted();

#if 0
    void rowsAboutToBeMoved(const QModelIndex &, int, int, const QModelIndex &, int);
    void rowsAboutToBeRemoved(const QModelIndex &, int, int);
    void modelAboutToBeReset();
    void rowsMoved();
    void rowsRemoved();
    void modelReset();
#endif

private:
    AbstractItem *m_rootItem;
};

#endif // ITEMMODELCONTROLLER_H
