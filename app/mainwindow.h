#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "connectiondialog.h"
#include "controller.h"
#include <QAction>
#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QPushButton>
#include <QSlider>
#include <QVector>
#include <QWidget>

#include "connectionstate.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QVector<QWidget *> m_connectedWidgets;
    QVector<QAction *> m_connectedActions;

    QSlider *m_slider;

    ConnectionDialog *m_connectionDialog;

private:
private slots:
    void setConnectionState(Controller::ConnectionState);
};
#endif // MAINWINDOW_H
