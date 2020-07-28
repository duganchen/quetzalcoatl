#ifndef UNORDEREDSONGITEM_H
#define UNORDEREDSONGITEM_H

#include "songitem.h"

class UnorderedSongItem : public SongItem
{
public:
    explicit UnorderedSongItem(QIcon, Qt::ItemFlags, bool, bool, Item *parent = nullptr);
    void onDoubleClicked(Controller *) override;
};

#endif // UNORDEREDSONGITEM_H
