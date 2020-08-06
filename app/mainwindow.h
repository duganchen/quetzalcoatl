#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "connectiondialog.h"
#include "controller.h"
#include <QAction>
#include <QEvent>
#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QPushButton>
#include <QSlider>
#include <QVector>
#include <QWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void changeEvent(QEvent *);

private:
    QVector<QWidget *> m_connectedWidgets;
    QVector<QAction *> m_connectedActions;

    QSlider *m_slider;

    ConnectionDialog *m_connectionDialog;

private:
    void onPaletteChanged();
    int m_seekPosition;
    Controller *m_controller;
private slots:
    void setConnectionState(Controller::ConnectionState);
};
#endif // MAINWINDOW_H
