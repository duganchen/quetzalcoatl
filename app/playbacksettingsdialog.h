#ifndef PLAYBACKSETTINGSDIALOG_H
#define PLAYBACKSETTINGSDIALOG_H

#include <QDialog>
#include <QObject>

class PlaybackSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PlaybackSettingsDialog(QWidget *parent = nullptr,
                                    Qt::WindowFlags f = Qt::WindowFlags());

signals:
};

#endif // PLAYBACKSETTINGSDIALOG_H
