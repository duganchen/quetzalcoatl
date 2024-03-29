#include "genresitem.h"
#include "controller.h"
#include "genreitem.h"
#include "iconnames.h"

GenresItem::GenresItem()
    : Item({"Genres"}, QIcon::fromTheme(IconNames::Database), Qt::ItemIsEnabled, true, true)
{}
QVector<Item *> GenresItem::fetchMore(Controller *controller)
{
    QVector<Item *> items;
    for (auto genre : controller->listTags(MPD_TAG_GENRE)) {
        items.append(new GenreItem({genre}));
    }
    return items;
}
