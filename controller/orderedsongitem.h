#ifndef ORDEREDSONGITEM_H
#define ORDEREDSONGITEM_H

#include "abstractsongitem.h"

class OrderedSongItem : public AbstractSongItem
{
public:
    explicit OrderedSongItem(QIcon, Qt::ItemFlags, bool, bool, Item *parent = nullptr);
    void onDoubleClicked(Controller *) override;
};

#endif // ORDEREDSONGITEM_H
