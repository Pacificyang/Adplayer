#-------------------------------------------------
#
# Project created by QtCreator 2018-08-28T19:14:13
#
#-------------------------------------------------

QT       += core gui avwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Adplayer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#DESTDIR = $$PWD/bin
INCLUDEPATH += $$PWD/include \
               $$PWD/include/QtAV \
               $$PWD/include/QtAVWidgets

LIBS += $$PWD/lib/libQt5AV.a -lQt5AV \
        $$PWD/lib/libQt5AVWidgets.a -lQt5AVWidgets \
        $$PWD/lib/libQtAV1.a -lQtAV1 \
        $$PWD/lib/libQtAVWidgets1.a -lQtAVWidgets1

SOURCES += \
        main.cpp \
        widget.cpp \
    playerwindow.cpp \
    redis.cpp \
    adplayer.cpp

HEADERS += \
        widget.h \
    playerwindow.h \
    redis.h \
    adplayer.h

FORMS += \
        widget.ui

include(Redis/QtRedis.pri)
include(Net/Net.pri)
include(qtsingleapplication/qtsingleapplication.pri)
include(listmanager/listmanager.pri)

DISTFILES +=
