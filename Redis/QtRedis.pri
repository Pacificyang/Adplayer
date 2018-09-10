DEPENDPATH += $$PWD
INCLUDEPATH += $$PWD

QT += network

SOURCES += \
    $$PWD/qtredis.cpp \
    $$PWD/reader.cpp

HEADERS += QtRedis \
    $$PWD/qtredis.h \
    $$PWD/reader.h
