#include "mainwindow.h"
#include "configurationdialog.h"
#include "connectionstate.h"
#include "databasemodel.h"
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

    auto cfgDlg = new ConfigurationDialog(this);

    toolBar->addAction(QIcon(":/icons/network-connect.svg"), "Connect to MPD");
    toolBar->addSeparator();

    auto stopAction = toolBar->addAction(QIcon(":/icons/media-playback-stop"), "Stop");
    stopAction->setShortcut(QKeySequence(Qt::Key::Key_MediaStop));
    auto playAction = toolBar->addAction(QIcon(":/icons/media-playback-start"), "Play");
    playAction->setShortcut(QKeySequence(Qt::Key::Key_MediaPlay));
    auto pauseAction = toolBar->addAction(QIcon(":/icons/media-playback-pause"), "Pause");
    pauseAction->setShortcut(QKeySequence(Qt::Key::Key_MediaPause));
    auto skipBackAction = toolBar->addAction(QIcon(":/icons/media-skip-backward"), "Previous");
    skipBackAction->setShortcut(QKeySequence(Qt::Key::Key_MediaPrevious));
    auto skipForthAction = toolBar->addAction(QIcon(":/icons/media-skip-forward"), "Next");
    skipForthAction->setShortcut(QKeySequence(Qt::Key::Key_MediaNext));

    toolBar->addSeparator();

    auto shuffleAction = toolBar->addAction(QIcon(":/icons/media-playlist-shuffle"), "Shuffle");
    shuffleAction->setCheckable(true);
    auto repeatAction = toolBar->addAction(QIcon(":/icons/media-playlist-repeat"), "Repeat");
    repeatAction->setCheckable(true);

    toolBar->addSeparator();

    auto deleteAction = toolBar->addAction(QIcon(":/icons/list-remove"),
                                           "[DEL]ete selected playlist items");

    deleteAction->setShortcut(QKeySequence(Qt::Key_Delete));
    auto savePlaylistDialog = new SavePlaylistDialog(this);
    auto savePlaylistAction = toolBar->addAction(QIcon(":/icons/document-save-all"),
                                                 "[CTRL-S]ave playlist",
                                                 [=]() { savePlaylistDialog->exec(); });

    toolBar->addSeparator();
    toolBar->addAction(QIcon(":/icons/configure.svg"), "Playback Settings", [=]() {
        cfgDlg->exec();
    });

    savePlaylistAction->setShortcut(QKeySequence("CTRL+S"));

    auto layout = new QVBoxLayout();
    auto slider = new QSlider(Qt::Horizontal);
    slider->setTracking(false);
    layout->addWidget(slider);
    auto splitter = new QSplitter();
    auto databaseModel = new DatabaseModel();
    auto databaseView = new QTreeView();
    databaseView->setHeaderHidden(true);
    databaseView->setModel(databaseModel);
    splitter->addWidget(databaseView);
    splitter->addWidget(new QTreeView());
    layout->addWidget(splitter);
    auto widget = new QWidget();
    widget->setLayout(layout);
    setCentralWidget(widget);

    auto status = statusBar();
    status->addWidget(new QProgressBar());
    status->addWidget(new QLabel());
}

MainWindow::~MainWindow() {}
