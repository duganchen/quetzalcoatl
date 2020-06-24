#ifndef SAVEPLAYLISTDIALOG_H
#define SAVEPLAYLISTDIALOG_H

#include <QDialog>

class SavePlaylistDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SavePlaylistDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

signals:
};

#endif // SAVEPLAYLISTDIALOG_H
