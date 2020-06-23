#include "mpdprocess.h"
#include <mpd/client.h>
#include <QCoreApplication>
#include <QFile>
#include <QTest>
#include <QTextStream>

MPDProcess::MPDProcess(QObject *parent)
    : QObject(parent)
{
    m_mpdProc = new QProcess(this);

    QFile templateFile{"resources/mpd.conf"};
    templateFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in{&templateFile};
    QString tmplate{in.readAll()};
    tmplate = tmplate.arg(QCoreApplication::applicationDirPath() + "/resources/Music");
    QDir().mkdir(m_temp.path() + "/playlists");
    tmplate = tmplate.arg(m_temp.path());

    auto confPath = m_temp.path() + "/mpd.conf";
    QFile confFile{confPath};
    confFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream confStream(&confFile);
    confStream << tmplate;
    confFile.close();

    // For now, we don't need to copy the audio files to the temporary directory. The tests
    // don't modify them. If that changes, see:
    // https://forum.qt.io/topic/105993/copy-folder-qt-c

    QStringList args{"-v", "--no-daemon", confPath};

    m_mpdProc = new QProcess();

    m_mpdProc->setProcessChannelMode(QProcess::ForwardedChannels);

    // On OS X, you may need to go to Projects->Build and add
    // /usr/local/bin to the PATH in the build environment, if that's
    // where mpd is installed.

    qDebug() << "Starting MPD";
    m_mpdProc->start("mpd", args);

    QTest::qWait(MPD_START_MS);

    if (m_mpdProc->state() != QProcess::Running) {
        qDebug() << m_mpdProc->errorString();
    }

    m_socketPath = m_temp.path() + "/socket";
    auto conn = mpd_connection_new(m_socketPath.toUtf8().constData(), 0, 0);
    QVERIFY(conn);
    m_mpdError = mpd_connection_get_error(conn);
    if (MPD_ERROR_SUCCESS == m_mpdError) {
        mpd_run_update(conn, nullptr);
        mpd_connection_free(conn);
        conn = nullptr;
    }
}

MPDProcess::~MPDProcess()
{
    auto confPath = m_temp.path() + "/mpd.conf";
    QStringList args{"--kill", confPath};
    QProcess killer;
    killer.start("mpd", args);
    killer.waitForFinished();
    m_mpdProc->waitForFinished();
}

mpd_error MPDProcess::mpdError()
{
    return m_mpdError;
}

QProcess::ProcessState MPDProcess::mpdState()
{
    return m_mpdProc->state();
}

QString MPDProcess::socketPath()
{
    return m_socketPath;
}
