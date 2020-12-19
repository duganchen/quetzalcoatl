#ifndef COMPOSERITEM_H
#define COMPOSERITEM_H

#include "item.h"

class ComposerItem : public Item
{
public:
    explicit ComposerItem(QString, Item *parent = nullptr);

    QVector<Item *> fetchMore(Controller *) override;

    QString text(int) const override;

private:
    QString m_composer;
};
#endif // COMPOSERITEM_H
