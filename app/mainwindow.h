#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "connectionstate.h"
#include "controller.h"

#include <QMainWindow>
#include <QVector>

class ConnectionDialog;
class Controller;

class QAction;
class QSlider;
class QWidget;

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
    int m_seekPosition{};
    Controller *m_controller{};
    QAction *m_connectAction{};

    bool m_iconsAreThere{true};
private slots:
    void setConnectionState(MPDConnection::State);
};
#endif // MAINWINDOW_H
