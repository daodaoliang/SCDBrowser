/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      IEDSlaveItem.h
  * Author:         zhang-xch
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   信息流图中从IED的绘制
  */ 
#pragma once
#include "IedItem.h"

class CIEDSlaveItem : public CBaseIEDItem
{
public:
	CIEDSlaveItem(void);
	CIEDSlaveItem(HmiGraphicsScene *hmiScene);
	~CIEDSlaveItem(void);

	//序列化方法;
	bool	save(QDataStream &stream);
	bool	load(QDataStream &stream);

	//鼠标动作响应方法;
	void	mousePressEvent(QGraphicsSceneMouseEvent *event);
	void	mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void	mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void	hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void	hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	void    hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void    mouseMoveEvent (QGraphicsSceneMouseEvent * event);
	
	//被选中时高亮与取消高亮;
	void	highLight();
	void	alwaysHighLight();
	void	highLightCancel();

	void    setIedSlaveType(int type);
	void	setChangeType(ItemChangeType changeType);		//设置变更类型;
	void	setSideOfMaster(bool isRight);					//设置相对于在主IED的那一侧,左侧false,右侧true;
	void	setToolTipPos(int xPos, int yPos);
	//void	setDiffInfo(QString diffInfo);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	QRectF boundingRect() const;

private:
	QRectF m_Rect;
	bool hendInRect;
	bool manPartInRect;
	bool m_IsRightSide;
	QColor m_TitleReguClr;
	QColor m_TitleHLClr;
	QColor m_ReguClr;
	QColor m_HLClr;
	int   m_IedSlaveType;
	QPointF mousePos;
	HmiGraphicsScene *mainScene;
};

