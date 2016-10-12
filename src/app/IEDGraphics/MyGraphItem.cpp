#include "MyGraphItem.h"
#include <QPainter>

CMyGraphItem::CMyGraphItem()
	: QGraphicsItem()
{
	m_toolTipItem = NULL;
}
CMyGraphItem::~CMyGraphItem()
{

}

QString CMyGraphItem::getItemID()
{
	return m_ItemID;
}

QString CMyGraphItem::getMasterKey()
{
	return strIedKeyMaster;
}

QString CMyGraphItem::getSlaveKey()
{
	return strIedKeySlave;
}

int CMyGraphItem::getLinkType()
{
	return m_linkType;
}

void CMyGraphItem::setMasterKey(QString iedMasterKey)
{
	strIedKeyMaster = iedMasterKey;
}

void CMyGraphItem::setSlaveKey(QString iedSlaveKey)
{
	strIedKeySlave = iedSlaveKey;
}

void CMyGraphItem::setLinkType(LinkIndexType linkType)
{
	m_linkType = linkType;
}

void CMyGraphItem::setChangeType(ItemChangeType changeType)
{
	m_ChangeType = changeType;
}

int CMyGraphItem::getChangeType()
{
	return m_ChangeType;
}

void CMyGraphItem::setDiffInfo(QString diffInfo)
{
	if (m_toolTipItem != NULL)
	{
		m_toolTipItem->setDiffInfo(diffInfo);
	}
}

CMyGraphicsRectItem::CMyGraphicsRectItem(const QRectF &rect, QGraphicsItem *parent)
{
	setRect(rect);
}

CMyGraphicsRectItem::~CMyGraphicsRectItem()
{

}

bool CMyGraphicsRectItem::save(QDataStream &stream)
{
	stream << rect().left();
	stream << rect().top();
	stream << rect().right() - rect().left();
	stream << rect().bottom() - rect().top();\
	return true;
}

bool CMyGraphicsRectItem::load(QDataStream &stream)
{
	float l,t,w,h;
	stream >> l;
	stream >> t;
	stream >> w;
	stream >> h;
	setRect(l,t,w,h);

	return true;
}

bool CMyGraphicsLineItem::save(QDataStream &stream)
{
	stream << line().p1();
	stream << line().p2();

	return true;
}

bool CMyGraphicsLineItem::load(QDataStream &stream)
{
	QPointF p1,p2;
	stream >> p1;
	stream >> p2;
	setLine(p1.x(),p1.y(),p2.x(),p2.y());

	return true;
}

bool CMyGraphicsTextItem::save(QDataStream &stream)
{
	stream << toPlainText();

	return true;
}

bool CMyGraphicsTextItem::load(QDataStream &stream)
{
	QString txt;
	stream >> txt;
	setPlainText(txt);

	return true;
}

//CMyLinkIndex实现
CMyLinkIndex::CMyLinkIndex(/*HmiGraphicsScene *scene, */const QRectF &rect, QGraphicsItem *parent/* = 0*/)
{
	m_isInRect = false;
	m_rect = rect;
	setAcceptsHoverEvents(true);
}

CMyLinkIndex::~CMyLinkIndex()
{

}

bool CMyLinkIndex::save(QDataStream &stream)
{
	//QRectF m_rect = boundingRect();
	stream << m_rect.left();
	stream << m_rect.top();
	stream << m_rect.right() - m_rect.left();
	stream << m_rect.bottom() - m_rect.top();
	stream << m_linkType;
	stream << strIedKeyMaster;
	stream << strIedKeySlave;

	return true;
}

bool CMyLinkIndex::load(QDataStream &stream)
{
	//boundingRect();
	int type;
	stream >> type;
	m_linkType = (LinkIndexType)type;
	QString masterKey, slaveKey;
	stream >> masterKey;
	stream >> slaveKey;
	strIedKeyMaster = masterKey;
	strIedKeySlave = slaveKey;

	return true;
}

void CMyLinkIndex::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (this->boundingRect().contains(event->pos()))
	{
		m_isInRect = true;
	}
}

void CMyLinkIndex::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void CMyLinkIndex::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	m_isInRect = true;
	update(boundingRect());
}

void CMyLinkIndex::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	m_isInRect = false;
	update(boundingRect());
}

void CMyLinkIndex::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
{
	QRectF dRect = boundingRect();
	if (m_isInRect)
	{
		painter->setBrush(QColor(0,106,206,100));
		QPen pen(Qt::red,0,Qt::SolidLine);
		pen.setWidth(5);
		painter->setPen(pen);
		painter->drawRoundedRect(dRect,15,15);
	}
}

QRectF CMyLinkIndex::boundingRect() const
{
	QRectF rect = m_rect;
	return QRectF(rect.left() -15,rect.top() - 15,rect.width() + 45,rect.height() + 45);
}

void CMyLinkIndex::setLinkType(LinkIndexType linkType)
{
	m_linkType = linkType;
}

int CMyLinkIndex::getLinkType()
{
	switch (m_linkType)
	{
	case link_type_subnet:
		return 0;
	case link_type_msgflow:
		return 1;
	case link_type_vloop_goose:
		return 2;
	case link_type_vloop_sv:
		return 3;
	case link_type_vterm:
		return 4;
	case link_type_vloop_diff_goose:
		return 5;
	case link_type_vloop_diff_sv:
		return 6;
	}
	return -1;
}

void CMyLinkIndex::setMasterKey(QString iedMasterKey)
{
	strIedKeyMaster = iedMasterKey;
}

QString CMyLinkIndex::getMasterKey()
{
	return strIedKeyMaster;
}

void CMyLinkIndex::setSlaveKey(QString iedSlaveKey)
{
	strIedKeySlave = iedSlaveKey;
}

QString CMyLinkIndex::getSlaveKey()
{
	return strIedKeySlave;
}

//CMyGotoUpLevelButton实现
CMyGotoUpLevelButton::CMyGotoUpLevelButton(/*HmiGraphicsScene *scene, */const QRectF &rect, QGraphicsItem *parent)
{
	m_rect = rect;
	m_isInRect = false;
}

CMyGotoUpLevelButton::~CMyGotoUpLevelButton()
{

}

bool CMyGotoUpLevelButton::save(QDataStream &stream)
{
	return true;
}

bool CMyGotoUpLevelButton::load(QDataStream &stream)
{
	return true;
}

void CMyGotoUpLevelButton::mouseMoveEvent(QMouseEvent *event)
{
	if (this->rect().contains(event->globalPos()))
	{
		m_isInRect = true;
	}
}

void CMyGotoUpLevelButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	
}

void CMyGotoUpLevelButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
{

}

QRectF CMyGotoUpLevelButton::boundingRect() const
{
	return m_rect;
}