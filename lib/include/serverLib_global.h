#ifndef SERVERLIB_GLOBAL_H
#define SERVERLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SERVERLIB_LIBRARY)
#define SERVERLIB_EXPORT Q_DECL_EXPORT
#else
#define SERVERLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // SERVERLIB_GLOBAL_H
