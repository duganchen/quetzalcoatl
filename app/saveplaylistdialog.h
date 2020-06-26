#ifndef SAVEPLAYLISTDIALOG_H
#define SAVEPLAYLISTDIALOG_H

#include "controller.h"
#include <QDialog>

class SavePlaylistDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SavePlaylistDialog(Controller *,
                                QWidget *parent = nullptr,
                                Qt::WindowFlags f = Qt::WindowFlags());

signals:
private:
    Controller *m_controller;
};

#endif // SAVEPLAYLISTDIALOG_H
