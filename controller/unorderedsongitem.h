#ifndef UNORDEREDSONGITEM_H
#define UNORDEREDSONGITEM_H

#include "abstractsongitem.h"

class UnorderedSongItem : public AbstractSongItem
{
public:
    explicit UnorderedSongItem(QIcon, Qt::ItemFlags, bool, bool, Item *parent = nullptr);
    void onDoubleClicked(Controller *) override;
};

#endif // UNORDEREDSONGITEM_H
