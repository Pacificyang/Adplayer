DEFINES += _CRT_SECURE_NO_WARNINGS
#DEFINES +=_UNICODE  UNICODE
DEFINES += WIN32

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/include

QT += concurrent

HEADERS += \
    $$PWD/netmanager.h

SOURCES += \
    $$PWD/netmanager.cpp
