#include "adplayer.h"

Adplayer::Adplayer(QObject *parent) : QObject(parent)
{

    m_net = new Netmanager();
    m_net->Getplayresource();
    //init 3 list
    //updateLists();

    QtAV::Widgets::registerRenderers();
    player = new PlayerWindow() ;

    playlist = new Playlist();
    //loop playback playlist
    loopPlay();
    m_video_index = 0;

    m_redis = new Redis;
    m_redis->startSubscribe();

    QObject::connect(m_redis,SIGNAL(vlaueschange()),m_net,SLOT(Getplayresource()));

    //connect(player, SIGNAL(changeMedia()),this, SLOT(changeMedia()));
    connect(m_net, SIGNAL(filedowncomplete()),m_net, SLOT(updateLists()));

}

void Adplayer::updateLists()
{
    //net->Getplayresource();
    qDebug() << "download completed, update playlist";
    playlist->updatePlayList();
    if(m_video_index == -1){
        qDebug() << "no playwindow in play, start new window";
        loopPlay();
    }
}

//Netmanager* Adplayer::getNet(){
//    return m_net;
//}

Redis* Adplayer::getRedis(){
    return m_redis;
}

void Adplayer::changeMedia(){
//    video_index ++;
//    if(video_index >= playlist->getPlayList().count()){
//        video_index = 0;
//    }
//    player->playDefault(playlist->getFilePath() + playlist->getPlayList().at(video_index));
}

void Adplayer::loopPlay(){

 //   qDebug() << "video number:"<<(*stringplaylist).count();
//    while(playlist->getPlayList().count()==0){
//        qDebug() << "no video for display";
//        //video_index=-1;
//        //return;
//        QThread::sleep(5);
//    }


    player->setWindowState(Qt::WindowFullScreen);
    player->show();
    //player->resize(800, 600);
    //player->playDefault("C:/develop/v1080.mp4");

    if(((playlist->getPlayList())).count()==0){
        qDebug() << "no video for display";
        m_video_index=-1;
        return;

    }

    qDebug() << "before m_player->playList(m_playlist)";
    player->playList(playlist);

}
