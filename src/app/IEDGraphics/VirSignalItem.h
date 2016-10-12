/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      VirSignalItem.h
  * Author:         nzm
  * Description:   绘制端子排图和虚回路图的信号框及其文字
  */ 
#pragma once
#include "MyGraphItem.h"
#include "HmiGraphicsScene.h"

class CVirSignalItem : public CMyGraphItem
{
public:
	CVirSignalItem(HmiGraphicsScene *hmiScene);
	~CVirSignalItem(void);

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

	void	setVirSignalName(QString virsignalName);		//设置虚信号名称;
	void	setHeight(int height);	//设置高度;
	void	setWidth(int width);	//设置高度;
	void	setChangeType(ItemChangeType changeType);		//设置变更类型;
	void	setToolTipPos(int xPos, int yPos);
	void	appendAssoID(QString assoID);
	QVector<QString> getAssoIDs();
	int		getHeight();
	void	setRightImage(bool isRight);
	void	setDescEmpty(bool isDEmpty);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	QRectF boundingRect() const;

private:
	HmiGraphicsScene *mainScene;
	QRectF m_Rect;
	QString m_VirSignalName;
	int m_Height;
	int m_Width;
	ItemChangeType m_ChangeType;
	QVector<QString> m_assoIDs;
	bool m_IsInRect;
	bool middleWay;//居中方式
	bool isRightImage;
	bool isDescEmpty;

    // add by zxc
public:
    void SetSignalOrd(int ord);
    void SetSignalPath(QString path);

private:
    int m_Ord;
    QString m_SignalPath;
    bool m_iVirLink; //判断信号是否为虚回路
    QColor m_BrushColor;
    QString ChangeSignalPathToNormal(QString path);
};

