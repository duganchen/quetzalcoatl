#ifndef PLAYBACKSETTINGSDIALOG_H
#define PLAYBACKSETTINGSDIALOG_H

#include "controller.h"
#include <QDialog>
#include <QObject>
#include <QSpinBox>

class PlaybackSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PlaybackSettingsDialog(Controller *,
                                    QWidget *parent = nullptr,
                                    Qt::WindowFlags f = Qt::WindowFlags());
public slots:
    void setCrossfade(int);
    void setVolume(int);

signals:
private:
    Controller *m_controller;
    QSpinBox *m_fadeSpinner;
    QSpinBox *m_volumeSpinner;
};

#endif // PLAYBACKSETTINGSDIALOG_H
