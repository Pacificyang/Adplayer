#include "playlist.h"
#include <QDir>
#include <QFileInfo>
#include <QMap>
#include <QApplication>

Playlist::Playlist(QObject *parent) : QObject(parent)
{

    filepath = QDir::toNativeSeparators(
                qApp->applicationDirPath()
                + QDir::separator()
                +"Playresource"
                + QDir::separator());

    updatePlayList();

}

void Playlist::updatePlayList(){
    QDir dir(filepath);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList filelist = dir.entryInfoList();
    QStringList playlist;
    for(int j = 0 ; j < filelist.count() ; j++)
    {
        playlist.append(filelist.at(j).fileName());
        qDebug("playlist.append:%s",filelist.at(j).fileName());
    }

    m_playlist = &playlist;
    qDebug("playlist size:%d",(*m_playlist).count());

}


QStringList Playlist::getPlayList(){

    QDir dir(filepath);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList filelist = dir.entryInfoList();
    QStringList playlist;
    for(int j = 0 ; j < filelist.count() ; j++)
    {
        playlist.append(filelist.at(j).fileName());
        //qDebug("playlist.append:%s",filelist.at(j).fileName());
    }

    return playlist;
    //qDebug("playlist size:%d",(*m_playlist).count());

}

QString Playlist::getFilePath(){
    return filepath;
}
