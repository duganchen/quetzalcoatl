#ifndef COMPOSERALBUMITEM_H
#define COMPOSERALBUMITEM_H

#include "item.h"

class ComposerAlbumItem : public Item
{
public:
    explicit ComposerAlbumItem(QString, QString, Item *parent = nullptr);
    QVector<Item *> fetchMore(Controller *) override;
    QString text(int) const override;

private:
    QString m_composer;
    QString m_album;
};

#endif // COMPOSERALBUMITEM_H
