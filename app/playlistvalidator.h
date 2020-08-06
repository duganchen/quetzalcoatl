#ifndef PLAYLISTVALIDATOR_H
#define PLAYLISTVALIDATOR_H

#include <QValidator>

class PlaylistValidator : public QValidator
{
    Q_OBJECT
public:
    PlaylistValidator(QObject *parent = nullptr);
    QValidator::State validate(QString &, int &) const override;

public slots:
    void setPlaylists(const QVector<QString> &);

private:
    QVector<QString> m_playlists;
};

#endif