/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      IEDSlaveItem.h
  * Author:         zhang-xch
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   ��Ϣ��ͼ�д�IED�Ļ���
  */ 
#pragma once
#include "IedItem.h"

class CIEDSlaveItem : public CBaseIEDItem
{
public:
	CIEDSlaveItem(void);
	CIEDSlaveItem(HmiGraphicsScene *hmiScene);
	~CIEDSlaveItem(void);

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

	void    setIedSlaveType(int type);
	void	setChangeType(ItemChangeType changeType);		//���ñ������;
	void	setSideOfMaster(bool isRight);					//�������������IED����һ��,���false,�Ҳ�true;
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

