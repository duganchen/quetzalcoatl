#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "controller.h"
#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QPushButton>

#include "connectionstate.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void setConnectionState(Controller::ConnectionState);
signals:
    void connectClicked();
    void listAlbumsClicked();

private:
    QPushButton *m_connectButton;
    QPushButton *m_listAbumsButton;
    QProgressBar *m_busyIndicator;
};
#endif // MAINWINDOW_H
