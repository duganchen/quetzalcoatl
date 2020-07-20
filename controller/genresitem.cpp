#include "genresitem.h"

#include <QDebug>

GenresItem::GenresItem(
    QIcon icon, Qt::ItemFlags myFlags, bool hazChildren, QString text, Item *parent)
    : DBItem(icon, myFlags, hazChildren, text, parent)
{}

void GenresItem::fetchMore(QSocketNotifier *notifier, mpd_connection *mpd)
{
    qDebug() << "Fetching more genres";
    Q_UNUSED(notifier)
    Q_UNUSED(mpd)
    DBItem::fetchMore(notifier, mpd);
}
