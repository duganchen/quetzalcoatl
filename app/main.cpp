#include "controller.h"
#include "mainwindow.h"
#include <QApplication>
#include <QIcon>
#include <QObject>
#include <QThread>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("dugan-c");
    QCoreApplication::setOrganizationDomain("duganchen.ca");
    QCoreApplication::setApplicationName("Quetzalcoatl");

    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return a.exec();
}
