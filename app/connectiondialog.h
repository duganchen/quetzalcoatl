#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include "controller.h"
#include <QCheckBox>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QProgressBar>
#include <QSpinBox>

class ConnectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConnectionDialog(Controller *,
                              QWidget *parent = nullptr,
                              Qt::WindowFlags f = Qt::WindowFlags());
public slots:
    void setConnectionState(Controller::ConnectionState);
    void accept() override;

private:
    QCheckBox *m_passwordCheck;
    QLineEdit *m_hostEdit;
    QSpinBox *m_portSpinner;
    QLineEdit *m_passwordEdit;
    QProgressBar *m_progressBar;
    QLabel *m_errorLabel;
signals:
};

#endif // CONNECTIONDIALOG_H
