#ifndef COMPOSERALBUMITEM_H
#define COMPOSERALBUMITEM_H

#include "item.h"

class ComposerAlbumItem : public Item
{
public:
    ComposerAlbumItem(const std::vector<QString> &, QString);
    QVector<Item *> fetchMore(Controller *) override;

private:
    QString m_composer;
};

#endif // COMPOSERALBUMITEM_H
