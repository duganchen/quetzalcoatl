#include "connectiondialog.h"
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QSpinBox>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

ConnectionDialog::ConnectionDialog(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    setWindowIcon(QIcon(":/icons/network-connect.svg"));
    setWindowTitle("Connect to MPD");
    auto layout = new QVBoxLayout();
    auto connectionLayout = new QFormLayout();
    auto hostEdit = new QLineEdit();
    connectionLayout->addRow("&Host:", hostEdit);
    auto portSpinner = new QSpinBox();
    portSpinner->setMinimum(0);
    portSpinner->setMaximum(65535);
    connectionLayout->addRow("&Port:", portSpinner);
    connectionLayout->addRow("&Use Password:", new QCheckBox());
    auto passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    connectionLayout->addRow("Pass&word:", passwordEdit);
    layout->addLayout(connectionLayout);

    layout->addWidget(new QProgressBar());
    layout->addWidget(new QLabel());

    auto buttonBox = new QDialogButtonBox();
    auto okButton = buttonBox->addButton(QDialogButtonBox::Ok);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    auto cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    buttonBox->addButton(QDialogButtonBox::RestoreDefaults);
    layout->addWidget(buttonBox);
    setLayout(layout);
}
