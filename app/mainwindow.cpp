#include "mainwindow.h"
#include "controller.h"
#include "databasemodel.h"
#include "playbacksettingsdialog.h"
#include "playlistdelegate.h"
#include "queuemodel.h"
#include "saveplaylistdialog.h"
#include <QDebug>
#include <QKeySequence>
#include <QMenu>
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
#ifndef Q_OS_LINUX

    /*
     * Future versions should detect when the OS changes into dark mode.
    void MainWindow::changeEvent(QEvent *event)
    {
        if (event->type() == QEvent::PaletteChange) {
            qDebug() << palette().color(QPalette::Active, QPalette::Window);
        }
    }
    */

    constexpr OSX_LIGHT_MODE = 236;
    constexpr OSX_DARK_MODE = 50;
    constexpr THRESHOLD = OSX_LIGHT_MODE / 2 - OSX_DARK_MODE / 2;

    if (palette().color(QPalette::Active, QPalette::Window).lightness() < THRESHOLD) {
        QIcon::setThemeName("breeze-dark");
    } else {
        QIcon::setThemeName("breeze");
    }
#endif
    setWindowTitle(tr("Quetzalcoatl"));
    setWindowIcon(QIcon::fromTheme("multimedia-player"));

    auto controller = new Controller(this);
    connect(controller, &Controller::connectionState, this, &MainWindow::setConnectionState);

    auto toolBar = addToolBar("ToolBar");
    toolBar->setMovable(false);
    toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    m_connectionDialog = new ConnectionDialog(controller, this);

    auto connectAction = toolBar->addAction(QIcon::fromTheme("network-connect"),
                                            "Connect to MPD",
                                            [=]() { m_connectionDialog->exec(); });
    toolBar->addSeparator();

    auto stopAction = toolBar->addAction(QIcon::fromTheme("media-playback-stop"), "Stop");
    stopAction->setShortcut(QKeySequence(Qt::Key::Key_MediaStop));
    stopAction->setEnabled(false);
    m_connectedActions.append(stopAction);

    auto playAction = toolBar->addAction(QIcon::fromTheme("media-playback-start"), "Play");
    playAction->setShortcut(QKeySequence(Qt::Key::Key_MediaPlay));
    playAction->setEnabled(false);
    m_connectedActions.append(playAction);

    auto pauseAction = toolBar->addAction(QIcon::fromTheme("media-playback-pause"), "Pause");
    pauseAction->setShortcut(QKeySequence(Qt::Key::Key_MediaPause));
    pauseAction->setEnabled(false);
    m_connectedActions.append(pauseAction);

    auto skipBackAction = toolBar->addAction(QIcon::fromTheme("media-skip-backward"), "Previous");
    skipBackAction->setShortcut(QKeySequence(Qt::Key::Key_MediaPrevious));
    skipBackAction->setEnabled(false);
    m_connectedActions.append(skipBackAction);

    auto skipForthAction = toolBar->addAction(QIcon::fromTheme("media-skip-forward"), "Next");
    skipForthAction->setShortcut(QKeySequence(Qt::Key::Key_MediaNext));
    skipForthAction->setEnabled(false);
    m_connectedActions.append(skipForthAction);

    toolBar->addSeparator();

    auto shuffleAction = toolBar->addAction(QIcon::fromTheme("media-playlist-shuffle"), "Shuffle");
    shuffleAction->setCheckable(true);
    shuffleAction->setEnabled(false);
    m_connectedActions.append(shuffleAction);

    auto repeatAction = toolBar->addAction(QIcon::fromTheme("media-playlist-repeat"), "Repeat");
    repeatAction->setCheckable(true);
    repeatAction->setEnabled(false);
    m_connectedActions.append(repeatAction);

    toolBar->addSeparator();

    auto deleteAction = toolBar->addAction(QIcon::fromTheme("list-remove"),
                                           "[DEL]ete selected playlist items");
    deleteAction->setEnabled(false);

    deleteAction->setShortcut(QKeySequence(Qt::Key_Delete));
    auto savePlaylistDialog = new SavePlaylistDialog(controller, this);
    savePlaylistDialog->setEnabled(false);

    auto savePlaylistAction = toolBar->addAction(QIcon::fromTheme("document-save-all"),
                                                 "[CTRL-S]ave playlist",
                                                 [=]() { savePlaylistDialog->exec(); });

    savePlaylistAction->setShortcut(QKeySequence("CTRL+S"));
    savePlaylistAction->setEnabled(false);
    m_connectedActions.append(savePlaylistAction);

    toolBar->addSeparator();

    auto playbackSettingsDialog = new PlaybackSettingsDialog(controller, this);
    m_connectedWidgets.append(playbackSettingsDialog);
    playbackSettingsDialog->setEnabled(false);
    auto playbackSettingsAction = toolBar->addAction(QIcon::fromTheme("configure"),
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
    connect(databaseView, &QTreeView::doubleClicked, databaseModel, &ItemModel::onDoubleClicked);
    databaseView->setHeaderHidden(true);
    databaseView->setModel(databaseModel);
    databaseView->setEnabled(false);
    databaseView->setSelectionMode(QTreeView::ExtendedSelection);
    databaseView->setDragEnabled(true);
    databaseView->setItemDelegate(new PlaylistDelegate());
    m_connectedWidgets.append(databaseView);
    splitter->addWidget(databaseView);

    // https://stackoverflow.com/a/22198672/240515
    databaseView->setContextMenuPolicy(Qt::CustomContextMenu);
    auto deletePlaylistAction = new QAction("Delete", this);

    // currentIndex does appear to be the right way to do it...
    // https://www.qtcentre.org/threads/19919-Custom-context-menu-in-QTreeView?p=97803#post97803

    connect(deletePlaylistAction, &QAction::triggered, [=]() {
        auto item = static_cast<Item *>(databaseView->currentIndex().internalPointer());
        controller->deletePlaylist(item->text(0));
    });

    auto renamePlaylistAction = new QAction("Rename", this);
    connect(renamePlaylistAction, &QAction::triggered, [=]() {
        databaseView->edit(databaseView->currentIndex());
    });

    connect(databaseView, &QTreeView::customContextMenuRequested, [=](const QPoint &point) {
        auto index = databaseView->indexAt(point);
        if (!index.isValid()) {
            return;
        }

        auto item = static_cast<Item *>(index.internalPointer());
        if (item->parent()->text(0) == "Playlists") {
            QMenu::exec({deletePlaylistAction, renamePlaylistAction},
                        databaseView->viewport()->mapToGlobal(point));
        }
    });

    auto queueModel = new QueueModel(controller);

    auto queueView = new QTreeView();

    queueView->setAcceptDrops(true);
    queueView->setDropIndicatorShown(true);
    queueView->setDragEnabled(true);
    queueView->setModel(queueModel);
    queueView->setSelectionMode(QTreeView::ExtendedSelection);
    connect(queueView, &QTreeView::doubleClicked, queueModel, &ItemModel::onDoubleClicked);
    connect(queueModel, &ItemModel::columnResized, queueView, &QTreeView::resizeColumnToContents);
    splitter->addWidget(queueView);
    queueView->setEnabled(false);

    // This needs to be done after setting the model.
    // https://stackoverflow.com/a/30793898/240515
    connect(queueView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            queueModel,
            &QueueModel::onSelectionChanged);

    m_connectedWidgets.append(queueView);
    layout->addWidget(splitter);
    auto widget = new QWidget();
    widget->setLayout(layout);
    setCentralWidget(widget);

    auto status = statusBar();
    auto combinedTimeLabel = new QLabel();
    status->addPermanentWidget(combinedTimeLabel);
    connect(controller, &Controller::combinedTime, combinedTimeLabel, &QLabel::setText);

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
