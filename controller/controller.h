#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Controller_global.h"
#include "item.h"
#include <mpd/client.h>
#include <QObject>
#include <QSocketNotifier>

class CONTROLLER_EXPORT Controller : public QObject
{
    Q_OBJECT

public:
    explicit Controller(QObject *parent = nullptr);
    ~Controller();

    enum class ConnectionState { Disconnected, Connecting, Connected };
    Q_ENUM(ConnectionState)

    QString defaultHost();
    unsigned defaultPort();

    QSocketNotifier *notifier() const;
    mpd_connection *mpd() const;

    QVector<QString> listTags(mpd_tag_type);

    // The parameter is a sequence of tag_type, tag_value pairs.
    QVector<mpd_song *> searchSongs(const QVector<QPair<mpd_tag_type, QString>> &);

    void moveSongs(const QVector<QPair<unsigned, unsigned>> &, unsigned);

    QVector<QString> searchTags(mpd_tag_type, const QVector<QPair<mpd_tag_type, QString>>);

    QVector<mpd_playlist *> listPlaylists();

    QVector<mpd_entity *> listPlaylist(mpd_playlist *);

    QVector<mpd_entity *> listSongs();

    QVector<mpd_entity *> listDir(mpd_entity *);

    void queueUris(const QVector<QString> &, int);
    void playSongEntity(mpd_entity *);
    void addAndPlaySong(QString);
    void playAlbum(const QVector<QString> &, QString uri);

    void fetchMore();

    void setCombinedTime(unsigned);

    void renamePlaylist(QString, QString);
    void savePlaylist(QString);

    void deletePlaylist(QString);

    void deleteSongIds(const QVector<unsigned> &);

public slots:
    void connectToMPD(QString, int, int);

    void updateStatus();

    void stop();
    void play();
    void pause();
    void skipForth();
    void skipBack();

    void random(bool);
    void repeat(bool);

signals:
    void errorMessage(QString);

    void serverErrorMessage(QString);

    void connectionState(Controller::ConnectionState connectionState);

    void beginMPDCommand();
    void endMPDCommand();

    void unrecoverableError();

    void connectionErrorMessage(QString);

    void sliderMax(int);
    void sliderValue(int);

    void statusMessage(QString, int);

    void shuffled(bool);
    void repeating(bool);
    void crossfade(int);
    void volume(int);

    void queueChanged(const QVector<Item *> &);

    void songId(int);

    void combinedTime(QString);

    // There's a technical reason for this: you can't have a signal that
    // carries a pointer to a libmpdclient pointer (like mpd_playlist *).
    void playlistItems(const QVector<Item *> &);
    void playlistNames(const QVector<QString> &);

private:
    void handleIdle(mpd_idle);
    void createMPD(QString, int, int);

    void enableIdle();
    void disableIdle();

    void pollForStatus();

    void checkStoredPlaylists();

    QString m_defaultHost;
    unsigned m_defaultPort;
    unsigned m_defaultTimeout;

    mpd_connection *m_connection;
    QSocketNotifier *m_notifier;

    unsigned m_queueVersion;

    ConnectionState m_connectionState;

    mpd_state m_mpdPlayerState;

    QVector<mpd_playlist *> listPlaylistsImpl();

private slots:
    void handleActivation();
};

Q_DECLARE_METATYPE(Controller::ConnectionState);

#endif // CONTROLLER_H
