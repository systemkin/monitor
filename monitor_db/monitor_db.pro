QT -= gui
QT += sql
TEMPLATE = lib
DEFINES += MONITOR_DB_LIBRARY
DEFINES += MONITOR_DB_BUILD
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
include(../structs.pri)
SOURCES += \
    db_repository.cpp \
    monitor_db.cpp

HEADERS += \
    db_repository.h \
    monitor_db_global.h \
    monitor_db.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
