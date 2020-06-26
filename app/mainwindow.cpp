#include "mainwindow.h"
#include "connectiondialog.h"
#include "connectionstate.h"
#include "databasemodel.h"
#include "playbacksettingsdialog.h"
#include "playlistmodel.h"
#include "saveplaylistdialog.h"
#include <QKeySequence>
#include <QProgressBar>
#include <QPushButton>
#include <QSlider>
#include <QSplitter>
#include <QStatusBar>
#include <QToolBar>
#include <QTreeView>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Quetzalcoatl"));
    setWindowIcon(QIcon(":/icons/multimedia-player.svg"));

    auto toolBar = addToolBar("ToolBar");
    toolBar->setMovable(false);
    toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    auto connectionDialog = new ConnectionDialog(this);

    toolBar->addAction(QIcon(":/icons/network-connect.svg"), "Connect to MPD", [=]() {
        connectionDialog->exec();
    });
    toolBar->addSeparator();

    auto stopAction = toolBar->addAction(QIcon(":/icons/media-playback-stop"), "Stop");
    stopAction->setShortcut(QKeySequence(Qt::Key::Key_MediaStop));
    m_connectedActions.append(stopAction);

    auto playAction = toolBar->addAction(QIcon(":/icons/media-playback-start"), "Play");
    playAction->setShortcut(QKeySequence(Qt::Key::Key_MediaPlay));
    m_connectedActions.append(playAction);

    auto pauseAction = toolBar->addAction(QIcon(":/icons/media-playback-pause"), "Pause");
    pauseAction->setShortcut(QKeySequence(Qt::Key::Key_MediaPause));
    m_connectedActions.append(pauseAction);

    auto skipBackAction = toolBar->addAction(QIcon(":/icons/media-skip-backward"), "Previous");
    skipBackAction->setShortcut(QKeySequence(Qt::Key::Key_MediaPrevious));
    m_connectedActions.append(skipBackAction);

    auto skipForthAction = toolBar->addAction(QIcon(":/icons/media-skip-forward"), "Next");
    skipForthAction->setShortcut(QKeySequence(Qt::Key::Key_MediaNext));
    m_connectedActions.append(skipForthAction);

    toolBar->addSeparator();

    auto shuffleAction = toolBar->addAction(QIcon(":/icons/media-playlist-shuffle"), "Shuffle");
    shuffleAction->setCheckable(true);
    m_connectedActions.append(shuffleAction);

    auto repeatAction = toolBar->addAction(QIcon(":/icons/media-playlist-repeat"), "Repeat");
    repeatAction->setCheckable(true);
    m_connectedActions.append(repeatAction);

    toolBar->addSeparator();

    auto deleteAction = toolBar->addAction(QIcon(":/icons/list-remove"),
                                           "[DEL]ete selected playlist items");
    deleteAction->setEnabled(false);

    deleteAction->setShortcut(QKeySequence(Qt::Key_Delete));
    auto savePlaylistDialog = new SavePlaylistDialog(this);
    savePlaylistDialog->setEnabled(false);
    auto savePlaylistAction = toolBar->addAction(QIcon(":/icons/document-save-all"),
                                                 "[CTRL-S]ave playlist",
                                                 [=]() { savePlaylistDialog->exec(); });
    savePlaylistAction->setShortcut(QKeySequence("CTRL+S"));
    savePlaylistAction->setEnabled(false);

    toolBar->addSeparator();

    auto playbackSettingsDialog = new PlaybackSettingsDialog();
    m_connectedWidgets.append(playbackSettingsDialog);
    auto playbackSettingsAction = toolBar->addAction(QIcon(":/icons/configure.svg"),
                                                     "Playback Settings",
                                                     [=]() { playbackSettingsDialog->exec(); });
    m_connectedActions.append(playbackSettingsAction);

    auto layout = new QVBoxLayout();
    auto slider = new QSlider(Qt::Horizontal);
    slider->setTracking(false);
    layout->addWidget(slider);
    m_connectedWidgets.append(slider);

    auto splitter = new QSplitter();
    m_connectedWidgets.append(splitter);

    auto dbRootItem = new Item(QIcon(), "");
    dbRootItem->append(new Item(QIcon(":/icons/folder-favorites.svg"), "Playlists"));
    auto artistsItem = new Item(QIcon(":/icons/server-database.svg"), "Artists");
    artistsItem->append(new Item(QIcon(":/icons/server-database.svg"), "Johnny Cash"));
    dbRootItem->append(artistsItem);
    dbRootItem->append(new Item(QIcon(":/icons/server-database.svg"), "Albums"));
    dbRootItem->append(new Item(QIcon(":/icons/server-database.svg"), "Compilations"));
    dbRootItem->append(new Item(QIcon(":/icons/server-database.svg"), "Songs"));
    dbRootItem->append(new Item(QIcon(":/icons/server-database.svg"), "Genres"));
    dbRootItem->append(new Item(QIcon(":/icons/server-database.svg"), "Composers"));
    dbRootItem->append(new Item(QIcon(":/icons/drive-harddisk"), "/"));
    auto databaseModel = new DatabaseModel(dbRootItem);
    auto databaseView = new QTreeView();
    databaseView->setHeaderHidden(true);
    databaseView->setModel(databaseModel);
    splitter->addWidget(databaseView);

    auto playlistRoot = new Item(QIcon(), "");
    auto playlistModel = new PlaylistModel(playlistRoot);
    auto playlistView = new QTreeView();
    playlistView->setModel(playlistModel);
    splitter->addWidget(playlistView);
    layout->addWidget(splitter);
    auto widget = new QWidget();
    widget->setLayout(layout);
    setCentralWidget(widget);

    auto status = statusBar();
    status->addWidget(new QLabel());

    setConnectionState(Controller::ConnectionState::Disconnected);
}

MainWindow::~MainWindow() {}

void MainWindow::setConnectionState(Controller::ConnectionState connectionState)
{
    for (auto widget : m_connectedWidgets) {
        widget->setEnabled(Controller::ConnectionState::Connected == connectionState);
    }

    for (auto action : m_connectedActions) {
        action->setEnabled(Controller::ConnectionState::Connected == connectionState);
    }
}
