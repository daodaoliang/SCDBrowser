/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      MyGraphItem.h
  * Author:         zhang-xch
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   Item类的父类
  */ 

#ifndef CMYGRAPHICITEM_H
#define CMYGRAPHICITEM_H

#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QApplication>//必须加这个和Q_DECLARE_TR_FUNCTIONS宏，否则编译错误
#include "ToolTipItem.h"

enum LinkIndexType {
	link_type_subnet = 0,
	link_type_msgflow,	
	link_type_vloop_goose,
	link_type_vloop_sv,
	link_type_vterm,
	link_type_vloop_diff_goose,
	link_type_vloop_diff_sv,
	link_type_diff_msgflow,
	link_type_diff_signal
};

enum ItemChangeType {
	UnKnown = 0,
	NoChange,
	ItemDelete,
	ItemAdd,
	ItemUpdate	
};

struct LinkIndexParam {
	QString		strMasterKey;
	QString		strSlaveKey;
	LinkIndexType type;
};

//图元基类;
class CMyGraphItem : public QGraphicsItem
{
public:
	CMyGraphItem();
	virtual ~CMyGraphItem();

	//序列化方法;
	virtual bool	save(QDataStream &stream) = 0;
	virtual bool	load(QDataStream &stream) = 0;

	//鼠标动作响应方法;
	virtual void	mousePressEvent(QGraphicsSceneMouseEvent *event) = 0;
	virtual void	mouseReleaseEvent(QGraphicsSceneMouseEvent *event) = 0;
	virtual void	mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) = 0;
	virtual void	hoverEnterEvent(QGraphicsSceneHoverEvent *event) = 0;
	virtual void	hoverLeaveEvent(QGraphicsSceneHoverEvent *event) = 0;

	//被选中时高亮与取消高亮;
	virtual void	highLight() = 0;
	virtual void	alwaysHighLight() = 0;
	virtual void	highLightCancel() = 0;

	virtual void	setItemID(QString itemID)
	{
		m_ItemID = itemID;
	}
	virtual QString getItemID();
	virtual void	setMasterKey(QString iedMasterKey);
	virtual QString getMasterKey();
	virtual void	setSlaveKey(QString iedSlaveKey);
	virtual QString getSlaveKey();
	virtual void	setLinkType(LinkIndexType linktype);
	virtual int		getLinkType();
	virtual void	setChangeType(ItemChangeType changeType);
	virtual int		getChangeType();
	virtual void	setDiffInfo(QString diffInfo);

protected:
	QString m_ItemID;	//图元ID;
	QString				strIedKeyMaster;	//主IED
	QString				strIedKeySlave;		//从IED
	QString				m_ToolTipStr;
	LinkIndexType		m_linkType;
	ItemChangeType		m_ChangeType;
	CToolTipItem*		m_toolTipItem;
};

class CMyGraphicsRectItem : public CMyGraphItem , public QGraphicsRectItem
{
public:
	CMyGraphicsRectItem(const QRectF &rect, QGraphicsItem *parent = 0);
	~CMyGraphicsRectItem();

	bool	save(QDataStream &stream);
	bool	load(QDataStream &stream);
};

class CMyGraphicsLineItem : public CMyGraphItem , public QGraphicsLineItem
{
public:
	bool	save(QDataStream &stream);
	bool	load(QDataStream &stream);
};

class CMyGraphicsTextItem : public CMyGraphItem , public QGraphicsTextItem
{
public:
	bool	save(QDataStream &stream);
	bool	load(QDataStream &stream);
};

//热点类;
class CMyLinkIndex : public QGraphicsItem
{
public:
	CMyLinkIndex (/*HmiGraphicsScene *scene, */const QRectF &rect, QGraphicsItem *parent = 0);
	~ CMyLinkIndex ();

	bool save(QDataStream &stream);
	bool load(QDataStream &stream);

	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

	//重写绘图函数，不显示;
	void	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	QRectF boundingRect() const;

	void	setMasterKey(QString iedMasterKey);
	QString getMasterKey();
	void	setSlaveKey(QString iedSlaveKey);
	QString getSlaveKey();
	void	setLinkType(LinkIndexType linktype);
	int		getLinkType();

protected:
	LinkIndexType		m_linkType;
	QString				strIedKeyMaster,	//主IED
						strIedKeySlave;		//从IED

private:
	bool m_isInRect;
	QRectF m_rect;
};

class CMyGotoUpLevelButton : public CMyGraphItem , public QGraphicsRectItem
{
public:
	CMyGotoUpLevelButton (/*HmiGraphicsScene *scene, */const QRectF &rect, QGraphicsItem *parent = 0);
	~ CMyGotoUpLevelButton ();

	bool	save(QDataStream &stream);
	bool	load(QDataStream &stream);

	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);

	//绘制返回上一级 图片按钮;
	void	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	QRectF boundingRect() const;

protected:
	LinkIndexType		linkType;
	QString				strIedKeyMaster,	//主IED
						strIedKeySlave;		//从IED

private:
	bool m_isInRect;
	QRectF m_rect;
};

#endif // CMYGRAPHICITEM_H