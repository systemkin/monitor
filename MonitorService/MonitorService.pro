QT = core
QT += xml
QT += core gui widgets network
CONFIG += c++17 cmdline
QT += sql

INCLUDEPATH += $$PWD/../lib/include
win32 {
    LIBS += -L$$PWD/../lib/debug -lmonitor_db
}
unix {
    LIBS += $$PWD/../lib/debug/libmonitor_db.so.1.0.0
    LIBS += $$PWD/../lib/debug/librefresherLib.so.1.0.0
    LIBS += $$PWD/../lib/debug/libserverLib.so.1.0.0
    QMAKE_LFLAGS += -Wl,-rpath,/home/alexej/monitor/lib/debug

}
include(../structs.pri)
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        configreader.cpp \
        main.cpp \
        server.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    configreader.h \
    server.h
