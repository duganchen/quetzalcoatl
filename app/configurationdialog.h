#ifndef CONFIGURATIONDIALOG_H
#define CONFIGURATIONDIALOG_H

#include <QDialog>
#include <QObject>

class ConfigurationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigurationDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

signals:
};

#endif // CONFIGURATIONDIALOG_H
