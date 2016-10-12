#ifndef SECUMEASSTRUCTDEFINE_H
#define SECUMEASSTRUCTDEFINE_H

#include <QString>
#include <QVector>
#include <QDateTime>

//类名：安全措施
//功能：封装安全措施的数据
class SecurityMeasures
{
public:
	QString  panel;
    QString  IED;
    QString  trip;
    bool     state;
};

//图形信息
class IEDGraphic
{
	QString graphic;
};

class IEDInfo
{
public:
	QString panel;
	QString IED;
};

#endif