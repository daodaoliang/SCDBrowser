#ifndef GraphicNameGenerator_H
#define GraphicNameGenerator_H

#include "GraphicDefine.h"
#include <QString>
#include <QVector>
#include "SCLNameTranslator.h"

//ͼ�����ƹ�������
//����ͼ:�������� + ����ͼ
//��Ϣ��ͼ��װ������ + ��Ϣ��ͼ
//������ͼ��װ������ + ������ͼ
//������ͼ��װ������ + ������ͼ

class GraphicNameGenerator
{
public:
	
	QString FromMaintanceIEDInfoToGraphicName(QString mainDev,QString relateDev,int graphicType)
	{
		SCLNameTranslator translator;		
		QString graphicName;
		if(graphicType==GRAPHIC_TYPE_NO_SUBNET||
			graphicType==GRAPHIC_TYPE_NO_INFOMATION_FLOW||
			graphicType==GRAPHIC_TYPE_NO_VIRTUAL_SIGNAL)
		{			
			if(translator.GetParentName(mainDev.toLocal8Bit().constData())=="����")
			{

				graphicName = translator.GetNameFromKey(mainDev.toLocal8Bit().constData()).c_str() +
					FromGraphicTypeNoToGraphicTypeName(graphicType);
			}
			else{
				graphicName = translator.GetParentName(mainDev.toLocal8Bit().constData()).c_str() + (QString)translator.GetNameFromKey(mainDev.toLocal8Bit().constData()).c_str() +
					FromGraphicTypeNoToGraphicTypeName(graphicType);
			}
		}
		else
		{
			if(translator.GetParentName(mainDev.toLocal8Bit().constData())=="����")
			{
				graphicName = (QString)(translator.GetNameFromKey(mainDev.toLocal8Bit().constData()).c_str()) + "-" + (QString)(translator.GetNameFromKey(relateDev.toLocal8Bit().constData()).c_str()) +
					FromGraphicTypeNoToGraphicTypeName(graphicType);
			}
			else
			{
				graphicName = translator.GetParentName(mainDev.toLocal8Bit().constData()).c_str() + (QString)translator.GetNameFromKey(mainDev.toLocal8Bit().constData()).c_str() + "-" +
					translator.GetParentName(relateDev.toLocal8Bit().constData()).c_str() + translator.GetNameFromKey(relateDev.toLocal8Bit().constData()).c_str() +
					FromGraphicTypeNoToGraphicTypeName(graphicType);
			}
		}
		return graphicName;
	};

	QString FromGraphicTypeNoToGraphicTypeName(int graphicTypeNo)
	{
		switch (graphicTypeNo)
		{
		case GRAPHIC_TYPE_NO_SUBNET:
			return GRAPHIC_TYPE_NAME_SUBNET;
		case GRAPHIC_TYPE_NO_INFOMATION_FLOW:
			return GRAPHIC_TYPE_NAME_INFOMATION_FLOW;
		case GRAPHIC_TYPE_NO_VIRTUAL_LINK:
			return GRAPHIC_TYPE_NAME_VIRTUAL_LINK;
		case GRAPHIC_TYPE_NO_VIRTUAL_SIGNAL:
			return GRAPHIC_TYPE_NAME_VIRTUAL_SIGNAL;			
		default:
			return "";
		}
	};

	int FromGraphicTypeNameToGraphicTypeNo(QString graphicTypeName)
	{
		if(graphicTypeName==GRAPHIC_TYPE_NAME_SUBNET)
		{
			return GRAPHIC_TYPE_NO_SUBNET;
		}
		else if(graphicTypeName==GRAPHIC_TYPE_NAME_INFOMATION_FLOW)
		{
			return GRAPHIC_TYPE_NO_INFOMATION_FLOW;

		}
		else if(graphicTypeName==GRAPHIC_TYPE_NAME_VIRTUAL_LINK)
		{
			return GRAPHIC_TYPE_NO_VIRTUAL_LINK;

		}
		else if(graphicTypeName==GRAPHIC_TYPE_NAME_VIRTUAL_SIGNAL)
		{
			return GRAPHIC_TYPE_NO_VIRTUAL_SIGNAL;

		}
	};
};

#endif