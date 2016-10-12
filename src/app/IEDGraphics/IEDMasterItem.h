/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      IEDMasterItem.h
  * Author:         zhang-xch
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   ��Ϣ��ͼ����IED�Ļ���
  */ 
#pragma once
#include "IedItem.h"

class CIEDMasterItem : public CBaseIEDItem
{
public:
	CIEDMasterItem(void);
	CIEDMasterItem(HmiGraphicsScene *hmiScene);
	~CIEDMasterItem(void);
	
	//����ctl ����ͼԪ�߶�;
	int getAdjustHeight();
	int getAdjustHeight(int cbCount);
   
	//���л�����;
	bool save(QDataStream &stream);
	bool load(QDataStream &stream);
	
	//��궯����Ӧ����;
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
	
	//��ѡ��ʱ������ȡ������;
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
