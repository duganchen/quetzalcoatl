#include "connectiondialog.h"
#include "controller.h"
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QIcon>
#include <QPushButton>
#include <QSpinBox>
#include <QTabWidget>
#include <QVBoxLayout>

ConnectionDialog::ConnectionDialog(Controller *controller, QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    Q_UNUSED(controller)

    setWindowIcon(QIcon(":/icons/network-connect.svg"));
    setWindowTitle("Connect to MPD");
    auto layout = new QVBoxLayout();
    auto connectionLayout = new QFormLayout();
    m_hostEdit = new QLineEdit();
    connectionLayout->addRow("&Host:", m_hostEdit);
    m_portSpinner = new QSpinBox();
    m_portSpinner->setMinimum(0);
    m_portSpinner->setMaximum(65535);
    connectionLayout->addRow("&Port:", m_portSpinner);
    m_passwordCheck = new QCheckBox();
    connectionLayout->addRow("&Use Password:", m_passwordCheck);
    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    connectionLayout->addRow("Pass&word:", m_passwordEdit);
    layout->addLayout(connectionLayout);

    m_progressBar = new QProgressBar();
    layout->addWidget(m_progressBar);
    m_errorLabel = new QLabel();
    layout->addWidget(m_errorLabel);

    auto buttonBox = new QDialogButtonBox();
    auto okButton = buttonBox->addButton(QDialogButtonBox::Ok);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    auto cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    buttonBox->addButton(QDialogButtonBox::RestoreDefaults);
    layout->addWidget(buttonBox);
    setLayout(layout);
}

void ConnectionDialog::setConnectionState(Controller::ConnectionState) {}
