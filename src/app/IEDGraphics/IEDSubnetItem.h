/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      IEDSubnetItem.h
  * Author:         zhang-xch
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   ���������е�IEDͼԴ
  */ 
#pragma once
#include "IedItem.h"

class CIEDSubnetItem : public CBaseIEDItem
{
public:
	CIEDSubnetItem(void);
	CIEDSubnetItem(HmiGraphicsScene *hmiScene);
	~CIEDSubnetItem(void);

	//���л�����;
	bool	save(QDataStream &stream);
	bool	load(QDataStream &stream);

	//��궯����Ӧ����;
	void	mousePressEvent(QGraphicsSceneMouseEvent *event);
	void	mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void	mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void	hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void	hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	void    hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void    mouseMoveEvent (QGraphicsSceneMouseEvent * event);

	//��ѡ��ʱ������ȡ������;
	void	highLight();
	void	alwaysHighLight();
	void	highLightCancel();

	void	setIEDName(QString iedName);
	void	setChangeType(ItemChangeType changeType);		//���ñ������;
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

