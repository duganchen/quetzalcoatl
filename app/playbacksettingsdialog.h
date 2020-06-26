#ifndef PLAYBACKSETTINGSDIALOG_H
#define PLAYBACKSETTINGSDIALOG_H

#include "controller.h"
#include <QDialog>
#include <QObject>

class PlaybackSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PlaybackSettingsDialog(Controller *,
                                    QWidget *parent = nullptr,
                                    Qt::WindowFlags f = Qt::WindowFlags());

signals:
private:
    Controller *m_controller;
};

#endif // PLAYBACKSETTINGSDIALOG_H
