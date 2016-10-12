/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      VirLinkItem.h
  * Description:   信息流图和虚回路图线的绘制
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
	//void    setTypeLine(CtrlBlockSigType type); //设置连接线种类

	void	setStartPt(QPointF pt);
	void	setEndPt(QPointF pt);
	void    setTypeLine(int type); //设置连接线种类  //  0：goose input 1: sv 2: 虚回路图;
	void	setSrcItemID(QString srcItemID);	//设置起始图元ID;
	void	setDestItemID(QString destItemID);	//设置目标图元ID;
	void	setChangeType(ItemChangeType changeType);	//设置变更类型;
	void	setMasterSlaveIEDKey(QString masterIEDKey, QString slaveIEDKey);
	void	setCtrlBlockKey(QString cbKey);
    void    setRoateangle();   // 设置旋转角度 不能绘制中实现;
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
