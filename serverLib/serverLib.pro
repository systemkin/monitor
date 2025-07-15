QT -= gui
QT += xml
QT += core gui widgets network
QT += sql

TEMPLATE = lib
DEFINES += SERVERLIB_LIBRARY

CONFIG += c++17
INCLUDEPATH += $$PWD/../lib/include
win32 {
    LIBS += -L$$PWD/../lib/debug -lmonitor_db
}
unix {
    LIBS += $$PWD/../lib/debug/libmonitor_db.so.1.0.0
    QMAKE_LFLAGS += -Wl,-rpath,/home/alexej/monitor/lib/debug

}
include(../structs.pri)
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    serverlib.cpp

HEADERS += \
    serverLib_global.h \
    serverlib.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
