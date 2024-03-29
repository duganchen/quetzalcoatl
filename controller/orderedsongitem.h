#ifndef ORDEREDSONGITEM_H
#define ORDEREDSONGITEM_H

#include "songitem.h"

class OrderedSongItem : public SongItem
{
public:
    OrderedSongItem(std::vector<QString> labels, QIcon, Qt::ItemFlags, bool, bool, QString);
    void onDoubleClicked(Controller *) override;
};

#endif // ORDEREDSONGITEM_H
