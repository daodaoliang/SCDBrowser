#include "IEDSubnetItem.h"
#include <QPainter>
#include <QDebug>
#include "QLinearGradient"
#include "VoltageLevelAnalysor.h"

CIEDSubnetItem::CIEDSubnetItem(void)
{
	setAcceptHoverEvents(true);
	hendInRect=false;
	manPartInRect=false;
}

CIEDSubnetItem::CIEDSubnetItem(HmiGraphicsScene *hmiScene)
{
	mainScene = hmiScene;
	setAcceptHoverEvents(true);
	hendInRect = false;
	manPartInRect = false;

	m_ReguClr = QColor(247,253,255);
	m_HLClr = QColor(255,240,183);

	m_TitleReguClr = QColor(174,222,242);
	m_TitleHLClr = QColor(253,217,0);

	m_IEDType = NO;
}

CIEDSubnetItem::~CIEDSubnetItem(void)
{

}
//序列化方法;
bool	CIEDSubnetItem::save(QDataStream &stream)
{
	stream << m_ItemID;
	int type_1 = m_ChangeType;
	stream << type_1;
	stream << m_IEDName;
	stream << m_IEDDesc;
	stream << strIedKeyMaster;
	stream << strIedKeySlave;
	stream << m_linkType;
	stream << m_Height;
	stream << m_Width;
	int type = m_IEDType;
	stream << type;

	stream << m_Rect.left();
	stream << m_Rect.top();
	stream << m_Rect.right() - m_Rect.left();
	stream << m_Rect.bottom() - m_Rect.top();
	return true;
}
bool	CIEDSubnetItem::load(QDataStream &stream)
{
	stream >> m_ItemID;
	int type_1;
	stream >> type_1;
	switch(type_1)
	{
	case    NoChange :
		m_ChangeType = NoChange;
		break;
	case    ItemUpdate :
		m_ChangeType = ItemUpdate;
		break;
	case	ItemAdd:
		m_ChangeType = ItemAdd;
		break;
	case	ItemDelete:
		m_ChangeType = ItemDelete;
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
	stream >> type;
	switch(type)
	{
	case    P :	//保护;
		typetext="保护";
		m_IEDType=P;
		m_brush = QColor("#fcca42");
		m_rectStroke=QColor("#dda206");
		break;
	case    C :			//测控;
		typetext="测控";
		m_IEDType=C;
		m_brush = QColor(61,206,201);
		m_rectStroke = QColor(38,164,160);
		break;
	case	ST:			//智能;
		typetext="终端";
		m_IEDType=ST;
		m_brush = QColor("#8ed1fe");
		m_rectStroke=QColor("#169fff");
		break;
	case	MU:			//合并;
		typetext="合并";
		m_IEDType=MU;
		m_brush = QColor("#2dde8e");
		m_rectStroke=QColor("#0bc16e");
		break;
	case	MS:				//智合;
		typetext="智合";
		m_IEDType=MS;
		m_brush=QColor(131,204,211);
		m_rectStroke=QColor(4,151,158);
		break;
	case	PC:				//保测;
		typetext="保测";
		m_IEDType=PC;
		m_brush=QColor(255,195,66);
		m_rectStroke=QColor(231,158,3);
		break;
	case	NO:	
		typetext="其它";
		m_IEDType=NO;
		m_brush=QColor(255,242,31);
		m_rectStroke=QColor(199,62,136);
		break;
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
void	CIEDSubnetItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
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

void	CIEDSubnetItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}

void	CIEDSubnetItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		mainScene->procInfos(0,strIedKeyMaster,strIedKeySlave,m_linkType);
	}
}
void    CIEDSubnetItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{

}
void	CIEDSubnetItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	mousePos=event->pos();
	highLight();
}
void	CIEDSubnetItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{

	mousePos=event->pos();
	highLight();
}

void    CIEDSubnetItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	//mousePos=event->pos();
	//highLight();
}
//被选中时高亮与取消高亮;
void	CIEDSubnetItem::highLight()
{
	hendInRect = !hendInRect;
	manPartInRect = !manPartInRect;
	
	update(boundingRect());	 
}
void	CIEDSubnetItem::alwaysHighLight()
{
	m_ReguClr = m_HLClr;
	m_TitleReguClr = m_TitleHLClr;
}
void	CIEDSubnetItem::highLightCancel()
{
	setAcceptHoverEvents(true);
	m_ReguClr = QColor(247,253,255);
	m_TitleReguClr = QColor(174,222,242);
	update(boundingRect());
}

void CIEDSubnetItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	m_Rect = boundingRect();

	//设置画笔与画刷;
	painter->setBrush(m_vtClr);
	QPen pointLinePen(m_vtClr, 2, Qt::SolidLine);
	painter->setPen(pointLinePen);

	//画装置矩形
	if(!manPartInRect)
		painter->setBrush(m_ReguClr);
	else
		painter->setBrush(m_HLClr);
	QPen pen(QColor(155,155,155),0,Qt::SolidLine);
	pen.setWidth(1);
	painter->setPen(pen);
	painter->setRenderHint(QPainter::HighQualityAntialiasing);
	painter->drawRoundedRect(QRectF(m_Rect.left(),m_Rect.top(),m_Rect.right(),m_Rect.bottom()),3,3);
	
	//画标题
	if(!hendInRect)
		painter->setBrush(m_TitleReguClr);
	else
		painter->setBrush(m_TitleHLClr);
	QPen pen_biaoti(Qt::NoPen);
	painter->setPen(pen_biaoti);
	QRectF rectText=QRectF(m_Rect.left()+1,m_Rect.top()+1,m_Rect.right()-1.8,21);
	painter->drawRoundedRect(rectText,4,4);

	QFont textFont;
	QString textFontName = QObject::tr("宋体");
	QPen pen_1(QColor(0,0,0));
	textFont.setFamily(textFontName);
	textFont.setPixelSize(12);
	textFont.setBold(true);

	painter->setFont(textFont);
	painter->setPen(pen_1);
	painter->drawText(rectText,m_IEDName,Qt::AlignHCenter | Qt::AlignVCenter);

	//画装置
	QLinearGradient linearGradient(13,0,13,19);
	linearGradient.setColorAt(0, m_brush.color());
	linearGradient.setColorAt(1, m_rectStroke.color());
	rectText = QRectF(m_Rect.left()+2,m_Rect.top()+2,29,19);
	QPen typePen(m_rectStroke.color(), Qt::SolidLine);
	typePen.setWidthF(0.5);
	painter->setPen(typePen);
	//painter->setBrush(m_brush);
	painter->setBrush(linearGradient);
	painter->drawRoundedRect(rectText,2,2);

	rectText=QRectF(m_Rect.left()+3,m_Rect.top()+2,27,19);         //矩形描边;
	QPen pen_2(QColor(255,255,255));
	pen.setWidth(0.5);
	painter->setPen(pen_2);
	painter->setBrush(m_rectStroke);
	//painter->drawRect(rectText);

	QFont font;
	textFontName = QObject::tr("宋体");
	font.setFamily(textFontName);
	font.setPixelSize(11);
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
}

QRectF CIEDSubnetItem::boundingRect() const
{
	return QRectF(0,0,m_Width,m_Height);
}

void	CIEDSubnetItem::setIEDName(QString iedName)
{
	m_IEDName = iedName;
}

void	CIEDSubnetItem::setChangeType(ItemChangeType changeType)
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

void	CIEDSubnetItem::setToolTipPos(int xPos, int yPos)
{
	QPointF imagePos;
	imagePos = QPointF(xPos + m_Width - 6, yPos - 7);
	
	if (m_toolTipItem != NULL)
	{
		m_toolTipItem->setPos(imagePos);
	}
}
