#ifndef LISTMANAGER_H
#define LISTMANAGER_H

#include <QObject>
#include "playlist.h"
#include "downloadlist.h"
#include "deletelist.h"
#include "Netmanager.h"

class Listmanager : public QObject
{
    Q_OBJECT
public:
    explicit Listmanager(QObject *parent = nullptr);

    void Listmanager::loopPlay(Playlist *playlist);
signals:

public slots:

private:
    Netmanager *net;
    Playlist *playlist;
    Downloadlist *downloadlist;

};

#endif // LISTMANAGER_H
