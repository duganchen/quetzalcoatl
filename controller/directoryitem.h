#ifndef DIRECTORYITEM_H
#define DIRECTORYITEM_H

#include "item.h"
#include <mpd/client.h>

class DirectoryItem : public Item
{
public:
    explicit DirectoryItem(const std::vector<QString> &labels, mpd_entity *entity);
    ~DirectoryItem();
    QVector<Item *> fetchMore(Controller *) override;

private:
    mpd_entity *m_entity{};
};

#endif // DIRECTORYITEM_H
