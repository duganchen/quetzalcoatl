#ifndef COMPILATIONARTISTITEM_H
#define COMPILATIONARTISTITEM_H

#include "item.h"

class CompilationArtistItem : public Item
{
public:
    explicit CompilationArtistItem(const std::vector<QString> &);

    QVector<Item *> fetchMore(Controller *) override;
};

#endif // COMPILATIONARTISTITEM_H
