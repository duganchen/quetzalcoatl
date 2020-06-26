#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include <QObject>

class ConnectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConnectionDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

signals:
};

#endif // CONNECTIONDIALOG_H
