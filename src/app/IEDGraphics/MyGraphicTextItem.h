/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      MyGraphicTextItem.h
  * Author:         nzm
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   标题栏中子网的信息的绘制及跳转控制
  */ 
#pragma once
#include "qgraphicsitem.h"
#include "HmiGraphicsScene.h"

//信息流图中标题栏的子网显示字图元;
class CMyGraphicTextItem : public QGraphicsItem
{
public:
	CMyGraphicTextItem(void);
	CMyGraphicTextItem(HmiGraphicsScene *hmiScene);
	~CMyGraphicTextItem(void);

	//序列化方法;
	bool save(QDataStream &stream);
	bool load(QDataStream &stream);

	//鼠标动作响应方法;
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

	void setDisplayText(const QString& disText);	//设置显示文字;
	void setSubnetKey(const QString& subnetKey);	//设置子网Key;
	void setFontSize(const int fontSize);			//设置字号;

	int  getDisplayTextLength();					//获取显示字符串宽度;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	QRectF boundingRect() const;

private:
	HmiGraphicsScene* mainScene;	//主场景;
	QString m_Text;			//显示文字;
	QString m_SubnetKey;	//子网Key;
	int		m_FontSize;		//字号;
	QFont textFont;			//字体;
	QPen  textPen;			//画笔;
};

