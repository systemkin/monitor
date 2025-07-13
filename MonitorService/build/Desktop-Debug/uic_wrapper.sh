#!/bin/sh
LD_LIBRARY_PATH=/home/alexej/Desktop/qt-everywhere-src-5.15.2/qtbase/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}
export LD_LIBRARY_PATH
QT_PLUGIN_PATH=/home/alexej/Desktop/qt-everywhere-src-5.15.2/qtbase/plugins${QT_PLUGIN_PATH:+:$QT_PLUGIN_PATH}
export QT_PLUGIN_PATH
exec /home/alexej/Desktop/qt-everywhere-src-5.15.2/qtbase/bin/uic "$@"
