#ifndef SEC_MEAS_DEFINE_H
#define SEC_MEAS_DEFINE_H

#include <qvector>
#include <qstring>

struct QSecurityMeasuresStrategy
{
	QString  strategyName;
	QVector<bool> thisIEDStrategy;
	QVector<bool> relateIEDStrategy;
};	

#endif