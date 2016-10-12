/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      IEDMasterItem.h
  * Author:         zhang-xch
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   信息流图中主IED的绘制
  */ 
#pragma once
#include "IedItem.h"

class CIEDMasterItem : public CBaseIEDItem
{
public:
	CIEDMasterItem(void);
	CIEDMasterItem(HmiGraphicsScene *hmiScene);
	~CIEDMasterItem(void);
	
	//根据ctl 返回图元高度;
	int getAdjustHeight();
	int getAdjustHeight(int cbCount);
   
	//序列化方法;
	bool save(QDataStream &stream);
	bool load(QDataStream &stream);
	
	//鼠标动作响应方法;
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	
	//被选中时高亮与取消高亮;
	void highLight();
	void alwaysHighLight();
	void highLightCancel();

	void setChangeType(ItemChangeType changeType);
	void setToolTipPos(int xPos, int yPos);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	QRectF boundingRect() const;


private:
	bool m_IsInRect;
	QRectF m_Rect;
	HmiGraphicsScene *mainScene;
};
