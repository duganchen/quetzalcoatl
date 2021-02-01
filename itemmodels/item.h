#ifndef ITEM_H
#define ITEM_H

#include <QIcon>
#include <QVariant>
#include <QVector>

namespace mpd {
class Connection;
}

class Controller;

class Item
{
public:
    Item(QIcon, Qt::ItemFlags, bool, bool, Item *parent = nullptr);
    virtual ~Item();

    void append(Item *at);

    Item *at(int row);
    int count() const;
    QIcon icon() const;
    virtual QString text(int) const;
    int row() const;
    Item *parent();
    void setParent(Item *parent);

    void clear();

    virtual QVariant tooltip();

    virtual bool hasChildren();
    QVector<Item *> &children();

    Qt::ItemFlags flags() const;

    bool canFetchMore();
    void setCanFetchMore(bool);
    virtual QVector<Item *> fetchMore(mpd::Connection &);

    virtual void onDoubleClicked(mpd::Connection &);

private:
    Q_DISABLE_COPY(Item)
    QVector<Item *> m_children;
    QIcon m_icon;
    Item *m_parent{};
    Qt::ItemFlags m_flags{};
    bool m_hasChildren{};
    bool m_canFetchMore{};
};

#endif // ITEM_H
