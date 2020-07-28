#ifndef ITEM_H
#define ITEM_H

#include <mpd/client.h>
#include <QSocketNotifier>
#include <QVariant>
#include <QVector>
#include <QtGui/QIcon>

class Controller;

class Item
{
public:
    explicit Item(QIcon, Qt::ItemFlags, bool, bool, Item *parent = nullptr);
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

    Qt::ItemFlags flags() const;

    bool canFetchMore();
    void setCanFetchMore(bool);

    virtual QVector<Item *> fetchMore(Controller *);
    virtual bool hasChildren();
    QVector<Item *> &children();

    // The song URI, for songs in the database.
    virtual QString uri();

    // The song id, for songs in the queue.
    virtual unsigned id();

    virtual unsigned duration();

    virtual void onDoubleClicked(Controller *);

private:
    QVector<Item *> m_children;
    QIcon m_icon;
    Item *m_parent;
    Qt::ItemFlags m_flags;
    bool m_hasChildren;
    bool m_canFetchMore;
};

#endif // ITEM_H
