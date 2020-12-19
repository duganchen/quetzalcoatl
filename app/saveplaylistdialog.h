#ifndef SAVEPLAYLISTDIALOG_H
#define SAVEPLAYLISTDIALOG_H

#include <QDialog>

class QLineEdit;
class QValidator;

class SavePlaylistDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SavePlaylistDialog(QValidator *,
                                QWidget *parent = nullptr,
                                Qt::WindowFlags f = Qt::WindowFlags());

    QString name() const;

    void clear();
signals:
private:
    QLineEdit *m_nameEdit;
};

#endif // SAVEPLAYLISTDIALOG_H
