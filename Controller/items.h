#ifndef ITEMS_H
#define ITEMS_H

// This is largely from this video:
// https://youtu.be/9BcAYDlpuT8
// Which is embedded here:
// https://doc.qt.io/qt-5/qtquick-modelviewsdata-cppmodels.html

#include "dbitem.h"
#include <QModelIndex>
#include <QObject>

class Items : public QObject
{
    Q_OBJECT

public:
    explicit Items(AbstractItem *, QObject *parent = nullptr);
    ~Items();

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

#endif // ITEMS_H
