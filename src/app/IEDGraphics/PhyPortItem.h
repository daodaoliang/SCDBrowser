/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      PhyPortItem.h
  * Author:         zhang-xch
  * Version:        V1.0
  * Description:   绘制端子排图的物理端口方框和线
  */ 
#pragma once
#include "MyGraphItem.h"
#include "HmiGraphicsScene.h"

class CPhyPortItem : public CMyGraphItem
{
public:
	CPhyPortItem(HmiGraphicsScene *hmiScene);
	~CPhyPortItem(void);

	//序列化方法;
	bool	save(QDataStream &stream);
	bool	load(QDataStream &stream);

	//鼠标动作响应方法;
	void	mousePressEvent(QGraphicsSceneMouseEvent *event);
	void	mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void	mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
	void	hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void	hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

	//被选中时高亮与取消高亮;
	void	highLight();
	void	alwaysHighLight();
	void	highLightCancel();

	void	setWidth(const int widthNum);
	void	setHeight(const int heightNum);

	QString getPortName();
	void	setPortName(QString portName);	//设置物理端口名;
	QString getPortType();
	void	setPortType(QString portType);
	void	setChangeType(ItemChangeType changeType);	//设置变更类型;
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

