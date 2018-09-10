#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>

class Playlist : public QObject
{
    Q_OBJECT
public:
    explicit Playlist(QObject *parent = nullptr);

    QStringList getPlayList();
    void updatePlayList();
    QString getFilePath();
signals:

public slots:

private:
    QStringList * m_playlist;
    QString filepath;
};

#endif // PLAYLIST_H
