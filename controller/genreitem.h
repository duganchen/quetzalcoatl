#ifndef GENREITEM_H
#define GENREITEM_H

#include "dbitem.h"

class GenreItem : public Item
{
public:
    explicit GenreItem(QString, Item *parent = nullptr);
    QVector<Item *> fetchMore(Controller *) override;

    QString text(int) const override;

private:
    QString m_genre;
};
#endif // GENREITEM_H
