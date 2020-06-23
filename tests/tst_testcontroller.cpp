#include "controller.h"
#include "mpdprocess.h"
#include <mpd/client.h>
#include <QCoreApplication>
#include <QDebug>
#include <QProcess>
#include <QSignalSpy>
#include <QTest>
#include <QtTest>

class TestController : public QObject
{
    Q_OBJECT

public:
    TestController();
    ~TestController();

private slots:
    void init();
    void cleanup();

    void test_theTest();

private:
    MPDProcess *m_mpdProcess;
};

TestController::TestController()
    : m_mpdProcess(nullptr)
{}

TestController::~TestController() {}

void TestController::init()
{
    m_mpdProcess = new MPDProcess(this);
}

void TestController::cleanup()
{
    delete m_mpdProcess;
    m_mpdProcess = nullptr;
}

void TestController::test_theTest()
{
    QCOMPARE(m_mpdProcess->mpdState(), QProcess::Running);

    QCOMPARE(m_mpdProcess->mpdError(), MPD_ERROR_SUCCESS);
    if (m_mpdProcess->mpdError() != MPD_ERROR_SUCCESS) {
        return;
    }

    Controller controller;
    QSignalSpy spy(&controller, &Controller::connectionState);
    controller.connectToMPD(m_mpdProcess->socketPath().toUtf8().constData(), 0, 1000);
    spy.wait();
    auto albums = controller.getAlbumList();
    QCOMPARE(albums[0], "Touhou Luna Nights - Original Soundtrack");

    QSignalSpy playerSpy(&controller, &Controller::queueChanged);

    auto other = mpd_connection_new(m_mpdProcess->socketPath().toUtf8().constData(), 0, 0);
    QVERIFY(other && mpd_connection_get_error(other) == MPD_ERROR_SUCCESS);

    QVERIFY(mpd_search_db_songs(other, false));
    QVERIFY(
        mpd_search_add_tag_constraint(other, MPD_OPERATOR_DEFAULT, MPD_TAG_TITLE, "Event - 不穏"));
    QVERIFY(mpd_search_commit(other));
    mpd_song *song;
    while ((song = mpd_recv_song(other)) != nullptr) {
        qDebug() << "Queueing " << mpd_song_get_uri(song);
        QVERIFY(mpd_run_add(other, mpd_song_get_uri(song)));
        mpd_song_free(song);
    }

    mpd_connection_free(other);
    other = nullptr;

    playerSpy.wait();

    controller.connectToMPD(m_mpdProcess->socketPath().toUtf8().constData(), 0, 1000);
    spy.wait();
    albums = controller.getAlbumList();
    QCOMPARE(albums[0], "Touhou Luna Nights - Original Soundtrack");
}

QTEST_MAIN(TestController)

#include "tst_testcontroller.moc"
