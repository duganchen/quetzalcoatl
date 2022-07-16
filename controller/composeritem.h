#ifndef COMPOSERITEM_H
#define COMPOSERITEM_H

#include "item.h"

class ComposerItem : public Item
{
public:
    explicit ComposerItem(const std::vector<QString> &);

    QVector<Item *> fetchMore(Controller *) override;
};
#endif // COMPOSERITEM_H
