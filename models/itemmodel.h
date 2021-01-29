#ifndef ITEMMODEL_H
#define ITEMMODEL_H

// This closely follows the official Qt Simple Tree Model
// example.
// https://doc.qt.io/qt-5/qtwidgets-itemviews-simpletreemodel-example.html
// The nodes are in items.h.

#include <QAbstractItemModel>

class Controller;
class Item;

class ItemModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit ItemModel(Controller *, QObject *parent = nullptr);
    ~ItemModel();

    QVariant data(const QModelIndex &index, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

public slots:
    void onDoubleClicked(const QModelIndex &);
signals:
    void columnResized(int);

protected:
    void setRootItem(Item *);
    Item *rootItem() const;
    Controller *controller() const;

private:
    Item *m_rootItem{};
    Controller *m_controller{};
};

#endif // ITEMMODEL_H
