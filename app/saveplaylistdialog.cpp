#include "saveplaylistdialog.h"
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

SavePlaylistDialog::SavePlaylistDialog(Controller *controller, QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
    , m_controller(controller)
{
    setWindowTitle("Save Playlist");
    auto layout = new QVBoxLayout();
    auto formLayout = new QFormLayout();
    formLayout->addRow("&Name", new QLineEdit());
    layout->addLayout(formLayout);
    auto buttonBox = new QDialogButtonBox();
    auto okButton = buttonBox->addButton(QDialogButtonBox::Ok);
    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(buttonBox);
    auto cancelButton = buttonBox->addButton(QDialogButtonBox::Cancel);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    layout->addWidget(buttonBox);
    setLayout(layout);
}
