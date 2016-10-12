/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      csofttripitem.h
  * Author:         牛子明
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   软压板图源绘制
  */ 

#ifndef CSOFTTRIPITEM_H
#define CSOFTTRIPITEM_H

#include <QGraphicsItem>
#include <QObject>
#include "HmiGraphicsScene.h"

/*
软压板类，图元，内部设置0、1状态，或者图元装载或者程序绘制2种方式;
*/
class CSoftTripItem : public QObject, public QGraphicsItem
{
	Q_OBJECT

public:
	CSoftTripItem();
	CSoftTripItem(QMenu *contextMenu,QGraphicsItem *parent = 0, QGraphicsScene *scene=0);
	~CSoftTripItem();

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		QWidget *widget = 0);
	QRectF boundingRect() const;

	bool	GetStateFlag();
	void	SetStateFlag(bool flag);

	bool	save(QDataStream &stream);
	bool	load(QDataStream &stream);

	void	SetKeyValue(QString str);
	QString GetKeyValue();
protected:
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
	HmiGraphicsScene	*mainScene;		// 
	QMenu				*myContextMenu;	// 
	bool				bCloseFlag;		// 暂无作用
	QString				strKeyValue;	// 信号的内部短地址

    QString ChangeSignalPathToNormal(QString path);
};

#endif // CSOFTTRIPITEM_H
