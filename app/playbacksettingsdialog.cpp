#include "playbacksettingsdialog.h"
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QIcon>
#include <QLineEdit>
#include <QPushButton>

#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>

PlaybackSettingsDialog::PlaybackSettingsDialog(Controller *controller,
                                               QWidget *parent,
                                               Qt::WindowFlags f)
    : QDialog(parent, f)
    , m_controller(controller)
{
    setWindowIcon(QIcon(":/icons/configure.svg"));
    setWindowTitle("Playback Settings");
    auto layout = new QVBoxLayout();
    auto playbackLayout = new QFormLayout();
    m_fadeSpinner = new QSpinBox();
    m_fadeSpinner->setMinimum(0);
    m_fadeSpinner->setMaximum(20);
    playbackLayout->addRow("&Crossfade (in seconds)", m_fadeSpinner);
    m_volumeSpinner = new QSpinBox();
    m_volumeSpinner->setMinimum(0);
    m_volumeSpinner->setMaximum(100);
    playbackLayout->addRow("&Volume", m_volumeSpinner);
    layout->addLayout(playbackLayout);

    auto buttonBox = new QDialogButtonBox();
    auto okButton = buttonBox->addButton(QDialogButtonBox::Ok);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    auto cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    layout->addWidget(buttonBox);
    setLayout(layout);
}

void PlaybackSettingsDialog::setCrossfade(int value)
{
    m_fadeSpinner->setValue(value);
}

void PlaybackSettingsDialog::setVolume(int value)
{
    if (value == -1) {
        m_volumeSpinner->setValue(0);
        m_volumeSpinner->setEnabled(false);
    } else {
        m_volumeSpinner->setEnabled(true);
        m_volumeSpinner->setValue(value);
    }
}
