#ifndef ITEM_H
#define ITEM_H

#include <QIcon>
#include <QVariant>
#include <QVector>

class Controller;

class Item
{
public:
    Item(const std::vector<QString> &labels, QIcon, Qt::ItemFlags, bool, bool, QString = QString());
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

    QVariant tooltip();

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

    int columnCount() const;

private:
    QVector<Item *> m_children;
    std::vector<QString> m_labels;
    QIcon m_icon;
    Item *m_parent{};
    Qt::ItemFlags m_flags{};
    bool m_hasChildren{};
    bool m_canFetchMore{};
    QString m_tooltip;

    Q_DISABLE_COPY(Item)
};

#endif // ITEM_H
