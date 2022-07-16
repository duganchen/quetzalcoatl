#ifndef COMPOSERSONGSITEM_H
#define COMPOSERSONGSITEM_H

#include "item.h"

class ComposerSongsItem : public Item
{
public:
    explicit ComposerSongsItem(QString);

    QVector<Item *> fetchMore(Controller *) override;

private:
    QString m_composer;
};

#endif // COMPOSERSONGSITEM_H
