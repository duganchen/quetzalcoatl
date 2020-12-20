#ifndef QUEUEDITEM_H
#define QUEUEDITEM_H

#include "songitem.h"
#include <mpd/client.h>

class QueuedItem : public SongItem
{
public:
    explicit QueuedItem(mpd_entity *, Item *parent = nullptr);
    ~QueuedItem();

    void onDoubleClicked(Controller *) override;
    unsigned id() override;
    unsigned duration() override;

protected:
    const mpd_song *song() const override;

private:
    mpd_entity *m_entity{};
};

#endif // QUEUEDITEM_H
