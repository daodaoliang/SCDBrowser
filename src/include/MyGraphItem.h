#ifndef CMYGRAPHICITEM_H
#define CMYGRAPHICITEM_H

#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QApplication>//����������Q_DECLARE_TR_FUNCTIONS�꣬����������
//#include "HmiGraphicsScene.h"

enum LinkIndexType {
	link_type_subnet = 0,
	link_type_msgflow,
	link_type_vloop_goose,
	link_type_vloop_sv,
	link_type_vterm
};

struct LinkIndexParam {
	QString		strMasterKey;
	QString		strSlaveKey;
	LinkIndexType type;
};

class CMyGraphItem : public QGraphicsItem
{
public:
	CMyGraphItem();
	~CMyGraphItem();

	virtual bool	save(QDataStream &stream) = 0;
	virtual bool	load(QDataStream &stream) = 0;
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

//�ȵ���;
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

	//��д��ͼ����������ʾ;
	void	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	QRectF boundingRect() const;

	void	setMasterKey(QString iedMasterKey);
	QString getMasterKey();
	void	setSlaveKey(QString iedSlaveKey);
	QString getSlaveKey();
	void	setLinkType(LinkIndexType linktype);
	int		getLinkType();

	//LinkIndexParam getLinkIndexParam();
protected:
	LinkIndexType		m_linkType;
	QString				strIedKeyMaster,	//��IED
						strIedKeySlave;		//��IED

private:
	bool m_isInRect;
	QRectF m_rect;
	//HmiGraphicsScene *mainScene;
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

	//���Ʒ�����һ�� ͼƬ��ť;
	void	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	QRectF boundingRect() const;

protected:
	LinkIndexType		linkType;
	QString				strIedKeyMaster,	//��IED
						strIedKeySlave;		//��IED

private:
	bool m_isInRect;
	QRectF m_rect;
	//HmiGraphicsScene *mainScene;
};

#endif // CMYGRAPHICITEM_H