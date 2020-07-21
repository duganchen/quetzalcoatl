#ifndef DBSONGITEM_H
#define DBSONGITEM_H

#include "item.h"
#include <mpd/client.h>

class DBSongItem : public Item
{
public:
    DBSongItem(QIcon, Qt::ItemFlags, bool, mpd_song *, Item *parent = nullptr);
    ~DBSongItem();

    QString text(int column) const override;

    QVariant tooltip() override;

private:
    mpd_song *m_song;
};

#endif // DBSONGITEM_H
