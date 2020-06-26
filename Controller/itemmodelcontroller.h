#ifndef ITEMMODELCONTROLLER_H
#define ITEMMODELCONTROLLER_H

#include <QObject>
#include <QModelIndex>
#include "item.h"

class ItemModelController : public QObject
{
    Q_OBJECT

public:
    explicit ItemModelController(QObject *parent = nullptr);
    ~ItemModelController();

public:
    Item *rootItem() const;
signals:
    void rowsAboutToBeInserted(const QModelIndex &, int, int);
    void rowsAboutToBeMoved(const QModelIndex &, int, int, const QModelIndex &, int);
    void rowsAboutToBeRemoved(const QModelIndex &, int, int);
    void modelAboutToBeReset();
    void rowsInserted();
    void rowsMoved();
    void rowsRemoved();
    void modelReset();

private:
    Item *m_rootItem;
};


#endif // ITEMMODELCONTROLLER_H
