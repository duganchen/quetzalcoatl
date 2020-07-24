#ifndef COMPILATIONARTISTALBUMITEM_H
#define COMPILATIONARTISTALBUMITEM_H

#include "controller.h"

class CompilationArtistAlbumItem : public Item
{
public:
    explicit CompilationArtistAlbumItem(QString, QString, Item *parent = nullptr);
    QVector<Item *> fetchMore(Controller *) override;
    QString text(int) const override;

private:
    QString m_albumArtist;
    QString m_album;
};
#endif // COMPILATIONARTISTALBUMITEM_H
