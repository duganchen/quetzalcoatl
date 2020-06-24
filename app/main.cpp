#include "controller.h"
#include "mainwindow.h"
#include <QApplication>
#include <QIcon>
#include <QObject>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return a.exec();
}
