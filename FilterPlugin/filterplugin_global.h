#ifndef FILTERPLUGIN_GLOBAL_H
#define FILTERPLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FILTERPLUGIN_LIBRARY)
#  define FILTERPLUGINSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FILTERPLUGINSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FILTERPLUGIN_GLOBAL_H
