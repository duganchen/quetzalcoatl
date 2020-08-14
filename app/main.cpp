#include "controller.h"
#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QIcon>
#include <QLockFile>
#include <QMessageBox>
#include <QObject>
#include <QThread>

int main(int argc, char *argv[])
{
    qputenv("QT_STYLE_OVERRIDE", "");

    QCoreApplication::setOrganizationName("dugan-c");
    QCoreApplication::setOrganizationDomain("duganchen.ca");
    QCoreApplication::setApplicationName("Quetzalcoatl");

    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    // I'm just going to make this a single-instance application. This avoids so many bugs...
    // Implemetation is from here:
    // http://blog.aeguana.com/2015/10/15/how-to-run-a-single-app-instance-in-qt/

    auto tmpDir = QDir::tempPath();
    QLockFile lockFile(tmpDir + "/quetzalcoatl.lock");

    if (!lockFile.tryLock(100)) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("You already have Quetzalcoatl running."
                       "\r\nOnly one instance is allowed.");
        msgBox.exec();
        return 1;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
