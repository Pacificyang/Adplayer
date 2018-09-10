#ifndef ADPLAYER_H
#define ADPLAYER_H

#include <QObject>
#include <QFileInfo>
#include <QMap>
#include "playlist.h"
#include "downloadlist.h"
#include "deletelist.h"
#include <QtAV>
#include <QtAVWidgets>
#include "playerwindow.h"
#include "netmanager.h"
#include "qtredis.h"
#include "redis.h"


class Adplayer : public QObject
{
    Q_OBJECT
public:
    explicit Adplayer(QObject *parent = nullptr);
    void updateLists();

    void loopPlay();
    //Netmanager* getNet();
    Redis* getRedis();
signals:

public slots:
    void changeMedia();

private:
    Netmanager *m_net;
    Redis* m_redis;
    Playlist *playlist;
    Downloadlist *m_downloadlist;
    PlayerWindow *player;
    int m_video_index;
};

#endif // ADPLAYER_H
