#include "mainwindow.h"
#include "controller.h"
#include "databasemodel.h"
#include "playbacksettingsdialog.h"
#include "playlistmodel.h"
#include "saveplaylistdialog.h"
#include <QKeySequence>
#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>
#include <QSettings>
#include <QSplitter>
#include <QStatusBar>
#include <QTimer>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Quetzalcoatl"));
    setWindowIcon(QIcon(":/icons/multimedia-player.svg"));

    auto controller = new Controller(this);
    connect(controller, &Controller::connectionState, this, &MainWindow::setConnectionState);

    auto toolBar = addToolBar("ToolBar");
    toolBar->setMovable(false);
    toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    m_connectionDialog = new ConnectionDialog(controller, this);

    auto connectAction = toolBar->addAction(QIcon(":/icons/network-connect.svg"),
                                            "Connect to MPD",
                                            [=]() { m_connectionDialog->exec(); });
    toolBar->addSeparator();

    auto stopAction = toolBar->addAction(QIcon(":/icons/stop_circle-24px.svg"), "Stop");
    stopAction->setShortcut(QKeySequence(Qt::Key::Key_MediaStop));
    stopAction->setEnabled(false);
    m_connectedActions.append(stopAction);

    auto playAction = toolBar->addAction(QIcon(":/icons/play_circle_outline-24px.svg"), "Play");
    playAction->setShortcut(QKeySequence(Qt::Key::Key_MediaPlay));
    playAction->setEnabled(false);
    m_connectedActions.append(playAction);

    auto pauseAction = toolBar->addAction(QIcon(":/icons/pause_circle_outline-24px.svg"), "Pause");
    pauseAction->setShortcut(QKeySequence(Qt::Key::Key_MediaPause));
    pauseAction->setEnabled(false);
    m_connectedActions.append(pauseAction);

    auto skipBackAction = toolBar->addAction(QIcon(":/icons/skip_previous-24px.svg"), "Previous");
    skipBackAction->setShortcut(QKeySequence(Qt::Key::Key_MediaPrevious));
    skipBackAction->setEnabled(false);
    m_connectedActions.append(skipBackAction);

    auto skipForthAction = toolBar->addAction(QIcon(":/icons/skip_next-24px.svg"), "Next");
    skipForthAction->setShortcut(QKeySequence(Qt::Key::Key_MediaNext));
    skipForthAction->setEnabled(false);
    m_connectedActions.append(skipForthAction);

    toolBar->addSeparator();

    auto shuffleAction = toolBar->addAction(QIcon(":icons/shuffle-24px.svg"), "Shuffle");
    shuffleAction->setCheckable(true);
    shuffleAction->setEnabled(false);
    m_connectedActions.append(shuffleAction);

    auto repeatAction = toolBar->addAction(QIcon(":/icons/repeat-24px.svg"), "Repeat");
    repeatAction->setCheckable(true);
    repeatAction->setEnabled(false);
    m_connectedActions.append(repeatAction);

    toolBar->addSeparator();

    auto deleteAction = toolBar->addAction(QIcon(":/icons/remove_from_queue-24px.svg"),
                                           "[DEL]ete selected playlist items");
    deleteAction->setEnabled(false);

    deleteAction->setShortcut(QKeySequence(Qt::Key_Delete));
    auto savePlaylistDialog = new SavePlaylistDialog(controller, this);
    savePlaylistDialog->setEnabled(false);

    auto savePlaylistAction = toolBar->addAction(QIcon(":/icons/save-24px.svg"),
                                                 "[CTRL-S]ave playlist",
                                                 [=]() { savePlaylistDialog->exec(); });

    savePlaylistAction->setShortcut(QKeySequence("CTRL+S"));
    savePlaylistAction->setEnabled(false);
    m_connectedActions.append(savePlaylistAction);

    toolBar->addSeparator();

    auto playbackSettingsDialog = new PlaybackSettingsDialog(controller, this);
    m_connectedWidgets.append(playbackSettingsDialog);
    playbackSettingsDialog->setEnabled(false);
    auto playbackSettingsAction = toolBar->addAction(QIcon(":/icons/configure.svg"),
                                                     "Playback Settings",
                                                     [=]() { playbackSettingsDialog->exec(); });
    playbackSettingsAction->setEnabled(false);
    m_connectedActions.append(playbackSettingsAction);

    auto layout = new QVBoxLayout();
    m_slider = new QSlider(Qt::Horizontal);
    m_slider->setTracking(false);

    connect(controller, &Controller::sliderMax, m_slider, &QSlider::setMaximum);
    connect(controller, &Controller::sliderValue, m_slider, &QSlider::setValue);
    connect(controller, &Controller::statusMessage, statusBar(), &QStatusBar::showMessage);

    layout->addWidget(m_slider);
    m_slider->setEnabled(false);
    m_connectedWidgets.append(m_slider);

    auto splitter = new QSplitter();

    auto databaseModel = new DatabaseModel(controller);
    auto databaseView = new QTreeView();
    databaseView->setHeaderHidden(true);
    databaseView->setModel(databaseModel);
    databaseView->setEnabled(false);
    databaseView->setSelectionMode(QTreeView::ExtendedSelection);
    databaseView->setDragEnabled(true);
    m_connectedWidgets.append(databaseView);
    splitter->addWidget(databaseView);

    auto playlistModel = new PlaylistModel(controller);

    auto playlistView = new QTreeView();
    playlistView->setAcceptDrops(true);
    playlistView->setDropIndicatorShown(true);
    playlistView->setDragEnabled(true);
    playlistView->setModel(playlistModel);
    playlistView->setSelectionMode(QTreeView::ExtendedSelection);
    connect(playlistModel,
            &ItemModel::columnResized,
            playlistView,
            &QTreeView::resizeColumnToContents);
    splitter->addWidget(playlistView);
    playlistView->setEnabled(false);

    m_connectedWidgets.append(playlistView);
    layout->addWidget(splitter);
    auto widget = new QWidget();
    widget->setLayout(layout);
    setCentralWidget(widget);

    auto status = statusBar();
    auto combinedTimeLabel = new QLabel();
    status->addWidget(combinedTimeLabel);

    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, controller, &Controller::updateStatus);
    timer->start(1000);

    // Error messages from the Controller are unforeseen and unrecoverable. If we get one, then we just
    // let the user shut the program down.
    connect(controller, &Controller::errorMessage, [=](QString message) {
        QMessageBox::critical(this, "Critical error", message);
        setConnectionState(Controller::ConnectionState::Disconnected);
        connectAction->setEnabled(false);
        timer->stop();
    });

    connect(controller, &Controller::repeating, repeatAction, &QAction::setChecked);
    connect(controller, &Controller::shuffled, shuffleAction, &QAction::setChecked);
    connect(controller,
            &Controller::volume,
            playbackSettingsDialog,
            &PlaybackSettingsDialog::setVolume);
    connect(controller,
            &Controller::crossfade,
            playbackSettingsDialog,
            &PlaybackSettingsDialog::setCrossfade);

    QSettings settings;
    if (settings.contains("host") && settings.contains("port")) {
        m_connectionDialog->connectToMPD();
    }
}

MainWindow::~MainWindow() {}

void MainWindow::setConnectionState(Controller::ConnectionState connectionState)
{
    m_connectionDialog->setConnectionState(connectionState);

    if (Controller::ConnectionState::Connected == connectionState) {
        for (auto widget : m_connectedWidgets) {
            widget->setEnabled(true);
        }

        for (auto action : m_connectedActions) {
            action->setEnabled(true);
        }
    } else {
        for (auto widget : m_connectedWidgets) {
            widget->setEnabled(false);
        }

        for (auto action : m_connectedActions) {
            action->setEnabled(false);
        }

        statusBar()->clearMessage();
    }
}
