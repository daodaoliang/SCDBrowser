#include "IEDMasterItem.h"
#include <QPainter>
#include <QDebug>

CIEDMasterItem::CIEDMasterItem(void)
{
	setAcceptHoverEvents(true);
	m_IsInRect=false;
	m_CtrlBlockAPPIDs.clear();
}

CIEDMasterItem::CIEDMasterItem(HmiGraphicsScene *hmiScene)
{
	mainScene = hmiScene;
	setAcceptHoverEvents(true);
	m_IsInRect=false;
	m_CtrlBlockAPPIDs.clear();
}


CIEDMasterItem::~CIEDMasterItem(void)
{

}
//序列化方法;
bool CIEDMasterItem::save(QDataStream &stream)
{
	stream << m_ItemID;
	int type_1=m_ChangeType;
	stream << type_1;
	stream << m_IEDKey;
	stream << m_IEDName;
	stream << m_IEDDesc;
	stream << strIedKeyMaster;
	stream << strIedKeySlave;
	stream << m_linkType;
	int type = m_IEDType;
	stream << type;
	stream << m_Height;
	stream << m_Width;

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
bool CIEDMasterItem::load(QDataStream &stream)
{
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
	stream >> m_IEDKey;
	stream >> m_IEDName;
	stream >> m_IEDDesc;
	stream >> strIedKeyMaster;
	stream >> strIedKeySlave;
	int linkType;
	stream >> linkType;
	m_linkType = (LinkIndexType)linkType;
	int type;
	stream >> type;
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
		m_brush = QColor("#8cd0fd");
		m_rectStroke=QColor(121,193,112,76);
		break;
	case	MU:			//合并
		typetext="合并";
		m_IEDType=MU;
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
	stream >> m_Height;
	stream >> m_Width;

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
void	CIEDMasterItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mousePressEvent(event);
}

void	CIEDMasterItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}

void	CIEDMasterItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		mainScene->procInfos(0,strIedKeyMaster,strIedKeySlave,m_linkType);
	}
}
void	CIEDMasterItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	//highLight();
}
void	CIEDMasterItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	//highLight();
}


//被选中时高亮与取消高亮;
void	CIEDMasterItem::highLight()
{
	m_IsInRect = !m_IsInRect;
	update();
}
void	CIEDMasterItem::alwaysHighLight()
{

}
void	CIEDMasterItem::highLightCancel()
{

}

void CIEDMasterItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	m_Rect=boundingRect();
	if(!m_IsInRect)
    	painter->setBrush(QColor(247,253,255));
	else
		painter->setBrush(QColor(255,240,183));
	QPen pen(QColor(155,155,155));
	pen.setWidth(2);
	painter->setPen(pen);
	painter->setRenderHint(QPainter::HighQualityAntialiasing);
	painter->drawRoundedRect(QRectF(m_Rect.left(),m_Rect.top(),m_Rect.right(),m_Rect.bottom()),2,2);
	
	//根据包含的控制块 调整间距;
	int i=m_CtrlBlockAPPIDs.size();
	int init_top= (((m_Height)- ((27+15)*i+20*2+50+15*2+10+40)))/2;
	if(init_top<0)
		init_top=m_Rect.top()+20;

	//绘制文字;
	QRectF rectText = QRectF(m_Rect.left(),init_top+20,m_Rect.right(),20);
	QFont	textFont;
	QString textFontName = QObject::tr("宋体");
	textFont.setFamily(textFontName);
	if (m_IEDName.toAscii().size() > 25)
	{
		textFont.setPointSize(18);
	}
	else
	{
		textFont.setPointSize(22);
	}
	painter->setPen(Qt::black);
	textFont.setPixelSize(12);
	painter->setBrush(QColor(238,203,173));	
	painter->setFont(textFont);
	painter->drawText(rectText,m_IEDName,Qt::AlignVCenter | Qt::AlignHCenter);

	
	if (m_IEDDesc.toAscii().size() > 30 && m_Height < 120)
	{
		textFont.setPointSize(18);
	} 
	else
	{
		textFont.setPointSize(22);
	}
	painter->setPen(Qt::black);
    textFont.setPixelSize(12);
	painter->setFont(textFont);
	rectText = QRectF(m_Rect.left(),init_top+50,m_Rect.width(),50);
	painter->drawText(rectText,m_IEDDesc,Qt::AlignTop | Qt::AlignHCenter);
	QPen pen_type(Qt::NoPen);
	painter->setPen(pen_type);     
	textFont.setPixelSize(12);
	painter->setFont(textFont);
	painter->setBrush(m_brush);
	painter->drawRoundedRect(m_Rect.left()+49,init_top,29,18,2,2); 

	rectText=QRectF(m_Rect.left()+49+1,init_top+1,27,16);         //矩形描边 
	QPen pen_2(QColor(255,255,255));
	pen.setWidth(0.5);
	painter->setPen(pen_2); 
	painter->setBrush(m_rectStroke);
	painter->drawRect(rectText);
	painter->setPen(QColor(255,255,255)); 
	rectText=  QRectF(m_Rect.left()+49,init_top,29,18);
	painter->drawText(rectText,typetext,Qt::AlignVCenter | Qt::AlignHCenter);

}
QRectF CIEDMasterItem::boundingRect() const
{
	 return QRectF(0,0,m_Width,m_Height);
}
int  CIEDMasterItem::getAdjustHeight()
{
	//int Height=800;
	if(m_CtrlBlockAPPIDs.size()==0)
	{
		return  m_Height;
	}
	else
	{
		int i=m_CtrlBlockAPPIDs.size();
		int hg=20*2;  //预留底部和顶部空间;

		int result=m_Height-((27+15)*m_CtrlBlockAPPIDs.size()+20*2+50+15*2+10+hg);
		if(result>0)
			return m_Height;
		else
		{ 
			int m_count=(m_Height-(20*2+50+15*2+10+hg))/(27+15);//得到当前高度最多能包含的控制块数目;
			if(m_count<m_CtrlBlockAPPIDs.size())
			{
				int adjustHeight=m_Height+((m_CtrlBlockAPPIDs.size()-m_count))*(27+15);// 如果不能包含 返回调整后的高度;
				m_Height=adjustHeight;
				return adjustHeight;
			}
			return 0;
		}
	}
}

int CIEDMasterItem::getAdjustHeight(int cbCount)
{
	if(cbCount==0)
	{
		return  m_Height;
	}
	else
	{
		int hg=20*2;  //预留底部和顶部空间;

		int result=m_Height-((27+15)*cbCount+20*2+50+15*2+10+hg);
		if(result>0)
			return m_Height;
		else
		{ 
			int m_count=(m_Height-(20*2+50+15*2+10+hg))/(27+15);//得到当前高度最多能包含的控制块数目;
			if(m_count<cbCount)
			{
				int adjustHeight=m_Height+((cbCount-m_count))*(27+15);// 如果不能包含 返回调整后的高度;
				m_Height=adjustHeight;
				return adjustHeight;
			}
			return 0;
		}
	}
}

void CIEDMasterItem::setChangeType(ItemChangeType changeType)
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

void CIEDMasterItem::setToolTipPos(int xPos, int yPos)
{
	QPointF imagePos = QPointF(xPos - 7, yPos - 7);
	if (m_toolTipItem != NULL)
	{
		m_toolTipItem->setPos(imagePos);
	}
}