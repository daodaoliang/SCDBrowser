/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      PhyLinkVirLinkItem.h
  * Author:         nzm
  * Version:        V1.0
  * Description:   ������ͼ�к��ߵĻ��Ʒ�ʽ
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

	void setSrcItemID(QString srcItemID);	//������ʼͼԪID;
	//void setDestItemID(QString destItemID);	//����Ŀ��ͼԪID;
	void appendDestItemID(const QString& destItemID);
	void setDestDesc(QString destDesc);		//���öԶ��ź�����;
	void setChangeType(ItemChangeType changeType);	//���ñ������;
	void setportPosVec(QVector<int> &posVec);	//��������˿�λ��;

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
