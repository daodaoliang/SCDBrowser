/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      PhyLinkVirLinkItem.h
  * Author:         nzm
  * Version:        V1.0
  * Description:   端子排图中横线的绘制方式
  */ 
#pragma once
#include "MyGraphItem.h"
#include "HmiGraphicsScene.h"
#include <QPointF>
class CPhyLinkVirLinkItem : public CMyGraphItem
{
public:
	CPhyLinkVirLinkItem(HmiGraphicsScene *hmiScene);
	CPhyLinkVirLinkItem(HmiGraphicsScene *hmiScene, QPointF ptStart, QPointF ptEnd);
	~CPhyLinkVirLinkItem(void);

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

	void setSrcItemID(QString srcItemID);	//设置起始图元ID;
	//void setDestItemID(QString destItemID);	//设置目标图元ID;
	void appendDestItemID(const QString& destItemID);
	void setDestDesc(QString destDesc);		//设置对端信号描述;
	void setChangeType(ItemChangeType changeType);	//设置变更类型;
	void setportPosVec(QVector<int> &posVec);	//设置物理端口位置;

	int	getRealWidth();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

	QRectF boundingRect() const;

private:
	HmiGraphicsScene *mainScene;
	QRectF m_Rect;
	//int m_vSize;
	int m_realWidth;
	QString m_SrcItemID;
	QVector<QString> m_DestItemIDs;
	QString m_DestDesc;
	QPointF m_PtStart, m_PtEnd;
	ItemChangeType m_ChangeType;
	bool m_IsInRect;
	QFont textFont;
	QVector<int> m_portPosVec;

    // add by zxc
public:
    void SetDestIsInSameInterval(bool iInterval);
private:
    bool m_bInSameInterval;
};
