#include "IEDSlaveItem.h"
#include <QPainter>
#include <QDebug>
CIEDSlaveItem::CIEDSlaveItem(void)
{
	setAcceptHoverEvents(true);
	hendInRect=false;
	manPartInRect=false;
}

CIEDSlaveItem::CIEDSlaveItem(HmiGraphicsScene *hmiScene)
{
	mainScene = hmiScene;
	setAcceptHoverEvents(true);
	hendInRect=false;
	manPartInRect=false;
	m_ReguClr = QColor(247,253,255);
	m_HLClr = QColor(255,240,183);
	m_TitleReguClr = QColor(182,235,251);
	m_TitleHLClr = QColor(253,217,0);
	m_IEDType=NO;
}

CIEDSlaveItem::~CIEDSlaveItem(void)
{
	
}
//序列化方法;
bool	CIEDSlaveItem::save(QDataStream &stream)
{
	stream <<m_IedSlaveType;
	stream << m_ItemID;
	int type_1=m_ChangeType;
	stream << type_1;
	stream << m_IEDName;
	stream << m_IEDDesc;
	stream << strIedKeyMaster;
	stream << strIedKeySlave;
	stream << m_linkType;
	stream << m_Height;
	stream << m_Width;
	int type=m_IEDType;
    stream << type;

	stream << m_CtrlBlockAPPIDs.size();
	for (int i = 0; i != m_CtrlBlockAPPIDs.size(); i++)
	{
		stream << m_CtrlBlockAPPIDs.at(i);
	}

	stream << m_Rect.left();
	stream << m_Rect.top();
	stream << m_Rect.right() - m_Rect.left();
	stream << m_Rect.bottom() - m_Rect.top();
	return true;
}
bool	CIEDSlaveItem::load(QDataStream &stream)
{
	stream >>m_IedSlaveType;
	stream >> m_ItemID;
	int type_1;
	stream >>type_1;
	switch(type_1)
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
	stream >> m_IEDName;
	stream >> m_IEDDesc;
	stream >> strIedKeyMaster;
	stream >> strIedKeySlave;
	int linkType;
	stream >> linkType;
	m_linkType = (LinkIndexType)linkType;
	stream >> m_Height;
	stream >> m_Width;
	int type;
	stream >>type;
	switch(type)
	{
	case    P :	//保护
		typetext="保护";
		m_IEDType=P;
		//m_brush=QColor(250,208,196);
		m_brush = QColor("#fdc83a");
		m_rectStroke=QColor(242,155,138,76);
		break;
	case    C :			//测控
		typetext="测控";
		m_IEDType=C;
		m_brush=QColor(61,206,201);
		m_rectStroke=QColor(38,164,160,76);
		break;
	case	ST:			//智能
		//typetext="智能";
		typetext="终端";
		m_IEDType=ST;
		//m_brush=QColor(196,224,183);
		m_brush = QColor("#8cd0fd");
		m_rectStroke=QColor(121,193,112,76);
		break;
	case	MU:			//合并
		typetext="合并";
		m_IEDType=MU;
		//m_brush=QColor(253,217,0);
		m_brush = QColor("#2ce089");
		m_rectStroke=QColor(223,190,0,76);
		break;
	case	MS:				//智合
		typetext="智合";
		m_IEDType=MS;
		m_brush=QColor(131,204,211);
		m_rectStroke=QColor(4,151,158,76);
		break;
	case	PC:				//保测
		typetext="保测";
		m_IEDType=PC;
		m_brush=QColor(255,195,66);
		m_rectStroke=QColor(231,158,3,76);
		break;
	case	NO:	
		typetext="其它";
		m_IEDType=NO;
		m_brush=QColor(255,242,31);
		m_rectStroke=QColor(199,62,136,76);
		break;
	}

	int cSize;
	stream >> cSize;
	m_CtrlBlockAPPIDs.clear();
	for (int i = 0; i != cSize; i++)
	{
		QString tempStr;
		stream >> tempStr;
		m_CtrlBlockAPPIDs.append(tempStr);
	}

	float l,t,w,h;
	stream >> l;
	stream >> t;
	stream >> w;
	stream >> h;
	m_Rect = QRectF(l,t,w,h);
	boundingRect();
	return true;
}

//鼠标动作响应方法;
void	CIEDSlaveItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
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
			mainScene->assoHighLight(m_ItemID, m_CtrlBlockAPPIDs);
		}
	}
}

void	CIEDSlaveItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	
}

void	CIEDSlaveItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		mainScene->procInfos(0,strIedKeyMaster,strIedKeySlave,m_linkType);
	}
}
void    CIEDSlaveItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	
}
void	CIEDSlaveItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
  	mousePos=event->pos();
	highLight();
}
void	CIEDSlaveItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	mousePos=event->pos();
	highLight();
}

void    CIEDSlaveItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	//mousePos=event->pos();
	//highLight();
}
//被选中时高亮与取消高亮;
void	CIEDSlaveItem::highLight()
{
    if(m_IedSlaveType==0)
	{
		hendInRect = !hendInRect;
		manPartInRect = !manPartInRect;
	}else if(m_IedSlaveType==1)
	{
		if(!boundingRect().contains(mousePos))
		{
		   hendInRect=false;
		   update(boundingRect());
		   return;
		}
		 if(mousePos.y()>0&&mousePos.y()<50)
			 hendInRect = true;
		 else 
			 hendInRect=false;

	}else if(m_IedSlaveType==2)
	{
		if(!boundingRect().contains(mousePos))
		{
			hendInRect=false;
			update(boundingRect());
			return;
		}
		if(mousePos.y()>0&&mousePos.y()<40)
			hendInRect = true;
		else
			hendInRect=false;
	}
	update(boundingRect());	 
}
void	CIEDSlaveItem::alwaysHighLight()
{
	//setAcceptHoverEvents(false);
	m_ReguClr = m_HLClr;
	m_TitleReguClr = m_TitleHLClr;
}
void	CIEDSlaveItem::highLightCancel()
{
	setAcceptHoverEvents(true);
	m_ReguClr = QColor(247,253,255);
	m_TitleReguClr = QColor(182,235,251);
	update(boundingRect());
}

void	CIEDSlaveItem::setSideOfMaster(bool isRight)
{
	m_IsRightSide = isRight;
}

void CIEDSlaveItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	m_Rect = boundingRect();

	//画装置矩形

	if (m_IedSlaveType==0)
	{
		if(!manPartInRect)
			painter->setBrush(m_ReguClr);
		else
			painter->setBrush(m_HLClr);
		QPen pen(QColor(155,155,155),0,Qt::SolidLine);
		pen.setWidth(2);
		painter->setPen(pen);
		painter->setRenderHint(QPainter::HighQualityAntialiasing);
		painter->drawRoundedRect(QRectF(m_Rect.left(),m_Rect.top(),m_Rect.right(),m_Rect.bottom()),0,0);
		
		//画标题
		if(!hendInRect)
			painter->setBrush(m_TitleReguClr);
		else
			painter->setBrush(m_TitleHLClr);
		QPen pen_biaoti(Qt::NoPen);
		painter->setPen(pen_biaoti);
		QRectF rectText=QRectF(m_Rect.left()+1,m_Rect.top()+1,m_Rect.right()-2.0,21);
		painter->drawRoundedRect(rectText,2,2);
	
		QFont textFont;
		QString textFontName = QObject::tr("宋体");
		QPen pen_1(QColor(0,0,0));
		textFont.setFamily(textFontName);
		textFont.setPixelSize(12);
		painter->setFont(textFont);
		painter->setPen(pen_1);
		painter->drawText(rectText,m_IEDName,Qt::AlignHCenter | Qt::AlignVCenter);

		//画装置
	  rectText=QRectF(m_Rect.left()+2,m_Rect.top()+1,29,21);
	  painter->setPen(Qt::NoPen);
	  painter->setBrush(m_brush);
	  painter->drawRoundedRect(rectText,2,2);
	
      rectText=QRectF(m_Rect.left()+3,m_Rect.top()+2,27,19);         //矩形描边;
      QPen pen_2(QColor(255,255,255));
	  pen.setWidth(0.5);
	  painter->setPen(pen_2);
	  painter->setBrush(m_rectStroke);
	  painter->drawRect(rectText);

		QFont font;
	   textFontName = QObject::tr("宋体");
	   font.setFamily(textFontName);
	   font.setPixelSize(12);
	   painter->setFont(font);
	   painter->drawText(rectText,typetext,Qt::AlignVCenter | Qt::AlignHCenter);

		// 画描述
		rectText=QRectF(m_Rect.left(),40,m_Rect.right(),m_Rect.bottom());
		QFont font_2;
		if (m_IEDDesc.toAscii().size() > 30 && m_Height < 120)
		{
			font_2.setPointSize(18);
		} 
		else
		{
			font_2.setPointSize(22);
		}
	
		textFontName = QObject::tr("宋体");
		font_2.setFamily(textFontName);
		font_2.setPixelSize(12);
		painter->setFont(font_2);
		painter->setPen(pen_1);
		rectText = QRectF(m_Rect.left(),m_Rect.top(),m_Rect.width(),m_Rect.height()+21);
		painter->drawText(rectText,m_IEDDesc,Qt::AlignHCenter | Qt::AlignVCenter);

		QPointF imagePt = QPointF(m_Width - 6, -7);

	}
	else if(m_IedSlaveType==1)
	{
		if(!manPartInRect)
			painter->setBrush(m_ReguClr);
		else
			painter->setBrush(m_HLClr);/**/
		//painter->setBrush(m_ReguClr);
		QPen pen(QColor(155,155,155),0,Qt::SolidLine);
		pen.setWidth(1);
		painter->setPen(pen);
		painter->setRenderHint(QPainter::HighQualityAntialiasing);
		painter->drawRoundedRect(QRectF(m_Rect.left(),m_Rect.top(),m_Rect.right(),m_Rect.bottom()),0,0);

		if(!hendInRect)
			painter->setBrush(m_TitleReguClr);
		else
			painter->setBrush(m_TitleHLClr);
		QPen pen_biaoti(Qt::NoPen);
		painter->setPen(pen_biaoti);
		QRectF rectText=QRectF(m_Rect.left()+0.6,m_Rect.top()+0.6,m_Rect.right()-1.2,50);
		painter->drawRoundedRect(rectText,2,2);

		rectText =QRectF(m_Rect.left(),m_Rect.top(),m_Rect.right(),20);
		QFont textFont;
		QString textFontName = QObject::tr("宋体");
		textFont.setBold(true);
		QPen pen_1(QColor(0,0,0));
		textFont.setFamily(textFontName);
		textFont.setPixelSize(12);
		painter->setFont(textFont);
		painter->setPen(pen_1);
		painter->drawText(rectText,m_IEDName,Qt::AlignHCenter | Qt::AlignVCenter);

		rectText =QRectF(m_Rect.left(),m_Rect.top()+20,m_Rect.right(),30);
		textFont.setFamily(textFontName);
		textFont.setPixelSize(12);
		painter->setFont(textFont);
		painter->setPen(pen_1);
		painter->drawText(rectText,m_IEDDesc,Qt::AlignHCenter | Qt::AlignVCenter);
	}
	else if(m_IedSlaveType==2)
	{
		if(!manPartInRect)
			painter->setBrush(m_ReguClr);
		else
			painter->setBrush(m_HLClr);/**/
		painter->setBrush(m_ReguClr);
		QPen pen(QColor(155,155,155),0,Qt::SolidLine);
		pen.setWidth(1);
		painter->setPen(pen);
		painter->setRenderHint(QPainter::HighQualityAntialiasing);
		painter->drawRoundedRect(QRectF(m_Rect.left(),m_Rect.top(),m_Rect.right(),m_Rect.bottom()),0,0);
		if(!hendInRect)
			painter->setBrush(m_TitleReguClr);
		else
			painter->setBrush(m_TitleHLClr);
		QPen pen_biaoti(Qt::NoPen);
		painter->setPen(pen_biaoti);
		QRectF rectText=QRectF(m_Rect.left()+0.6,m_Rect.top()+0.6,m_Rect.right()-1.2,40);
		painter->drawRoundedRect(rectText,2,2);

		QFont textFont;
		QString textFontName = QObject::tr("宋体");
		QPen pen_1(QColor(0,0,0));
		textFont.setFamily(textFontName);
		textFont.setPixelSize(12);
		painter->setFont(textFont);
		painter->setPen(pen_1);
		painter->drawText(rectText,m_IEDDesc,Qt::AlignHCenter | Qt::AlignVCenter);
	}
}

QRectF CIEDSlaveItem::boundingRect() const
{
	return QRectF(0,0,m_Width,m_Height);
}

void    CIEDSlaveItem::setIedSlaveType(int type)
{ 
	m_IedSlaveType=type;
}

void	CIEDSlaveItem::setChangeType(ItemChangeType changeType)
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

void	CIEDSlaveItem::setToolTipPos(int xPos, int yPos)
{
	QPointF imagePos;
	if (m_IsRightSide)
	{
		imagePos = QPointF(xPos + m_Width - 6, yPos - 7);
	}
	else
	{
		imagePos = QPointF(xPos - 6, yPos - 7);
	}
	if (m_toolTipItem != NULL)
	{
		m_toolTipItem->setPos(imagePos);
	}
}