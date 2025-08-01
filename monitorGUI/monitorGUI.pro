QT += core gui
QT += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QT += core gui network
QT += printsupport
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include($$PWD/../lib/pri/structs.pri)

SOURCES += \
    adddeviceform.cpp \
    configreader.cpp \
    deviceinfomodel.cpp \
    main.cpp \
    mainwindow.cpp \
    showhistoryform.cpp \
    showhistorymodel.cpp \
    tcpclient.cpp

HEADERS += \
    adddeviceform.h \
    configreader.h \
    deviceinfomodel.h \
    mainwindow.h \
    showhistoryform.h \
    showhistorymodel.h \
    tcpclient.h

FORMS += \
    adddeviceform.ui \
    mainwindow.ui \
    showhistoryform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
