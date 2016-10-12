/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      ToolTipItem.h
  * Author:         nzm
  * Version:        V1.0
  * Description:   差异化比较结束后有差异的图标样式的ToolTip
  */ 
#pragma once
#include "qgraphicsitem.h"
#include "DiffInfoDialog.h"
#include <QObject>
//,public QGraphicsItem

class CToolTipItem :public QObject, public QGraphicsItem
{
	Q_OBJECT
public:
	CToolTipItem(const QString imagePath);//, const QRectF rect);
	CToolTipItem(void);
	~CToolTipItem(void);

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		QWidget *widget = 0);
	QRectF boundingRect() const;

	void	mousePressEvent(QGraphicsSceneMouseEvent *event);
	void	mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

	void setPath(QString imagePath);
	void setDiffInfo(QString diffInfo);

	bool	save(QDataStream &stream);
	bool	load(QDataStream &stream);

private:
	QList <CDiffInfoDialog*> diffWidgetList;
	QString m_imagePath;
	QString m_diffInfo;
private slots:
	void diffwidgetClose();
     
};