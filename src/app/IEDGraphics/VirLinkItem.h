/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      VirLinkItem.h
  * Description:   ��Ϣ��ͼ�����·ͼ�ߵĻ���
  */ 
#pragma once
#include "MyGraphItem.h"
#include <QPointF>
#include "CtrlBlockItem.h"
#include "HmiGraphicsScene.h"

class CVirLinkItem : public CMyGraphItem
{
public:
	CVirLinkItem(HmiGraphicsScene *hmiScene);
	CVirLinkItem(QPointF ptStart, QPointF ptEnd,bool arrowBoth=false);
	~CVirLinkItem(void);

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
	//void    setTypeLine(CtrlBlockSigType type); //��������������

	void	setStartPt(QPointF pt);
	void	setEndPt(QPointF pt);
	void    setTypeLine(int type); //��������������  //  0��goose input 1: sv 2: ���·ͼ;
	void	setSrcItemID(QString srcItemID);	//������ʼͼԪID;
	void	setDestItemID(QString destItemID);	//����Ŀ��ͼԪID;
	void	setChangeType(ItemChangeType changeType);	//���ñ������;
	void	setMasterSlaveIEDKey(QString masterIEDKey, QString slaveIEDKey);
	void	setCtrlBlockKey(QString cbKey);
    void    setRoateangle();   // ������ת�Ƕ� ���ܻ�����ʵ��;
	void	setToolTipPos(int xPos, int yPos);
	void	setDiffInfo(QString diffInfo);
	void    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	QRectF  boundingRect()const;
	
	QString getMasterIEDKey();
	QString getSlaveIEDKey();
	QString getCtrlBlockKey();

private:
	QString GetVLToolTip();

private:
	HmiGraphicsScene *mainScene;
	QRectF m_Rect;
	QString m_SrcItemID, m_DestItemID;
	QString m_CtrlBlockKey;

	QPointF m_PtStart, m_PtEnd;
	bool m_IsInRect;
	bool m_arrowBoth;
	int  m_type;
};
