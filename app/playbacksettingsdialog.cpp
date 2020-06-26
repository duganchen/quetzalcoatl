#include "playbacksettingsdialog.h"
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

PlaybackSettingsDialog::PlaybackSettingsDialog(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    setWindowIcon(QIcon(":/icons/configure.svg"));
    setWindowTitle("Playback Settings");
    auto layout = new QVBoxLayout();
    auto playbackLayout = new QFormLayout();
    auto fadeSpinner = new QSpinBox();
    fadeSpinner->setMinimum(0);
    fadeSpinner->setMaximum(20);
    playbackLayout->addRow("&Crossfade (in seconds)", fadeSpinner);
    auto volumeSpinner = new QSpinBox();
    volumeSpinner->setMinimum(0);
    volumeSpinner->setMaximum(100);
    playbackLayout->addRow("&Volume", volumeSpinner);
    layout->addLayout(playbackLayout);

    auto buttonBox = new QDialogButtonBox();
    auto okButton = buttonBox->addButton(QDialogButtonBox::Ok);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    auto cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    layout->addWidget(buttonBox);
    setLayout(layout);
}
