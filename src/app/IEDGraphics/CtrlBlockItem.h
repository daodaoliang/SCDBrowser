/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      CtrlBlockItem.h
  * Author:         zhang-xch
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   控制块的图源，包括信息流图和虚回路图的
  */ 
#ifndef CTRLBLOCK_H
#define CTRLBLOCK_H

#include "MyGraphItem.h"
#include "HmiGraphicsScene.h"

enum CtrlBlockSigType
{
	gooseType = 0,
	svType,
	inputType
};

class CtrlBlockItem : public CMyGraphItem
{
public:
	CtrlBlockItem(HmiGraphicsScene *hmiScene);
	~CtrlBlockItem(void);

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

	void	setCBName(QString cbName);	//设置Name
	void	setAPPID(QString appid);	//设置APPID
	void	setSigType(CtrlBlockSigType sigType);	//设置信号类型 0:Goose   1:SV   2:接收;

	void	setRegularClr(const QColor clr);	//设置基本颜色;
	void	setHighLightClr(const QColor clr);	//设置高亮颜色;
	void	setChangeType(ItemChangeType changeType);		//设置变更类型;
	void	setToolTipPos(int xPos, int yPos);

	void	setWidth(int width);
	void	setIsRight(bool isRight);

	void	appendAssoID(QString assoID);
	QVector<QString> getAssoIDs();

	QString getCBName();
	QString getAPPID();
	int		getSigType();
	QPoint	getLeftPoint();			//获得左侧连接点;
	QPoint	getRightPoint();		//获得右侧连接点;

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	QRectF boundingRect() const;

private:
	HmiGraphicsScene *mainScene;
	QString m_Name;				//name
	QString m_APPID;			//appid
	CtrlBlockSigType m_SigType;	//控制块属性goose或sv或接收;
	QColor m_RegularClr;		//基本颜色;
	QColor m_HighLightClr;		//高亮颜色;
	QVector<QString> m_AssoItemIDs;
	int m_Width;
	QRectF m_Rect;
	ItemChangeType m_ChangeType;
	bool m_IsInRect;
	bool m_IsRight;

public:
    void SetHeight(int height);

private:
    int m_Height;

};
#endif //#ifndef CTRLBLOCK_H