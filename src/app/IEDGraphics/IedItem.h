/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      IedItem.h
  * Author:         zhang-xch
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   IED的Item绘制父类
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
	QString m_IEDName;	//IED名称;
	QString m_IEDDesc;	//IED描述;
	int m_Height;		//图元高度;
	int m_Width;		//图元宽度;
	QVector<QString> m_CtrlBlockAPPIDs;	//包含的控制块;
	IEDType m_IEDType;	//装置类型;
	QString typetext;   //装置类型名称;
	QBrush m_brush;     //装备背景色;
	QBrush m_rectStroke;  //矩形描边颜色
};