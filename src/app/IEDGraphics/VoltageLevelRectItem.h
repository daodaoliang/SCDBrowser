/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      VoltageLevelRectItem.h
  * Author:         ţ����
  * Description:   ��������ͼ�еĵ�ѹ�ȼ������ڲ�����
  */ 
#pragma once
#include "qgraphicsitem.h"
#include "HmiGraphicsScene.h"

//��ѹ�ȼ�ͼԪ (��������ͼ);
class CVoltageLevelRectItem :
	public QGraphicsItem
{
public:
	CVoltageLevelRectItem(HmiGraphicsScene *hmiScene);
	~CVoltageLevelRectItem(void);

	void setVoltageLevel(int voltageLevel);	//���õ�ѹ�ȼ�;
	void setItemWidth(int width);			//����ͼԪ���;
	void setItemHeight(int height);			//����ͼԪ�߶�;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	QRectF boundingRect() const;
	bool save(QDataStream &stream);
	bool load(QDataStream &stream);

private:
	void setBrushColorByVoltageLevel();			//���ݵ�ѹ�ȼ����û�ˢ��ɫ;
	void setVoltageLevelNameByVoltageLevel();	//���ݵ�ѹ�ȼ�������ʾ����;

private:
	HmiGraphicsScene *mainScene;	//������;
	int m_VoltageLevel;				//��ѹ�ȼ�;
	int m_Width;					//ͼԪ���;
	int m_Height;					//ͼԪ�߶�;
	QFont textFont;					//��������;
	QColor m_BrushClr;				//��ˢ��ɫ;
	QColor m_TextClr;				//������ɫ;
	QString m_VoltageLevelName;		//��ʾ����;
};

