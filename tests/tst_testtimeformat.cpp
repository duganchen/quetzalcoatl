#include "strformats.h"
#include <QObject>
#include <QTest>

class TestTimeFormat : public QObject
{
    Q_OBJECT

public:
    TestTimeFormat();
    ~TestTimeFormat();

private slots:
    void test_oneSecond();
    void test_72Seconds();
    void test_oneHourTenMinutesTwelveSeconds();
};

TestTimeFormat::TestTimeFormat() {}

TestTimeFormat::~TestTimeFormat() {}

void TestTimeFormat::test_oneSecond()
{
    QCOMPARE(timeStr(1), "0:01");
}

void TestTimeFormat::test_72Seconds()
{
    QCOMPARE(timeStr(72), "1:12");
}

void TestTimeFormat::test_oneHourTenMinutesTwelveSeconds()
{
    QCOMPARE(timeStr(4212), "1:10:12");
}

QTEST_MAIN(TestTimeFormat)

#include "tst_testtimeformat.moc"
