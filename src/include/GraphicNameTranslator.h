#ifndef GraphicNameTranslator_H
#define GraphicNameTranslator_H

#include "SecuMeasStructDefine.h"
#include "GraphicDefine.h"
#include <QString>
#include <QVector>

//ͼ�����ƹ�������
//�ۺ�ͼ:IEDNAME�ۺ�����ͼ
//ͨ��ͼ��IEDNAME1-IEDNAME2ͨ��ͼ
//����ͼ��IEDNAME����ͼ

//struct GraphicInfo
//{
//	QString maintanceIEDKey;
//	QString relateIEDKey;
//	int     graphicType;
//};

class GraphicNameTranslator
{
public:
	//GraphicInfo FromGraphicNameToMaintanceIEDInfos(QString graphicName)
	//{
	//	GraphicInfo tempGraphicInfo;
	//	return tempGraphicInfo;
	//};
	
	QString FromMaintanceIEDInfoToGraphicName(IEDInfo maintanceIED,IEDInfo relateIEDKey,int graphicType)
	{
		QString graphicName;
		if(graphicType==GRAPHIC_TYPE_NO_COMPREHENSIVE||
			graphicType==GRAPHIC_TYPE_NO_SIGNAL)
		{
			graphicName = maintanceIED.panel + maintanceIED.IED +
				FromGraphicTypeNoToGraphicTypeName(graphicType);
		}
		else
		{
			graphicName = maintanceIED.panel + maintanceIED.IED + "-" +
				relateIEDKey.panel + relateIEDKey.IED +
				FromGraphicTypeNoToGraphicTypeName(graphicType);
		}
		return graphicName;
	};

	int FromGraphicTypeNameToGraphicTypeNo(QString graphicTypeName)
	{
		if(graphicTypeName==GRAPHIC_TYPE_NAME_COMPREHENSIVE)
		{
			return GRAPHIC_TYPE_NO_COMPREHENSIVE;
		}
		else if(graphicTypeName==GRAPHIC_TYPE_NAME_COMMUNICATION)
		{
			return GRAPHIC_TYPE_NO_COMMUNICATION;
		}
		else if(graphicTypeName==GRAPHIC_TYPE_NAME_SIGNAL)
		{
			return GRAPHIC_TYPE_NO_SIGNAL;
		}
		else
		{
			return -1;
		}
	};

	QString FromGraphicTypeNoToGraphicTypeName(int graphicTypeNo)
	{
		switch (graphicTypeNo)
		{
		case GRAPHIC_TYPE_NO_COMPREHENSIVE:
			return GRAPHIC_TYPE_NAME_COMPREHENSIVE;
		case GRAPHIC_TYPE_NO_COMMUNICATION:
			return GRAPHIC_TYPE_NAME_COMMUNICATION;
		case GRAPHIC_TYPE_NO_SIGNAL:
			return GRAPHIC_TYPE_NAME_SIGNAL;
		default:
			return "";
		}
	};
};

#endif