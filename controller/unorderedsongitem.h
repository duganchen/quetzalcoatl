#ifndef UNORDEREDSONGITEM_H
#define UNORDEREDSONGITEM_H

#include "songitem.h"

class UnorderedSongItem : public SongItem
{
public:
    UnorderedSongItem(std::vector<QString> labels, QIcon, Qt::ItemFlags, bool, bool);
    void onDoubleClicked(Controller *) override;
};

#endif // UNORDEREDSONGITEM_H
