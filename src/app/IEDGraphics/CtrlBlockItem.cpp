#include "CtrlBlockItem.h"
#include <QPainter>
#include <QDebug>

CtrlBlockItem::CtrlBlockItem(HmiGraphicsScene *hmiScene)
{
	mainScene = hmiScene;
	setAcceptHoverEvents(true);
	m_IsInRect=false;
	m_Width = 80;
    m_Height = 27;
	m_Rect= QRectF(0,0,m_Width,m_Height);
	m_AssoItemIDs.clear();
	m_APPID = "0000";
	m_IsRight = false;
}


CtrlBlockItem::~CtrlBlockItem(void)
{
}
//���л�����;
bool CtrlBlockItem::save(QDataStream &stream)
{
	stream << m_ItemID;
	stream << m_Width;
    stream << m_Height;
	stream << m_Rect.left();
	stream << m_Rect.top();
	stream << m_Rect.width();
	stream << m_Rect.height();
	stream << m_RegularClr;
	stream << m_HighLightClr;
	stream << m_APPID;
	stream << m_Name;
	stream << toolTip();
	stream << m_AssoItemIDs.size();

	for (int i = 0; i != m_AssoItemIDs.size(); i++)
	{
		stream << m_AssoItemIDs.at(i);
	}
	int type_1 = m_ChangeType;
	stream << type_1;
	int type=m_SigType;
	stream << type;
	stream << m_IsRight;
	return true;
}
bool CtrlBlockItem::load(QDataStream &stream)
{
	stream >> m_ItemID;
	stream >> m_Width;
    stream >> m_Height;
	float l,t,w,h;
	stream >> l;
	stream >> t;
	stream >> w;
	stream >> h;
	m_Rect = QRectF(l,t,m_Width,h);
	boundingRect();
	stream >> m_RegularClr;
	stream >> m_HighLightClr;
	stream >> m_APPID;
	stream >> m_Name;
	QString toolTipStr;
	stream >> toolTipStr;
	setToolTip(toolTipStr);

	int aSize;
	stream >> aSize;
	m_AssoItemIDs.clear();
	for (int i = 0; i != aSize ; i++)
	{
		QString tempStr;
		stream >> tempStr;
		m_AssoItemIDs.append(tempStr);
	}

	int type_1;
	stream >>type_1;
	switch(type_1)
	{
	case    ItemUpdate :			//���;
		m_ChangeType=ItemUpdate;
		break;
	case	ItemAdd:			//����;
		m_ChangeType=ItemAdd;
		break;
	case	ItemDelete:			//�ϲ�;
		m_ChangeType=ItemDelete;
		break;
	default:	//����;
		m_ChangeType=NoChange;
		break;
	}
	
	int type;
	stream >> type;
	switch(type)
	{
	case    gooseType :	
		m_SigType=gooseType;
		break;
	case    svType :			
		m_SigType=svType;
		break;
	case	inputType:			
		m_SigType=inputType;
		break;
	}
	stream >> m_IsRight;

	return true;
}


//��궯����Ӧ����;
void	CtrlBlockItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
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

void CtrlBlockItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	
}

void CtrlBlockItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	
}

void CtrlBlockItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	highLight();
}

void CtrlBlockItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	highLight();
	QToolTip::hideText();
}
//��ѡ��ʱ������ȡ������;
void CtrlBlockItem::highLight()
{
	m_IsInRect = !m_IsInRect;
	update();
}
void CtrlBlockItem::alwaysHighLight()
{
	m_IsInRect = true;
	update();
	setAcceptHoverEvents(false);
}
void CtrlBlockItem::highLightCancel()
{
	setAcceptHoverEvents(true);
	m_IsInRect = false;
	update();
}

void CtrlBlockItem::setCBName(QString cbName)
{
	m_Name = cbName;
}
void	CtrlBlockItem::setAPPID(QString appid)  //����APPID
{
	if (appid != "")
	{
		m_APPID = appid;
	}
}

void	CtrlBlockItem::setIsRight(bool isRight)
{
	m_IsRight = isRight;
}

int		CtrlBlockItem::getSigType()
{
	return m_SigType;
}
void	CtrlBlockItem::setSigType(CtrlBlockSigType sigType)	//�����ź����� 0:Goose   1:SV   2:����;
{
	m_SigType=sigType;
}

void	CtrlBlockItem::setRegularClr(const QColor clr)//���û�����ɫ;
{
	m_RegularClr=clr;
}
void	CtrlBlockItem::setHighLightClr(const QColor clr)	//���ø�����ɫ;
{
	m_HighLightClr=clr;
}
void	CtrlBlockItem::setChangeType(ItemChangeType changeType)		//���ñ������;
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

void	CtrlBlockItem::setToolTipPos(int xPos, int yPos)
{
	QPointF imagePos;
	if (m_IsRight)
	{
		imagePos = QPointF(xPos - 6, yPos - 7);
	} 
	else
	{
		imagePos = QPointF(xPos + m_Width - 6, yPos - 7);
	}
	if (m_toolTipItem != NULL)
	{
		m_toolTipItem->setPos(imagePos);
	}
}

void	CtrlBlockItem::setWidth(int width)
{
	m_Width = width;
	m_Rect = QRectF(0,0,m_Width, m_Height);
}

void	CtrlBlockItem::appendAssoID(QString assoID)
{
	m_AssoItemIDs.append(assoID);
}

QVector<QString> CtrlBlockItem::getAssoIDs()
{
	return m_AssoItemIDs;
}

QString CtrlBlockItem::getCBName()
{
	return m_Name;
}

QString CtrlBlockItem::getAPPID()
{
	if (m_APPID == "0000")
	{
		return "";
	} 
	else
	{
		return m_APPID;
	}
}

QPoint	CtrlBlockItem::getLeftPoint()			//���������ӵ�;
{
	QPointF scenePos=pos();
	return QPoint(scenePos.x(),scenePos.y()+m_Rect.height()/2);
}
QPoint	CtrlBlockItem::getRightPoint()	//����Ҳ����ӵ�;
{
	QPointF scenePos=pos();
	return QPoint(scenePos.x()+m_Rect.width(),scenePos.y()+m_Rect.height()/2);
}


void CtrlBlockItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	m_Rect=boundingRect();
	QPen pen(QColor(155,155,155));
	pen.setWidth(2);
	painter->setPen(pen);
	QString content="";
	QString tempAPPID = QString("0x") + m_APPID;
	QBrush bursh;
	if(m_SigType==gooseType)
	{
		if(!m_IsInRect)
			bursh=m_RegularClr;
		else
			bursh=m_HighLightClr;
		painter->setBrush(bursh);
		painter->drawRoundedRect(m_Rect,2,2);
		content="GOOSE " + tempAPPID;
	}
	else if(m_SigType==svType)
	{
		if(!m_IsInRect)
			bursh=m_RegularClr;
		else
			bursh=m_HighLightClr;
		painter->setBrush(bursh);
		painter->drawRoundedRect(m_Rect,2,2);
		content="SV " + tempAPPID;
	}
	else if(m_SigType==inputType)
	{
		if(!m_IsInRect)
			bursh=m_RegularClr;
		else
			bursh=m_HighLightClr;
		painter->setBrush(bursh);
		painter->drawRoundedRect(m_Rect,2,2);
		content="����";
	}
	QPen textPen(QColor(0,0,0));
	QFont textFont;
	QString textFontName = QObject::tr("����");
	textFont.setFamily(textFontName);
	textFont.setPointSize(20);
	textFont.setPixelSize(12);
	painter->setFont(textFont);
	painter->setPen(textPen);
	painter->drawText(m_Rect,content,Qt::AlignHCenter|Qt::AlignVCenter);

	QPointF imagePt;
	if (m_IsRight)
	{
		imagePt = QPointF(- 6, -7);
	} 
	else
	{
		imagePt = QPointF(m_Width - 6, -7);
	}
}
QRectF CtrlBlockItem::boundingRect() const
{
	  return QRectF(0,0,m_Width, m_Height);
}

void CtrlBlockItem::SetHeight(int height)
{
    m_Height = height;
    m_Rect = QRectF(0,0,m_Width, m_Height);
}