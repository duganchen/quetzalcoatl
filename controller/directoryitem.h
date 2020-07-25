#ifndef DIRECTORYITEM_H
#define DIRECTORYITEM_H

#include "controller.h"
#include <mpd/client.h>

class DirectoryItem : public Item
{
public:
    explicit DirectoryItem(mpd_entity *entity, Item *parent = nullptr);
    ~DirectoryItem();

    QString text(int) const override;
    QVector<Item *> fetchMore(Controller *) override;

private:
    mpd_entity *m_entity;
};

#endif // DIRECTORYITEM_H
