/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      PhyPortItem.h
  * Author:         zhang-xch
  * Version:        V1.0
  * Description:   ���ƶ�����ͼ������˿ڷ������
  */ 
#pragma once
#include "MyGraphItem.h"
#include "HmiGraphicsScene.h"

class CPhyPortItem : public CMyGraphItem
{
public:
	CPhyPortItem(HmiGraphicsScene *hmiScene);
	~CPhyPortItem(void);

	//���л�����;
	bool	save(QDataStream &stream);
	bool	load(QDataStream &stream);

	//��궯����Ӧ����;
	void	mousePressEvent(QGraphicsSceneMouseEvent *event);
	void	mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void	mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void	hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void	hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

	//��ѡ��ʱ������ȡ������;
	void	highLight();
	void	alwaysHighLight();
	void	highLightCancel();

	void	setWidth(const int widthNum);
	void	setHeight(const int heightNum);

	QString getPortName();
	void	setPortName(QString portName);	//��������˿���;
	QString getPortType();
	void	setPortType(QString portType);
	void	setChangeType(ItemChangeType changeType);	//���ñ������;
	void	setToolTipPos(int xPos, int yPos);

	void	setVerticalSize(const int vSize);

	void	appendAssoID(QString assoID);
	QVector<QString> getAssoIDs();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	QRectF boundingRect() const;
	QPainterPath shape() const;

private:
	HmiGraphicsScene *mainScene;
	bool m_IsInRect;
	int m_Width;
	int m_Height;
	QString m_PortName;
	QString m_PortType;
	int m_vSize;
	ItemChangeType m_ChangeType;
	QVector<QString> m_AssoItemIDs;
};

