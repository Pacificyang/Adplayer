#include "listmanager.h"
#include "netmanager.h"

Listmanager::Listmanager(QObject *parent) : QObject(parent)
{

    net = new Netmanager();
    //init 3 list
    updateLists();

    //loop playback playlist
    //loopPlay(playlist);
}

void Listmanager::updateLists()
{
    net->Getplayresource();
}

void Listmanager::loopPlay(Playlist *playlist){

    QtAV::Widgets::registerRenderers();
    //QApplication a(argc, argv);
    PlayerWindow player;
    //player.setWindowState(Qt::WindowFullScreen);
    player.show();
    player.resize(800, 600);
    player.playDefault("C:/develop/v1080.mp4");
    return app.exec();

}
