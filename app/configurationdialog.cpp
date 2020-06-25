#include "configurationdialog.h"
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QIcon>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

ConfigurationDialog::ConfigurationDialog(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    setWindowIcon(QIcon(":/icons/configure.svg"));
    setWindowTitle("Configure");
    auto layout = new QVBoxLayout();
    auto tabs = new QTabWidget();

    auto connectionWidget = new QWidget();
    auto connectionLayout = new QFormLayout();
    auto hostEdit = new QLineEdit();
    connectionLayout->addRow("&Host:", hostEdit);
    auto portSpinner = new QSpinBox();
    portSpinner->setMinimum(0);
    portSpinner->setMaximum(65535);
    connectionWidget->setLayout(connectionLayout);
    connectionLayout->addRow("&Port:", portSpinner);
    connectionLayout->addRow("&Use Password:", new QCheckBox());
    auto passwordEdit = new QLineEdit();
    passwordEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
    connectionLayout->addRow("Pass&word:", passwordEdit);
    tabs->addTab(connectionWidget, "Connection");

    auto playbackWidget = new QWidget();
    auto playbackLayout = new QFormLayout();
    auto fadeSpinner = new QSpinBox();
    fadeSpinner->setMinimum(0);
    fadeSpinner->setMaximum(20);
    playbackLayout->addRow("&Crossfade (in seconds)", fadeSpinner);
    auto volumeSpinner = new QSpinBox();
    volumeSpinner->setMinimum(0);
    volumeSpinner->setMaximum(100);
    playbackLayout->addRow("&Volume", volumeSpinner);
    playbackWidget->setLayout(playbackLayout);
    tabs->addTab(playbackWidget, "Playback");
    layout->addWidget(tabs);

    auto buttonBox = new QDialogButtonBox();
    auto okButton = buttonBox->addButton(QDialogButtonBox::Ok);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    auto cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    buttonBox->addButton(QDialogButtonBox::RestoreDefaults);
    layout->addWidget(buttonBox);
    setLayout(layout);
}
