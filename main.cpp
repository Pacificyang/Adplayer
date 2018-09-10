//#include "widget.h"
#include <QApplication>
#include <QtSingleApplication>
#include "adplayer.h"

int main(int argc, char *argv[])
{
    QtSingleApplication::setApplicationName("Adplayer");
    QtSingleApplication::setOrganizationName("QtProject");
    QtSingleApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QtSingleApplication app(argc, argv);

    if (app.isRunning())
    {
        return EXIT_SUCCESS;
    }



    Adplayer *adplayer = new Adplayer();
    //listmanager->loopPlay(listmanager->playlist);



//    QtAV::Widgets::registerRenderers();
//    //QApplication a(argc, argv);
//    PlayerWindow player;
//    //player.setWindowState(Qt::WindowFullScreen);
//    player.show();
//    player.resize(800, 600);
//    player.playDefault("C:/develop/v1080.mp4");
    return app.exec();
}
