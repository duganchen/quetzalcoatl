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
};
#endif // MAINWINDOW_H
