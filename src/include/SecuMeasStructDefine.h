#ifndef SECUMEASSTRUCTDEFINE_H
#define SECUMEASSTRUCTDEFINE_H

#include <QString>
#include <QVector>
#include <QDateTime>

//��������ȫ��ʩ
//���ܣ���װ��ȫ��ʩ������
class SecurityMeasures
{
public:
	QString  panel;
    QString  IED;
    QString  trip;
    bool     state;
};

//ͼ����Ϣ
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