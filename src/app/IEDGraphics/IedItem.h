/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      IedItem.h
  * Author:         zhang-xch
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   IED��Item���Ƹ���
  */ 
#pragma once
#include "qgraphicsitem.h"
#include <QGraphicsScene>
#include "GlobalDef.h"
#include "MyGraphItem.h"
#include "HmiGraphicsScene.h"

class CIedItem : public QGraphicsItem
{
public:
	CIedItem(QGraphicsItem *parent = 0, QGraphicsScene *scene=0,int type = 0, int nwidth = IED_WIDTH,int nheight = 150/**/);
	CIedItem(void);
	~CIedItem(void);

	virtual void DrawHeader();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		QWidget *widget = 0);
	QRectF boundingRect() const;

	QString		GetKey();
	void	SetKey(QString &key);
	void	SetName(QString & name);
	void	SetModel(QString &name);
	bool	save(QDataStream &stream);
	bool	load(QDataStream &stream);
private:
	QString			m_key,m_strName,m_strModel;
	int				m_width,m_height;
	QRectF			m_rect;
};

class CBaseIEDItem : public CMyGraphItem
{
public:
	CBaseIEDItem();
	~CBaseIEDItem();

	void setIEDKey(QString iedKey)
	{
		m_IEDKey = iedKey;
	}

	virtual void setIEDName(QString iedName)
	{
		m_IEDName = QString("[") + iedName + QString("]");
	}

    void setIEDDesc(QString iedDesc)
	{
		m_IEDDesc=iedDesc;
	}

	void setHeight(int height)
	{
		 m_Height=height;
	}

	void setWidth(int width)
	{
		 m_Width=width;
	}

	void setIEDType(IEDType iedType)
	{
		 m_IEDType=iedType;
	}

	void appendCtrlBlock(QString ctrlBlockAPPID)
	{
		 m_CtrlBlockAPPIDs.append(ctrlBlockAPPID);
	}

	int getHeight()
	{
		return m_Height;
	}

	QVector<QString> getCtrlBlockIDs()
	{
		return m_CtrlBlockAPPIDs;
	}

protected:
	QString m_IEDKey;	//IEDKey;
	QString m_IEDName;	//IED����;
	QString m_IEDDesc;	//IED����;
	int m_Height;		//ͼԪ�߶�;
	int m_Width;		//ͼԪ���;
	QVector<QString> m_CtrlBlockAPPIDs;	//�����Ŀ��ƿ�;
	IEDType m_IEDType;	//װ������;
	QString typetext;   //װ����������;
	QBrush m_brush;     //װ������ɫ;
	QBrush m_rectStroke;  //���������ɫ
};