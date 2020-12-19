#ifndef COMPILATIONARTISTITEM_H
#define COMPILATIONARTISTITEM_H

#include "item.h"

class CompilationArtistItem : public Item
{
public:
    explicit CompilationArtistItem(QString, Item *parent = nullptr);

    QVector<Item *> fetchMore(Controller *) override;

    QString text(int) const override;

private:
    QString m_albumArtist;
};

#endif // COMPILATIONARTISTITEM_H
