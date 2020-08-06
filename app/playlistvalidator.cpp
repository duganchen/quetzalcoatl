#include "playlistvalidator.h"
#include <QDebug>

PlaylistValidator::PlaylistValidator(QObject *parent)
    : QValidator(parent)
{}

QValidator::State PlaylistValidator::validate(QString &input, int &pos) const
{
    input = input.trimmed();

    if (input.isEmpty()) {
        return Invalid;
    }

    if (input.contains("\\")) {
        return Invalid;
    }

    if (input.contains("/")) {
        return Invalid;
    }

    if (input.startsWith(".")) {
        return Invalid;
    }

    for (auto playlist : m_playlists) {
        if (input == playlist) {
            return Invalid;
        }
    }

    return Acceptable;
}

void PlaylistValidator::setPlaylists(const QVector<QString> &playlists)
{
    m_playlists.clear();
    for (auto playlist : playlists) {
        m_playlists << playlist;
    }
}