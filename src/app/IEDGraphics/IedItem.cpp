#include "IedItem.h"
#include <QtGui>
#include "GlobalDef.h"

CIedItem::CIedItem(QGraphicsItem *parent /* = 0 */, QGraphicsScene *scene/* =0 */,int type, int nwidth,int nheight/* =120 */)
	: QGraphicsItem(parent)
{
	setFlags(QGraphicsItem::ItemIsSelectable/*|QGraphicsItem::ItemIsMovable*/);
	m_key = "";
	m_strName = "220kVÄ¸Ïß²â¿Ø×°ÖÃ";
	m_strModel = "CM2201A";
	m_width = nwidth;
	m_height = nheight;
	m_rect = QRectF(0,0,m_width,m_height);
	boundingRect();
	setZValue(-1001);
}

CIedItem::CIedItem(void)
{
}


CIedItem::~CIedItem(void)
{
}

void CIedItem::DrawHeader()
{

}

QString CIedItem::GetKey()
{
	return m_key;
}

void CIedItem::SetKey(QString &key)
{
	m_key = key;
}

void CIedItem::SetName(QString & name)
{
	m_strName = name;
}

void CIedItem::SetModel(QString &name)
{
	m_strModel = name;
}

void CIedItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
	QWidget *)
{
	QRectF rect = m_rect;
	painter->setBrush(QColor(174,222,242,255));
	QPen pen(Qt::black,0,Qt::SolidLine);
	pen.setWidth(1);
	painter->setPen(pen);
	painter->setRenderHint(QPainter::HighQualityAntialiasing);
	painter->drawRoundedRect(QRectF(rect.left(),rect.top(),rect.right(),rect.bottom()-30),7,7);

	painter->setBrush(QColor(247,252,255,255));
	painter->drawRoundedRect(QRectF(rect.left(),rect.top() + 50,rect.right(),rect.bottom()-50),7,7);
	QPen whitePen(Qt::white, 0, Qt::SolidLine);
	painter->setPen(pen);
	painter->drawRect(QRectF(0,50,rect.width(),rect.bottom()-80));
	QPen grayPen(QColor(247,252,255,255), 0, Qt::SolidLine);
	painter->setPen(grayPen);
	painter->drawLine(2, rect.bottom()-30, rect.width()-1, rect.bottom()-30);

	QRectF rectText = QRectF(rect.left(),rect.top(),rect.right(),50);
	QFont	textFont;
	QString textFontName = QObject::tr("ËÎÌå");
	textFont.setFamily(textFontName);
	painter->setPen(pen);
	if (m_strModel.toAscii().size() > 25)
	{
		textFont.setPointSize(18);
	}
	else
	{
		textFont.setPointSize(22);
	}
	painter->setFont(textFont);
	painter->drawText(rectText,m_strModel,Qt::AlignHCenter | Qt::AlignVCenter);

	painter->setPen(pen);
	if (m_strName.toAscii().size() > 30 && m_height < 120)
	{
		textFont.setPointSize(18);
	} 
	else
	{
		textFont.setPointSize(22);
	}
	painter->setFont(textFont);
	rectText = QRectF(rect.left(),rect.top()+60,rect.width(),150);
	painter->drawText(rectText,m_strName,Qt::AlignHCenter | Qt::AlignTop);
}

QRectF CIedItem::boundingRect() const
{
	return m_rect;
}

bool CIedItem::save(QDataStream &stream)
{
	stream << m_key;
	stream << m_strName;
	stream << m_strModel;
	stream << m_height;
	stream << m_rect.left();
	stream << m_rect.top();
	stream << m_rect.right() - m_rect.left();
	stream << m_rect.bottom() - m_rect.top();

	return true;
}

bool CIedItem::load(QDataStream &stream)
{
	stream >> m_key;
	stream >> m_strName;
	stream >> m_strModel;
	stream >> m_height;
	float l,t,w,h;
	stream >> l;
	stream >> t;
	stream >> w;
	stream >> h;
	m_rect = QRectF(l,t,w,h);
	boundingRect();

	return true;
}

CBaseIEDItem::CBaseIEDItem()
{
	m_CtrlBlockAPPIDs.clear();
}

CBaseIEDItem::~CBaseIEDItem()
{

}