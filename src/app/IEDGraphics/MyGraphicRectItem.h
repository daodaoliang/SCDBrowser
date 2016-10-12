/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      MyGraphicRectItem.h
  * Author:         zhang-xch
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   ��������ͼ�����߿�
  */ 
#pragma once
#include "qgraphicsitem.h"
#include "HmiGraphicsScene.h"

//���߿�����ö��;
enum RectTypeEnum
{
	MuXianColor = 0,	//ĸ��;
	XianLuColor,		//��·;
	DuanLuQiColor,		//��·��;
	DianRongQiColor,	//����;
	DianKangColor,		//�翹;
	QiTaColor			//����;
};

//���������߿�ͼԪ;
class CMyGraphicRectItem : public QGraphicsItem
{
public:
	CMyGraphicRectItem(void);
	CMyGraphicRectItem(HmiGraphicsScene *hmiScene);
	~CMyGraphicRectItem(void);

	//���л�����;
	bool save(QDataStream &stream);
	bool load(QDataStream &stream);
	
	void setRectWidth(const int rectWidth);			//���ÿ��;
	void setRectHeight(const int rectHeight);		//���ø߶�;
	void setRectBGColor(RectTypeEnum colorType);	//��������������ɫ;
	void setRectName(const QString& rectName);		//���ñ���;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	QRectF boundingRect() const;

private:
	HmiGraphicsScene *mainScene;	//������;
	int m_rectWidth;				//���;
	int m_rectHeight;				//�߶�;
	RectTypeEnum m_rectBGColor;		//����;
	QString m_rectName;				//����;

	QPen rectPen;		//����;
	QFont rectFont;		//����;
	QBrush rectBrush;	//��ˢ;
};

