#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include "controller.h"
#include <QDialog>

class QCheckBox;
class QLabel;
class QLineEdit;
class QProgressBar;
class QSpinBox;

class ConnectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConnectionDialog(Controller *,
                              QWidget *parent = nullptr,
                              Qt::WindowFlags f = Qt::WindowFlags());

    bool isProtected() const;
    QString password() const;
public slots:
    void setConnectionState(Controller::ConnectionState);
    void connectToMPD();

    void restoreDefaults();

private:
    QCheckBox *m_passwordCheck{};
    QLineEdit *m_hostEdit{};
    QSpinBox *m_portSpinner{};
    QLineEdit *m_passwordEdit{};
    QProgressBar *m_progressBar{};
    QLabel *m_errorLabel{};
    Controller *m_controller{};
    QPushButton *m_connectButton{};
    QPushButton *m_defaultsButton{};

    Controller::ConnectionState m_connectionState{};
signals:
};

#endif // CONNECTIONDIALOG_H
