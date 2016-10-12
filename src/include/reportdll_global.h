#ifndef REPORTDLL_GLOBAL_H
#define REPORTDLL_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef REPORTDLL_LIB
# define REPORTDLL_EXPORT Q_DECL_EXPORT
#else
# define REPORTDLL_EXPORT Q_DECL_IMPORT
#endif

#endif // REPORTDLL_GLOBAL_H
