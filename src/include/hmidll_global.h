#ifndef HMIDLL_GLOBAL_H
#define HMIDLL_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef HMIDLL_LIB
# define HMIDLL_EXPORT Q_DECL_EXPORT
#else
# define HMIDLL_EXPORT Q_DECL_IMPORT
#endif

#endif // HMIDLL_GLOBAL_H
