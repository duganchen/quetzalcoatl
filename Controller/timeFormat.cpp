#include "timeFormat.h"

QString timeStr(unsigned duration)
{
    // Can't remember where I got this time format from. It might have been Foobar2000.

    unsigned seconds = duration % 60;
    unsigned minutes = duration % 3600 / 60;
    unsigned hours = duration / 3600;

    // For the string formatting:
    // Re: https://stackoverflow.com/a/2618444/240515

    if (!hours && !minutes) {
        return QStringLiteral("0:%1").arg(seconds, 2, 10, QLatin1Char('0'));
    }

    if (!hours) {
        return QStringLiteral("%1:%2").arg(QString::number(minutes), leftPad(seconds));
    }

    return QStringLiteral("%1:%2:%3").arg(QString::number(hours), leftPad(minutes), leftPad(seconds));
}

QString leftPad(unsigned i)
{
    // Left-zero-pad to two digits.
    return QStringLiteral("%1").arg(i, 2, 10, QLatin1Char('0'));
}
