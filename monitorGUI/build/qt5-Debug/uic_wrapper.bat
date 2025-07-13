@echo off
SetLocal EnableDelayedExpansion
(set PATH=C:\1\qt-everywhere-src-5.15.2\qtbase\bin;!PATH!)
if defined QT_PLUGIN_PATH (
    set QT_PLUGIN_PATH=C:\1\qt-everywhere-src-5.15.2\qtbase\plugins;!QT_PLUGIN_PATH!
) else (
    set QT_PLUGIN_PATH=C:\1\qt-everywhere-src-5.15.2\qtbase\plugins
)
C:\1\qt-everywhere-src-5.15.2\qtbase\bin\uic.exe %*
EndLocal
