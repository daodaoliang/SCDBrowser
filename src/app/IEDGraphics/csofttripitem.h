/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      csofttripitem.h
  * Author:         ţ����
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   ��ѹ��ͼԴ����
  */ 

#ifndef CSOFTTRIPITEM_H
#define CSOFTTRIPITEM_H

#include <QGraphicsItem>
#include <QObject>
#include "HmiGraphicsScene.h"

/*
��ѹ���࣬ͼԪ���ڲ�����0��1״̬������ͼԪװ�ػ��߳������2�ַ�ʽ;
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
	bool				bCloseFlag;		// ��������
	QString				strKeyValue;	// �źŵ��ڲ��̵�ַ

    QString ChangeSignalPathToNormal(QString path);
};

#endif // CSOFTTRIPITEM_H
