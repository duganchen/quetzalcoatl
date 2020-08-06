#include "mainwindow.h"
#include "controller.h"
#include "databasemodel.h"
#include "iconnames.h"
#include "playbacksettingsdialog.h"
#include "playlistdelegate.h"
#include "playlistvalidator.h"
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
    , m_seekPosition(0)
{
    setWindowTitle(tr("Quetzalcoatl"));

    onPaletteChanged();

    setWindowIcon(QIcon::fromTheme(IconNames::Player));

    auto controller = new Controller(this);
    connect(controller, &Controller::connectionState, this, &MainWindow::setConnectionState);

    auto toolBar = addToolBar("ToolBar");
    toolBar->setMovable(false);
    toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    m_connectionDialog = new ConnectionDialog(controller, this);

    auto connectAction = toolBar->addAction(QIcon::fromTheme(IconNames::Connect),
                                            "Connect to MPD",
                                            [=]() { m_connectionDialog->exec(); });
    toolBar->addSeparator();

    auto stopAction = toolBar->addAction(QIcon::fromTheme(IconNames::Stop), "Stop");
    stopAction->setShortcut(QKeySequence(Qt::Key::Key_MediaStop));
    stopAction->setEnabled(false);
    connect(stopAction, &QAction::triggered, controller, &Controller::stop);
    m_connectedActions.append(stopAction);

    auto playAction = toolBar->addAction(QIcon::fromTheme(IconNames::Start), "Play");
    playAction->setShortcut(QKeySequence(Qt::Key::Key_MediaPlay));
    playAction->setEnabled(false);
    connect(playAction, &QAction::triggered, controller, &Controller::play);
    m_connectedActions.append(playAction);

    auto pauseAction = toolBar->addAction(QIcon::fromTheme(IconNames::Pause), "Pause");
    pauseAction->setShortcut(QKeySequence(Qt::Key::Key_MediaPause));
    pauseAction->setEnabled(false);
    connect(pauseAction, &QAction::triggered, controller, &Controller::pause);
    m_connectedActions.append(pauseAction);

    auto skipBackAction = toolBar->addAction(QIcon::fromTheme(IconNames::SkipBackward), "Previous");
    skipBackAction->setShortcut(QKeySequence(Qt::Key::Key_MediaPrevious));
    skipBackAction->setEnabled(false);
    connect(skipBackAction, &QAction::triggered, controller, &Controller::skipBack);
    m_connectedActions.append(skipBackAction);

    auto skipForthAction = toolBar->addAction(QIcon::fromTheme(IconNames::SkipForward), "Next");
    skipForthAction->setShortcut(QKeySequence(Qt::Key::Key_MediaNext));
    skipForthAction->setEnabled(false);
    connect(skipForthAction, &QAction::triggered, controller, &Controller::skipForth);
    m_connectedActions.append(skipForthAction);

    toolBar->addSeparator();

    auto randomAction = toolBar->addAction(QIcon::fromTheme(IconNames::Shuffle), "Random");
    randomAction->setCheckable(true);
    randomAction->setEnabled(false);
    connect(randomAction, &QAction::toggled, controller, &Controller::random);
    m_connectedActions.append(randomAction);

    auto repeatAction = toolBar->addAction(QIcon::fromTheme(IconNames::Repeat), "Repeat");
    repeatAction->setCheckable(true);
    repeatAction->setEnabled(false);
    connect(repeatAction, &QAction::toggled, controller, &Controller::repeat);

    m_connectedActions.append(repeatAction);

    toolBar->addSeparator();

    auto deleteAction = toolBar->addAction(QIcon::fromTheme(IconNames::Remove),
                                           "[DEL]ete selected songs from the queue.");
    deleteAction->setEnabled(false);

    auto playlistValidator = new PlaylistValidator(this);
    connect(controller,
            &Controller::playlistNames,
            playlistValidator,
            &PlaylistValidator::setPlaylists);

    deleteAction->setShortcut(QKeySequence(Qt::Key_Delete));
    auto savePlaylistDialog = new SavePlaylistDialog(playlistValidator, this);
    savePlaylistDialog->setEnabled(false);
    m_connectedWidgets.append(savePlaylistDialog);

    auto savePlaylistAction = toolBar->addAction(QIcon::fromTheme(IconNames::SaveAll),
                                                 "[CTRL-S]ave queue to playlist",
                                                 [=]() {
                                                     savePlaylistDialog->clear();
                                                     savePlaylistDialog->exec();
                                                 });

    savePlaylistAction->setShortcut(QKeySequence("CTRL+S"));
    savePlaylistAction->setEnabled(false);
    m_connectedActions.append(savePlaylistAction);

    connect(savePlaylistDialog, &SavePlaylistDialog::accepted, [=]() {
        auto name = savePlaylistDialog->name();
        if (!name.isEmpty()) {
            controller->savePlaylist(name);
        }
        savePlaylistDialog->clear();
    });

    toolBar->addSeparator();

    auto playbackSettingsDialog = new PlaybackSettingsDialog(controller, this);
    m_connectedWidgets.append(playbackSettingsDialog);
    playbackSettingsDialog->setEnabled(false);
    auto playbackSettingsAction = toolBar->addAction(QIcon::fromTheme(IconNames::Configure),
                                                     "Playback Settings",
                                                     [=]() { playbackSettingsDialog->exec(); });
    playbackSettingsAction->setEnabled(false);
    m_connectedActions.append(playbackSettingsAction);

    auto layout = new QVBoxLayout();

    // Is the slider being dragged?
    bool isDragging = false;

    m_slider = new QSlider(Qt::Horizontal);
    m_slider->setTracking(false);

    connect(controller, &Controller::sliderMax, [=](int value) {
        if (!m_slider->isSliderDown()) {
            m_slider->setMaximum(value);
        }
    });
    connect(controller, &Controller::sliderValue, [=](int value) {
        if (!m_slider->isSliderDown()) {
            m_slider->setValue(value);
        }
    });

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

    auto playlistDelegate = new PlaylistDelegate(playlistValidator, this);
    databaseView->setItemDelegate(playlistDelegate);
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

    connect(m_slider, &QSlider::sliderMoved, [=](int value) { m_seekPosition = value; });
    connect(m_slider, &QSlider::sliderReleased, [=]() {
        controller->seek(queueModel->songId(), m_seekPosition);
    });

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

    connect(deleteAction, &QAction::triggered, [=]() {
        queueModel->deleteIndexes(queueView->selectionModel()->selectedIndexes());
    });

    // This needs to be done after setting the model.
    // https://stackoverflow.com/a/30793898/240515
    connect(queueView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            queueModel,
            &QueueModel::onSelectionChanged);

    connect(queueView->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            [=](const QItemSelection &selected, const QItemSelection &deselected) {
                deleteAction->setEnabled(selected.count());
            });

    connect(queueModel, &QueueModel::hasSongs, savePlaylistAction, &QAction::setEnabled);
    connect(queueModel, &QueueModel::hasSongs, savePlaylistDialog, &SavePlaylistDialog::setEnabled);

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

    // These are, say, when you rename a playlist to a name that already exists.
    connect(controller, &Controller::serverErrorMessage, [=](QString message) {
        QMessageBox::critical(this, "Server error", message);
    });

    connect(controller, &Controller::repeating, repeatAction, &QAction::setChecked);
    connect(controller, &Controller::shuffled, randomAction, &QAction::setChecked);
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

void MainWindow::changeEvent(QEvent *event)
{
    onPaletteChanged();
}

void MainWindow::onPaletteChanged()
{
#ifndef Q_OS_LINUX

    constexpr int OSX_LIGHT_MODE = 236;
    constexpr int OSX_DARK_MODE = 50;
    constexpr int THRESHOLD = OSX_LIGHT_MODE / 2 - OSX_DARK_MODE / 2;

    if (palette().color(QPalette::Active, QPalette::Window).lightness() < THRESHOLD) {
        QIcon::setThemeName(IconNames::Dark);
    } else {
        QIcon::setThemeName(IconNames::Light);
    }
#endif
}