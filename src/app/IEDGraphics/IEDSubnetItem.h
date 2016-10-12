/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      IEDSubnetItem.h
  * Author:         zhang-xch
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   绘制子网中的IED图源
  */ 
#pragma once
#include "IedItem.h"

class CIEDSubnetItem : public CBaseIEDItem
{
public:
	CIEDSubnetItem(void);
	CIEDSubnetItem(HmiGraphicsScene *hmiScene);
	~CIEDSubnetItem(void);

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

	void	setIEDName(QString iedName);
	void	setChangeType(ItemChangeType changeType);		//设置变更类型;
	void	setToolTipPos(int xPos, int yPos);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	QRectF boundingRect() const;

private:
	QRectF m_Rect;
	bool hendInRect;
	bool manPartInRect;
	QColor m_TitleReguClr;
	QColor m_TitleHLClr;
	QColor m_ReguClr;
	QColor m_HLClr;
	QColor m_vtClr;
	QPointF mousePos;
	HmiGraphicsScene *mainScene;
};

