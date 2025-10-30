#ifndef TT3_HELP_GLOBAL_H
#define TT3_HELP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TT3_HELP_LIBRARY)
#define TT3_HELP_EXPORT Q_DECL_EXPORT
#else
#define TT3_HELP_EXPORT Q_DECL_IMPORT
#endif

#endif // TT3_HELP_GLOBAL_H
