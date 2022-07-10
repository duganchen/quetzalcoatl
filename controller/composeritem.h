#ifndef COMPOSERITEM_H
#define COMPOSERITEM_H

#include "item.h"

class ComposerItem : public Item
{
public:
    explicit ComposerItem(const std::vector<QString> &, QString, Item *parent = nullptr);

    QVector<Item *> fetchMore(Controller *) override;

private:
    QString m_composer;
};
#endif // COMPOSERITEM_H
