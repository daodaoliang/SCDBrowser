#ifndef HMIDLL_H
#define HMIDLL_H
#include <QtGui>
#include "hmidll_global.h"

class HMIDLL_EXPORT HmiDll
{
public:
	HmiDll();
	~HmiDll();

private:

};
const int LENS_EXTENT = 10;
QRect circle_bounds(const QPointF &center, qreal radius, qreal compensation);

#endif // HMIDLL_H
