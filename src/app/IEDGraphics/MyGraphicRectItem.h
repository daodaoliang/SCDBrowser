/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      MyGraphicRectItem.h
  * Author:         zhang-xch
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   绘制子网图中虚线框
  */ 
#pragma once
#include "qgraphicsitem.h"
#include "HmiGraphicsScene.h"

//虚线框类型枚举;
enum RectTypeEnum
{
	MuXianColor = 0,	//母线;
	XianLuColor,		//线路;
	DuanLuQiColor,		//断路器;
	DianRongQiColor,	//电容;
	DianKangColor,		//电抗;
	QiTaColor			//其他;
};

//子网中虚线框图元;
class CMyGraphicRectItem : public QGraphicsItem
{
public:
	CMyGraphicRectItem(void);
	CMyGraphicRectItem(HmiGraphicsScene *hmiScene);
	~CMyGraphicRectItem(void);

	//序列化方法;
	bool save(QDataStream &stream);
	bool load(QDataStream &stream);
	
	void setRectWidth(const int rectWidth);			//设置宽度;
	void setRectHeight(const int rectHeight);		//设置高度;
	void setRectBGColor(RectTypeEnum colorType);	//根据类型设置颜色;
	void setRectName(const QString& rectName);		//设置标题;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	QRectF boundingRect() const;

private:
	HmiGraphicsScene *mainScene;	//主场景;
	int m_rectWidth;				//宽度;
	int m_rectHeight;				//高度;
	RectTypeEnum m_rectBGColor;		//类型;
	QString m_rectName;				//标题;

	QPen rectPen;		//画笔;
	QFont rectFont;		//字体;
	QBrush rectBrush;	//画刷;
};

