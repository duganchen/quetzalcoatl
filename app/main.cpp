#include "controller.h"
#include "mainwindow.h"
#include <QApplication>
#include <QObject>
#include <QThread>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    Controller controller("localhost", 6600, 1000);
    QObject::connect(&w,
                     &MainWindow::listAlbumsClicked,
                     &controller,
                     &Controller::handleListAlbumsClick);
    QObject::connect(&w, &MainWindow::connectClicked, &controller, &Controller::handleConnectClick);
    QObject::connect(&controller, &Controller::connectionState, &w, &MainWindow::setConnectionState);

    QObject::connect(&controller, &Controller::unrecoverableError, &a, &QApplication::quit);

    w.show();
    return a.exec();
}
