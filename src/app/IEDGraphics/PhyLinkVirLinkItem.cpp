#include "PhyLinkVirLinkItem.h"
#include <QPainter>
#include  "qmath.h"
#define Pi 3.1415926

CPhyLinkVirLinkItem::CPhyLinkVirLinkItem(HmiGraphicsScene *hmiScene)
{
	setAcceptHoverEvents(true);
	mainScene = hmiScene;
	m_IsInRect = false;
	m_portPosVec.clear();
	m_DestItemIDs.clear();

	QString textFontName = QObject::tr("宋体");
	textFont.setFamily(textFontName);
	textFont.setPixelSize(12);
    m_bInSameInterval = true;
}
CPhyLinkVirLinkItem::CPhyLinkVirLinkItem(HmiGraphicsScene *hmiScene, QPointF ptStart, QPointF ptEnd)
{
	setAcceptHoverEvents(true);
	mainScene = hmiScene;
	m_PtStart=ptStart;
	m_PtEnd=ptEnd;
	m_IsInRect = false;
	m_portPosVec.clear();
	m_DestItemIDs.clear();

	QString textFontName = QObject::tr("宋体");
	textFont.setFamily(textFontName);
	textFont.setPixelSize(12);
    m_bInSameInterval = true;
}

CPhyLinkVirLinkItem::~CPhyLinkVirLinkItem(void)
{
}
//序列化方法;
bool	CPhyLinkVirLinkItem::save(QDataStream &stream)
{
	stream << m_ItemID;
	stream << m_SrcItemID;
	stream << m_DestItemIDs.size();
	for (uint i = 0; i != m_DestItemIDs.size(); i++)
	{
		stream << m_DestItemIDs.at(i);
	}
	stream << m_DestDesc;
	stream << m_PtStart;
	stream << m_PtEnd;
	//stream << m_vSize;
	stream << m_realWidth;
	int type = m_ChangeType;
	stream << type;
	stream << m_portPosVec.size();
	for (uint i = 0; i != m_portPosVec.size(); i++)
	{
		stream << m_portPosVec.at(i);
	}
    stream << m_bInSameInterval;
	return true;
}

bool	CPhyLinkVirLinkItem::load(QDataStream &stream)
{
	stream >> m_ItemID;
	stream >> m_SrcItemID;
	int destIDsSize = 0;
	stream >> destIDsSize;
	for (uint i = 0; i != destIDsSize; i++)
	{
		QString tempID;
		stream >> tempID;
		m_DestItemIDs.append(tempID);
	}
	stream >> m_DestDesc;
	stream >> m_PtStart;
	stream >> m_PtEnd;
	//stream >> m_vSize;
	stream >> m_realWidth;
	int type;
	stream >> type;
	switch(type)
	{
	case    NoChange :	//保护
		m_ChangeType=NoChange;
		break;
	case    ItemUpdate :		//测控
		m_ChangeType=ItemUpdate;
		break;
	case	ItemAdd:			//智能
		m_ChangeType=ItemAdd;
		break;
	case	ItemDelete:			//合并
		m_ChangeType=ItemDelete;
		break;
	}

	int pVecSize = 0;
	stream >> pVecSize;
	for (uint i = 0; i != pVecSize; i++)
	{
		int portPos;
		stream >> portPos;
		m_portPosVec.append(portPos);
	}
    stream >> m_bInSameInterval;
	m_IsInRect = false;
	return true;
}

//鼠标动作响应方法;
void	CPhyLinkVirLinkItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (mainScene->getDrag())
	{
		QGraphicsItem::mousePressEvent(event);
	} 
	else
	{
		alwaysHighLight();
		QVector<QString> assoIDs;
		assoIDs.append(m_SrcItemID);
		//assoIDs.append(m_DestItemID);
		for (uint i = 0; i != m_DestItemIDs.size(); i++)
		{
			assoIDs.append(m_DestItemIDs.at(i));
		}
		mainScene->assoHighLight(m_ItemID, assoIDs);
	}
}

void	CPhyLinkVirLinkItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	
}

void	CPhyLinkVirLinkItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

}
void	CPhyLinkVirLinkItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	highLight();
}
void	CPhyLinkVirLinkItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	highLight();
}

//被选中时高亮与取消高亮;
void	CPhyLinkVirLinkItem::highLight()
{
	m_IsInRect = !m_IsInRect;
	update();
}
void	CPhyLinkVirLinkItem::alwaysHighLight()
{
	m_IsInRect = true;
	update();
	setAcceptHoverEvents(false);
}
void	CPhyLinkVirLinkItem::highLightCancel()
{
	setAcceptHoverEvents(true);
	m_IsInRect = false;
	update();
}

void	CPhyLinkVirLinkItem::setSrcItemID(QString srcItemID)	//设置起始图元ID;
{
	m_SrcItemID=srcItemID;
}

void	CPhyLinkVirLinkItem::appendDestItemID(const QString& destItemID)
{
	if (!m_DestItemIDs.contains(destItemID))
	{
		m_DestItemIDs.append(destItemID);
	}
}

void	CPhyLinkVirLinkItem::setDestDesc(QString destDesc)
{
	m_DestDesc = destDesc;
	int lineLength = abs(m_PtStart.x()-m_PtEnd.x());
	QFontMetrics fm(textFont);
	int textLength = fm.width(m_DestDesc) + 140;
	m_realWidth = qMax(lineLength, textLength);
}

void	CPhyLinkVirLinkItem::setChangeType(ItemChangeType changeType)	//设置变更类型;
{
	m_ChangeType=changeType;
}

//void	CPhyLinkVirLinkItem::setVerticalSize(int vSize)
//{
//	m_vSize = vSize;
//}

int	CPhyLinkVirLinkItem::getRealWidth()
{
	return m_realWidth;
}

void CPhyLinkVirLinkItem::setportPosVec(QVector<int> &posVec)
{
	for (uint i = 0; i != posVec.size(); i++)
	{
		m_portPosVec.append(posVec.at(i));
	}
}

void CPhyLinkVirLinkItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QRectF rect = boundingRect();
	QPointF ptStart_change;
	QPointF ptEnd_change;
	QPointF ptEnd1_change;
	int vXPos = abs(m_PtEnd.x()-m_PtStart.x());
	//int vYPos = m_PtStart.y() + m_vSize;
	if((m_PtEnd.x()-m_PtStart.x())>0.00001)
	{
		ptStart_change=QPointF(0,0);
		ptEnd_change=QPointF(90,m_PtStart.y());
		ptEnd1_change=QPointF(vXPos, m_PtEnd.y());
	}
	else
	{
		ptStart_change=QPointF(90,m_PtStart.y());
		ptEnd_change= QPointF(vXPos,m_PtEnd.y());
	}
	painter->setRenderHint(QPainter::Antialiasing,true);
	QGraphicsLineItem line;
	line.setLine(QLineF(ptStart_change, ptEnd_change));
	//QLineF vLine(QPointF(vXPos, 0),QPointF(vXPos, vYPos));
	qreal arrowSize = 8;
	int textLeng = m_DestDesc.size() * 20;
	QRectF textRect = QRectF(rect.left()+140, rect.top()+2, textLeng + 5, 25);
	QFont textFont;
	QString textFontName = QObject::tr("宋体");
	textFont.setFamily(textFontName);
	//textFont.setPointSize(20);
	textFont.setPixelSize(12);
	painter->setFont(textFont);
	double angle = ::acos(line.line().dx() / line.line().length());
	if (line.line().dy() >= 0)
		angle = (Pi * 2)-angle;
	QPolygonF arrowHead;
	if((m_PtEnd.x()-m_PtStart.x())>0.00001)
	{	
		painter->setPen(Qt::NoPen);
		if(!m_IsInRect)
			painter->setBrush(QColor(255,138,136));
		else
			painter->setBrush(QColor(253,217,0));
		
		QPoint ptArrow(line.line().p2().x(),line.line().p2().y());
		QPointF arrowP0 = ptArrow + QPointF(sin(angle - Pi / 3) * arrowSize,
			cos(angle - Pi / 3) * arrowSize);//below


		QPointF arrowP2 = ptArrow + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
			cos(angle - Pi + Pi / 3) * arrowSize);//above
		QPointF pt3 = QPointF(arrowP2.x(),arrowP2.y() + abs(arrowP2.y()-arrowP0.y())/2.5+1.5);
		QPointF pt4 = QPointF(rect.left(),pt3.y());
		QPointF pt5 = QPointF(rect.left(),arrowP0.y() - abs(arrowP2.y()-arrowP0.y())/2.5-1.5);
		QPointF pt6 = QPointF(arrowP0.x(),pt5.y());

		QPointF pt7 = QPointF(ptArrow.x()-0.6,arrowP0.y());
		QPointF pt8 = QPointF(ptArrow.x()-0.6,arrowP2.y());
		QPointF pt9 = QPointF(ptArrow.x()+abs(ptArrow.x()-pt3.x()),pt3.y());
		QPointF pt10= QPointF(pt9.x(),pt6.y());
		QPointF pt11=QPointF(ptEnd1_change.x(),pt9.y());
		QPointF pt12=QPointF(ptEnd1_change.x(),pt10.y());
		arrowHead.clear();
		arrowHead<<pt4<<pt3<<arrowP2<<ptArrow<<pt8<<pt9<<pt11<<pt12<<pt10<<pt7<<ptArrow<<arrowP0<<pt6<<pt5;
		painter->drawPolygon(arrowHead);

        if (!m_bInSameInterval)
        {
            QPen pen_text(QColor(0,0,255));
            painter->setPen(pen_text);
        }
        else
        {
            QPen pen_text(QColor(238,64,0));
            painter->setPen(pen_text);
        }
		painter->drawText(textRect,m_DestDesc,Qt::AlignLeft| Qt::AlignTop);

		QPen pen(QColor(114,159,220));
		painter->setPen(pen);
		painter->setBrush(QColor(114,159,220));

		for (uint i = 0; i != m_portPosVec.size(); i++)
		{
			painter->drawEllipse(m_portPosVec.at(i) + 12, m_PtStart.y() - 6,12,12);
		}
	}
	else
	{
		if(!m_IsInRect)
		{
			painter->setPen(QColor(48,156,203));
			painter->setBrush(QColor(48,156,203));
		}
		else
		{
			painter->setPen(QColor(253,217,0));
			painter->setBrush(QColor(253,217,0));
		}

		QPoint ptArrow(line.line().p1().x(),line.line().p1().y());
		QPointF arrowP0 = ptArrow + QPointF(sin(angle + Pi / 3) * arrowSize,
			cos(angle + Pi / 3) * arrowSize);
		QPointF arrowP2 = ptArrow + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
			cos(angle + Pi - Pi / 3) * arrowSize);
		QPointF pt3  = QPointF(arrowP2.x(),arrowP2.y() - (arrowP2.y()-arrowP0.y())/2.5+1.2);
		QPointF pt4  = QPointF(rect.right(),pt3.y());
		QPointF pt5  = QPointF(rect.right(),arrowP0.y() + (arrowP2.y()-arrowP0.y())/2.5-1.2);
		QPointF pt6  = QPointF(arrowP0.x(),pt5.y());
		QPointF pt7  = QPointF(ptArrow.x(),arrowP2.y());
		QPointF pt8  = QPointF(ptArrow.x(),arrowP0.y());
		QPointF pt9  = QPointF(ptArrow.x()-abs(ptArrow.x()-pt3.x()),pt3.y());
		QPointF pt10 = QPointF(ptArrow.x()-abs(ptArrow.x()-pt6.x()),pt6.y());
		QPointF pt11 = QPointF(rect.left(),pt9.y());
		QPointF pt12 = QPointF(rect.left(),pt10.y());
		arrowHead.clear();
		arrowHead <<pt11<<pt9<<pt7<<ptArrow<<arrowP2<<pt3<<pt4<<pt5<<pt6
			<<arrowP0<<ptArrow<<pt8<<pt10<<pt12;
		painter->drawPolygon(arrowHead);
		QPointF VerlineStart(QPointF(abs(m_PtEnd.x()-m_PtStart.x()),0));
		QPointF VerlineEnd(QPointF(abs(m_PtEnd.x()-m_PtStart.x()),abs(m_PtEnd.y()-m_PtStart.y())));
		QPen pen(QColor(255,138,136));
		//pen.setWidth(0.1);
		//painter->setPen(pen);
		painter->drawLine(VerlineStart,VerlineEnd);
		//painter->drawLine(vLine);
        if (!m_bInSameInterval)
        {
            QPen pen_text(QColor(0,0,255));
            painter->setPen(pen_text);
        }
        else
        {
            QPen pen_text(QColor(238,64,0));
            painter->setPen(pen_text);
        }		
		painter->drawText(textRect,m_DestDesc,Qt::AlignLeft| Qt::AlignTop);

		pen.setColor(QColor(114,159,220));
		painter->setPen(pen);
		painter->setBrush(QColor(114,159,220));
		for (uint i = 0; i != m_portPosVec.size(); i++)
		{
			//painter->drawPoint(m_portPosVec.at(i), m_PtStart.y());
			painter->drawEllipse(m_portPosVec.at(i) + 12, m_PtStart.y() - 6,12,12);
		}
	}	
}

QRectF CPhyLinkVirLinkItem::boundingRect() const
{
	return QRectF(0,-20,abs(m_PtStart.x()-m_PtEnd.x()),30);
}

void CPhyLinkVirLinkItem::SetDestIsInSameInterval(bool iInterval)
{
    m_bInSameInterval = iInterval;
}