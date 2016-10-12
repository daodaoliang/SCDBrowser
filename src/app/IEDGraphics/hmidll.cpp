#include "hmidll.h"

HmiDll::HmiDll()
{

}

HmiDll::~HmiDll()
{

}

QRect circle_bounds(const QPointF &center, qreal radius, qreal compensation)
{
	return QRect(qRound(center.x() - radius - compensation),
		qRound(center.y() - radius - compensation),
		qRound((radius + compensation) * 2),
		qRound((radius + compensation) * 2));

}