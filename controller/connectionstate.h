#ifndef CONNECTIONSTATE_H
#define CONNECTIONSTATE_H

#include <QObject>

namespace MPDConnection {
    Q_NAMESPACE
    enum class State { Disconnected, Connecting, Connected };
}; // namespace MPDConnection

#endif 