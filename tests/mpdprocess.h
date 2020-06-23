#ifndef MPDPROCESS_H
#define MPDPROCESS_H

#include <mpd/client.h>
#include <QObject>
#include <QProcess>
#include <QTemporaryDir>

// There's a delay between the time that you start MPD and when it
// starts accepting connections.This wait-time works reliably on my
// 2015 MacBook Pro.
constexpr int MPD_START_MS{500};

class MPDProcess : public QObject
{
    Q_OBJECT
public:
    explicit MPDProcess(QObject *parent = nullptr);
    virtual ~MPDProcess();

    QString socketPath();

    mpd_error mpdError();

    QProcess::ProcessState mpdState();

private:
    QTemporaryDir m_temp;
    QProcess *m_mpdProc;
    mpd_error m_mpdError;
    QString m_socketPath;
};

#endif // MPDPROCESS_H
