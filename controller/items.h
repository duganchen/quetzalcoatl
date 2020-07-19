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
    explicit Items(Item *, QObject *parent = nullptr);
    ~Items();

public:
    Item *rootItem() const;
signals:

    void modelAboutToBeReset();
    void modelReset();

#if 0
    void rowsAboutToBeInserted(int, int);
    void rowsInserted();
    void rowsMoved();
    void rowsRemoved();

    void rowsAboutToBeMoved(const QModelIndex &, int, int, const QModelIndex &, int);
    void rowsAboutToBeRemoved(const QModelIndex &, int, int);
    void modelAboutToBeReset();
    void rowsMoved();
    void rowsRemoved();
    void modelReset();
#endif

private:
    Item *m_rootItem;
};

#endif // ITEMS_H
