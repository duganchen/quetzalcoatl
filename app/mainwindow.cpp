#include "mainwindow.h"
#include "connectionstate.h"
#include <QPushButton>
#include <QStatusBar>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Qt/libmpdclient Demo"));
    auto widget = new QWidget();
    auto layout = new QVBoxLayout();
    m_connectButton = new QPushButton(tr("&Connect"));
    connect(m_connectButton, &QPushButton::clicked, [=]() {
        setConnectionState(Controller::ConnectionState::Connecting);
        emit connectClicked();
    });
    layout->addWidget(m_connectButton);
    m_listAbumsButton = new QPushButton(tr("&List Albums"));
    connect(m_listAbumsButton, &QPushButton::clicked, [=]() { emit listAlbumsClicked(); });
    m_listAbumsButton->setEnabled(false);
    layout->addWidget(m_listAbumsButton);

    m_busyIndicator = new QProgressBar();
    m_busyIndicator->setMinimum(0);
    m_busyIndicator->setMaximum(1);
    layout->addWidget(m_busyIndicator);

    widget->setLayout(layout);

    setCentralWidget(widget);
}

MainWindow::~MainWindow() {}

void MainWindow::setConnectionState(Controller::ConnectionState connectionState)
{
    switch (connectionState) {
    case Controller::ConnectionState::Disconnected:
        m_connectButton->setEnabled(true);
        m_listAbumsButton->setEnabled(false);
        m_busyIndicator->setMaximum(1);
        break;

    case Controller::ConnectionState::Connecting:
        m_connectButton->setEnabled(false);
        m_listAbumsButton->setEnabled(false);
        m_busyIndicator->setMaximum(0);
        break;
    case Controller::ConnectionState::Connected:
        m_connectButton->setEnabled(false);
        m_listAbumsButton->setEnabled(true);
        m_busyIndicator->setMaximum(1);
        break;
    }
}
