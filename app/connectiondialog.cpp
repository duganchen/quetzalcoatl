#include "connectiondialog.h"
#include "controller.h"
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QIcon>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QTabWidget>
#include <QVBoxLayout>

ConnectionDialog::ConnectionDialog(Controller *controller, QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
    , m_controller(controller)
    , m_connectionState(Controller::ConnectionState::Disconnected)
{
    connect(m_controller, &Controller::connectionState, this, &ConnectionDialog::setConnectionState);

    setWindowIcon(QIcon(":/icons/network-connect.svg"));
    setWindowTitle("Connect to MPD");
    auto layout = new QVBoxLayout();
    auto connectionLayout = new QFormLayout();
    m_hostEdit = new QLineEdit();

    QSettings settings;
    if (settings.contains("host")) {
        m_hostEdit->setText(settings.value("host").toString());
    } else {
        m_hostEdit->setText("localhost");
    }

    connectionLayout->addRow("&Host:", m_hostEdit);
    m_portSpinner = new QSpinBox();
    m_portSpinner->setMinimum(0);
    m_portSpinner->setMaximum(65535);

    if (settings.contains("port")) {
        m_portSpinner->setValue(settings.value("port").toInt());
    } else {
        m_portSpinner->setValue(6600);
    }

    connectionLayout->addRow("&Port:", m_portSpinner);
    m_passwordCheck = new QCheckBox();

    if (settings.contains("needsPassword")) {
        m_passwordCheck->setChecked(settings.value("needsPassword").toBool());
    }

    connectionLayout->addRow("&Use Password:", m_passwordCheck);
    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setEnabled(false);
    m_passwordEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);

    if (settings.contains("password")) {
        m_passwordEdit->setText(settings.value("password").toString());
    }

    connect(m_passwordCheck, &QCheckBox::stateChanged, [=](int state) {
        m_passwordEdit->setEnabled(state != Qt::Unchecked);
        if (state == Qt::Unchecked) {
            m_passwordEdit->setText("");
        }
    });

    connectionLayout->addRow("Pass&word:", m_passwordEdit);
    layout->addLayout(connectionLayout);

    auto buttonBox = new QDialogButtonBox();
    m_connectButton = new QPushButton("Connect to &MPD");

    connect(m_hostEdit, &QLineEdit::textChanged, [=](const QString &text) {
        m_connectButton->setEnabled(!text.trimmed().isEmpty()
                                    && m_connectionState
                                           == Controller::ConnectionState::Disconnected);
    });

    buttonBox->addButton(m_connectButton, QDialogButtonBox::AcceptRole);
    connect(m_connectButton, &QPushButton::clicked, this, &ConnectionDialog::connectToMPD);
    auto cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);
    connect(cancelButton, &QPushButton::clicked, this, [=]() {
        m_errorLabel->setText("");
        QDialog::reject();
    });
    m_defaultsButton = buttonBox->addButton(QDialogButtonBox::RestoreDefaults);
    connect(m_defaultsButton, &QPushButton::clicked, [=]() {
        m_errorLabel->setText("");
        m_hostEdit->setText(m_controller->defaultHost());
        m_portSpinner->setValue(m_controller->defaultPort());
    });
    layout->addWidget(buttonBox);

    m_progressBar = new QProgressBar();
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(1);
    layout->addWidget(m_progressBar);
    m_errorLabel = new QLabel();
    layout->addWidget(m_errorLabel);
    connect(m_controller, &Controller::connectionErrorMessage, m_errorLabel, &QLabel::setText);
    setLayout(layout);
}

void ConnectionDialog::setConnectionState(Controller::ConnectionState connectionState)
{
    if (m_connectionState == connectionState) {
        return;
    }

    m_connectionState = connectionState;

    m_hostEdit->setEnabled(Controller::ConnectionState::Disconnected == connectionState);
    m_connectButton->setEnabled(Controller::ConnectionState::Disconnected == connectionState);
    m_portSpinner->setEnabled(Controller::ConnectionState::Disconnected == connectionState);
    m_passwordCheck->setEnabled(Controller::ConnectionState::Disconnected == connectionState);
    m_passwordEdit->setEnabled(Controller::ConnectionState::Disconnected == connectionState
                               && m_passwordCheck->isChecked());
    m_defaultsButton->setEnabled(Controller::ConnectionState::Disconnected == connectionState);

    if (Controller::ConnectionState::Connecting == connectionState) {
        m_progressBar->setMaximum(0);
    } else {
        m_progressBar->setMaximum(1);
    }

    if (Controller::ConnectionState::Connected == connectionState) {
        QSettings settings;
        settings.setValue("host", m_hostEdit->text());
        settings.setValue("port", m_portSpinner->value());
        QDialog::accept();
    }
}

void ConnectionDialog::connectToMPD()
{
    m_errorLabel->setText("");
    m_hostEdit->setText(m_hostEdit->text().trimmed());
    m_controller->connectToMPD(m_hostEdit->text(), m_portSpinner->value(), 200);
}
