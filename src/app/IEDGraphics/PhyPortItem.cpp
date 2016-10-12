#include "PhyPortItem.h"
#include <QPainter>

CPhyPortItem::CPhyPortItem(HmiGraphicsScene *hmiScene)
{
	setAcceptHoverEvents(true);
	mainScene = hmiScene;
	m_AssoItemIDs.clear();
	m_IsInRect=false;
}


CPhyPortItem::~CPhyPortItem(void)
{
}
//序列化方法;
bool	CPhyPortItem::save(QDataStream &stream)
{
	stream << m_ItemID;
	stream << m_Width;
	stream << m_Height;
	stream << m_PortName;
	stream << m_PortType;
	stream << m_AssoItemIDs.size();
	for (uint i = 0; i != m_AssoItemIDs.size(); i++)
	{
		stream << m_AssoItemIDs.at(i);
	}
	int type=m_ChangeType;
	stream << type;
	stream << m_vSize;
	return true;
}

bool	CPhyPortItem::load(QDataStream &stream)
{
	stream >> m_ItemID;
	stream >> m_Width;
	stream >> m_Height;
	boundingRect();
	stream >> m_PortName;
	stream >> m_PortType;
	int assIDsSize = 0;
	stream >> assIDsSize;
	for (uint i = 0; i != assIDsSize; i++)
	{
		QString tempID;
		stream >> tempID;
		m_AssoItemIDs.append(tempID);
	}

	int type;
	stream >>type;
	switch(type)
	{
	case    NoChange :	//保护
		m_ChangeType=NoChange;
		break;
	case    ItemUpdate :			//测控
		m_ChangeType=ItemUpdate;
		break;
	case	ItemAdd:			//智能
		m_ChangeType=ItemAdd;
		break;
	case	ItemDelete:			//合并
		m_ChangeType=ItemDelete;
		break;
	}
	stream >> m_vSize;
	m_IsInRect = false;
	return true;
}

//鼠标动作响应方法;
void	CPhyPortItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (mainScene->getDrag())
		{
			QGraphicsItem::mousePressEvent(event);
		} 
		else
		{
			alwaysHighLight();
			mainScene->assoHighLight(m_ItemID, m_AssoItemIDs);
		}
	}
}

void	CPhyPortItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	
}

void	CPhyPortItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

}
void	CPhyPortItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	highLight();
}
void	CPhyPortItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	highLight();
}

//被选中时高亮与取消高亮;
void	CPhyPortItem::highLight()
{
	m_IsInRect = !m_IsInRect;
	update(boundingRect());
}
void	CPhyPortItem::alwaysHighLight()
{
	m_IsInRect = true;
	update(boundingRect());
	setAcceptHoverEvents(false);
}
void	CPhyPortItem::highLightCancel()
{
	setAcceptHoverEvents(true);
	m_IsInRect = false;
	update(boundingRect());
}

void CPhyPortItem::setWidth(const int widthNum)
{
	m_Width = widthNum;
}

void CPhyPortItem::setHeight(const int heightNum)
{
	m_Height = heightNum;
}

QString CPhyPortItem::getPortName()
{
	return m_PortName;
}

void	CPhyPortItem::setPortName(QString portName)	//设置物理端口名;
{
	m_PortName = portName;
}

QString CPhyPortItem::getPortType()
{
	return m_PortType;
}

void	CPhyPortItem::setPortType(QString portType)
{
	m_PortType = portType;
}

void	CPhyPortItem::setVerticalSize(const int vSize)
{
	m_vSize = vSize;
}

void	CPhyPortItem::setChangeType(ItemChangeType changeType)	//设置变更类型;
{
	m_ChangeType = changeType;
	if (m_ChangeType == DIFFSTATE_EDITED)
	{
		m_toolTipItem = new CToolTipItem("images/modify_sign.png");
		mainScene->addItem(m_toolTipItem);
	} 
	else if (m_ChangeType == DIFFSTATE_ADDED)
	{
		m_toolTipItem = new CToolTipItem("images/add_sign.png");
		mainScene->addItem(m_toolTipItem);
	}
	else if(m_ChangeType == DIFFSTATE_REMOVED)
	{
		m_toolTipItem = new CToolTipItem("images/delete_sign.png");
		mainScene->addItem(m_toolTipItem);
	}
}

void	CPhyPortItem::setToolTipPos(int xPos, int yPos)
{
	QPointF imagePos = QPointF(xPos - 6, yPos - 7);
	if (m_toolTipItem != NULL)
	{
		m_toolTipItem->setPos(imagePos);
	}
}

void	CPhyPortItem::appendAssoID(QString assoID)
{
	if (!m_AssoItemIDs.contains(assoID))
	{
		m_AssoItemIDs.append(assoID);
	}
}

QVector<QString> CPhyPortItem::getAssoIDs()
{
	return m_AssoItemIDs;
}

void CPhyPortItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	QRectF portRect(0,0,m_Width,m_Height);// = boundingRect();
	QBrush brush;
	QPen pen(QColor(155,155,155));
	pen.setWidth(2);
	QColor vLineColor;
	if(!m_IsInRect)
	{
		brush = QColor(238,238,209);
		vLineColor = QColor(250,204,167);
	}
	else
	{
		brush = QColor(253,217,0); // 黄色
		vLineColor = QColor(200,200,200);
	}
	painter->setPen(pen);
	painter->setBrush(brush);
	painter->drawRoundedRect(portRect,1,1);

	QPen vLinePen(vLineColor);
	vLinePen.setWidth(4);
	painter->setPen(vLinePen);
	double VXPos = m_Width/2;
	double VYPos = m_Height + m_vSize;
	QLineF vLine(QPointF(VXPos, m_Height), QPointF(VXPos, VYPos));
	painter->drawLine(vLine);

	QFont textFont;
	QString textFontName = QObject::tr("宋体");
	textFont.setFamily(textFontName);
	textFont.setPixelSize(12);
	pen.setColor(Qt::black);
	painter->setPen(pen);
	painter->setFont(textFont);
	painter->drawText(portRect, m_PortName, Qt::AlignHCenter | Qt::AlignVCenter);
}

QRectF CPhyPortItem::boundingRect() const
{
	return QRectF(0,0,m_Width,m_Height + m_vSize);
}

QPainterPath CPhyPortItem::shape() const
{
	QPainterPath painterPath;
	if (m_vSize == 0)
	{
		painterPath.addRect(boundingRect());
	} 
	else
	{
		QPolygonF myPolyGon;
		myPolyGon.clear();
		myPolyGon.append(QPointF(0,0));
		myPolyGon.append(QPointF(m_Width,0));
		myPolyGon.append(QPointF(m_Width,m_Height));
		myPolyGon.append(QPointF(m_Width/2 + 2, m_Height));
		myPolyGon.append(QPointF(m_Width/2 + 2, m_Height + m_vSize));
		myPolyGon.append(QPointF(m_Width/2 - 2, m_Height + m_vSize));
		myPolyGon.append(QPointF(m_Width/2 - 2, m_Height));
		myPolyGon.append(QPointF(0,m_Height));
		myPolyGon.append(QPointF(0,0));
		painterPath.addPolygon(myPolyGon);
	}
	return painterPath;
}
