#include "SubnetItem.h"
#include <QtGui>
#include "Hmidll.h"

CSubnetItem::CSubnetItem(void)
{
}
CSubnetItem::CSubnetItem(QGraphicsItem *parent, QGraphicsScene *scene,int type)
	: QGraphicsItem(parent)
{
	setFlags(QGraphicsItem::ItemIsSelectable);//|QGraphicsItem::ItemIsMovable);
	m_strSNName = "";
}


CSubnetItem::~CSubnetItem(void)
{
}

void CSubnetItem::	paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
	QWidget *widget)
{
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setBrush(QColor(Qt::white));
	QPen pen(QColor(229,229,229,255),0,Qt::SolidLine);//QColor(66,180,234,255)
	pen.setWidthF(1.5);
	painter->setPen(pen);
	
	QRectF rect = boundingRect();
	QRectF textRect(55, 0, 160, 36);
	QPointF centerPt(20,20);
	painter->drawEllipse(centerPt, 20, 20);
	painter->drawEllipse(centerPt, 18, 18);
	
	pen.setColor(Qt::black);
	painter->setPen(pen);

	QFont textFont;
	QString textFontName = QObject::tr("ËÎÌו");
	textFont.setFamily(textFontName);
	textFont.setBold(true);
	textFont.setPointSize(9);

	painter->setFont(textFont);
	painter->drawText(textRect,m_strSNName,Qt::AlignLeft | Qt::AlignTop);
}

QRectF CSubnetItem::boundingRect() const
{
	return QRectF(0,0,160,55);
}

bool CSubnetItem::save(QDataStream &stream)
{
	QRectF m_rect = boundingRect();
	stream << m_rect.left();
	stream << m_rect.top();
	stream << m_rect.right() - m_rect.left();
	stream << m_rect.bottom() - m_rect.top();
	stream << m_strSNName;
	return true;
}

bool CSubnetItem::load(QDataStream &stream)
{
	float l,t,w,h;
	stream >> l;
	stream >> t;
	stream >> w;
	stream >> h;
	boundingRect();
	stream >> m_strSNName;
	return true;
}

void CSubnetItem::setSubNetName(QString subnetName)
{
	m_strSNName = subnetName;
}

QPainterPath CSubnetItem::shape() const
{
	QPainterPath painterPath;
	if (m_strSNName.compare("") == 0)
	{
		painterPath.addRect(boundingRect());
	} 
	else
	{
		QPolygonF myPolyGon;
		myPolyGon.clear();
		myPolyGon.append(QPointF(0,0));
		myPolyGon.append(QPointF(160,0));
		myPolyGon.append(QPointF(160,36));
		myPolyGon.append(QPointF(55,36));
		myPolyGon.append(QPointF(55,55));
		myPolyGon.append(QPointF(0,55));
		myPolyGon.append(QPointF(0,0));
		painterPath.addPolygon(myPolyGon);
	}
	return painterPath;
}
