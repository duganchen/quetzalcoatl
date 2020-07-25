#ifndef TIMEFORMAT_H
#define TIMEFORMAT_H

#include <mpd/client.h>
#include <QString>

QString timeStr(unsigned);

QString leftPad(unsigned);

QString songLabel(const mpd_song *);
QString songEntityLabel(mpd_entity *);
QString dirEntryLabel(mpd_entity *);

QString songToolTip(mpd_song *);
QString songDuration(mpd_song *);

#endif
