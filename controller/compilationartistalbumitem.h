#ifndef COMPILATIONARTISTALBUMITEM_H
#define COMPILATIONARTISTALBUMITEM_H

#include "item.h"

class CompilationArtistAlbumItem : public Item
{
public:
    CompilationArtistAlbumItem(const std::vector<QString> &, QString, Item *parent = nullptr);
    QVector<Item *> fetchMore(Controller *) override;

private:
    QString m_albumArtist;
};
#endif // COMPILATIONARTISTALBUMITEM_H
