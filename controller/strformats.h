#ifndef TIMEFORMAT_H
#define TIMEFORMAT_H

#include <mpd/client.h>
#include <QString>

QString timeStr(unsigned);

QString leftPad(unsigned);

QString songLabel(const mpd_song *);
QString songEntityLabel(mpd_entity *);
QString dirEntryLabel(mpd_entity *);

QString songToolTip(const mpd_song *);
QString songDuration(const mpd_song *);

#endif
