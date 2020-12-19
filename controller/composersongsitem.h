#ifndef COMPOSERSONGSITEM_H
#define COMPOSERSONGSITEM_H

#include "item.h"

class ComposerSongsItem : public Item
{
public:
    explicit ComposerSongsItem(QString, Item *parent = nullptr);

    QVector<Item *> fetchMore(Controller *) override;

    QString text(int) const override;

private:
    QString m_composer;
};

#endif // COMPOSERSONGSITEM_H
