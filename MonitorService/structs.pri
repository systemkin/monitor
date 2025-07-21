
COMMON_ROOT = $$PWD/..
INCLUDEPATH += $$COMMON_ROOT/lib/pri
HEADERS += \
    $$COMMON_ROOT/lib/pri/deviceInfo.h \
    $$COMMON_ROOT/lib/pri/historyItem.h \
    $$COMMON_ROOT/lib/pri/states.h \
    $$COMMON_ROOT/lib/pri/device.h

    
SOURCES += \
    $$COMMON_ROOT/lib/pri/device.cpp
