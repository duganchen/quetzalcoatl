#include "controller.h"
#include "mainwindow.h"
#include <QApplication>
#include <QIcon>
#include <QObject>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QIcon::setFallbackSearchPaths(QIcon::fallbackSearchPaths() << ":icons");
    MainWindow w;
    w.show();
    return a.exec();
}
