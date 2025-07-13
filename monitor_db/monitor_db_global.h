#ifndef MONITOR_DB_GLOBAL_H
#define MONITOR_DB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MONITOR_DB_LIBRARY)
#define MONITOR_DB_EXPORT Q_DECL_EXPORT
#else
#define MONITOR_DB_EXPORT Q_DECL_IMPORT
#endif

#endif // MONITOR_DB_GLOBAL_H
