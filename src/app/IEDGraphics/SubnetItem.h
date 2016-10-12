/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      SubnetItem.h
  * Author:         nzm
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   绘制子网图圆形与文字
  */ 
#pragma once
#include "qgraphicsitem.h"

class CSubnetItem :
	public QGraphicsItem
{
public:
	CSubnetItem(QGraphicsItem *parent = 0, QGraphicsScene *scene=0,int type = 0);
	CSubnetItem(void);
	~CSubnetItem(void);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		QWidget *widget = 0);
	QRectF boundingRect() const;
	bool	save(QDataStream &stream);
	bool	load(QDataStream &stream);

	void setSubNetName(QString subnetName);

	QPainterPath shape() const;
private:
	QString m_strSNName;
};
