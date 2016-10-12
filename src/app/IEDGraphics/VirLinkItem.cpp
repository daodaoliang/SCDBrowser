#include "VirLinkItem.h"
//const qreal Pi = 3.1415926;
#include <QPainter>
#include <QDebug>

CVirLinkItem::CVirLinkItem(HmiGraphicsScene *hmiScene)
{
	mainScene = hmiScene;
	setAcceptHoverEvents(true);
	m_IsInRect = false;
	m_arrowBoth = false;
}

CVirLinkItem::CVirLinkItem(QPointF ptStart, QPointF ptEnd,bool arrowBoth)
{
	setAcceptHoverEvents(true);
    m_PtStart=ptStart;
	m_PtEnd=ptEnd;
	m_IsInRect=false;
	m_arrowBoth=arrowBoth;
}

CVirLinkItem::~CVirLinkItem(void)
{
}

//序列化方法;
bool	CVirLinkItem::save(QDataStream &stream)
{
	stream << m_ItemID;
	int type_1=m_ChangeType;
	stream << type_1;
	stream << m_arrowBoth;
    stream << m_PtStart;
	stream << m_PtEnd;
	stream << m_Rect;
	stream << m_type;
	stream << strIedKeyMaster;
	stream << strIedKeySlave;
	stream << m_CtrlBlockKey;
	stream << m_linkType;
	stream << m_SrcItemID;
	stream << m_DestItemID;
	stream << toolTip();
	return true;
}
bool	CVirLinkItem::load(QDataStream &stream)
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
	stream >> m_arrowBoth;
	stream >> m_PtStart;
	stream >> m_PtEnd;
	stream >> m_Rect;
	stream >> m_type;
	m_IsInRect=false;
	stream >> strIedKeyMaster;
	stream >> strIedKeySlave;
	stream >> m_CtrlBlockKey;
	int linkType;
	stream >> linkType;
	m_linkType = (LinkIndexType)linkType;
	stream >> m_SrcItemID;
	stream >> m_DestItemID;
	QString toolTipStr;
	stream >> toolTipStr;
	setToolTip(toolTipStr);

	return true;
}

//鼠标动作响应方法;
void	CVirLinkItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
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
			QVector<QString> assoIDs;
			assoIDs.append(m_SrcItemID);
			assoIDs.append(m_DestItemID);
			mainScene->assoHighLight(m_ItemID, assoIDs);
		}
	}
}

void	CVirLinkItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	
}

void	CVirLinkItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (strIedKeyMaster == "" && strIedKeySlave == "")
		{
			return;
		}
		QString MSKeys = strIedKeyMaster + "$" + strIedKeySlave;
		mainScene->procInfos(0,MSKeys, m_CtrlBlockKey, m_linkType);
	}
}

void	CVirLinkItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	highLight();
	if (toolTip().compare("") == 0 && m_type != 2)
	{
		setToolTip(GetVLToolTip());
	}
}
void	CVirLinkItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	highLight();
}

//被选中时高亮与取消高亮;
void	CVirLinkItem::highLight()
{
	m_IsInRect = !m_IsInRect;
	update(boundingRect());
}

void	CVirLinkItem::alwaysHighLight()
{
	m_IsInRect = true;
	update(boundingRect());
	setAcceptHoverEvents(false);
}
void	CVirLinkItem::highLightCancel()
{
	setAcceptHoverEvents(true);
	m_IsInRect = false;
	update(boundingRect());
}

void	CVirLinkItem::setStartPt(QPointF pt)
{
	m_PtStart = pt;
}

void	CVirLinkItem::setEndPt(QPointF pt)
{
	m_PtEnd = pt;
}

void	CVirLinkItem::setSrcItemID(QString srcItemID)	//设置起始图元ID;
{    
	   m_SrcItemID=srcItemID;
}

void	CVirLinkItem::setDestItemID(QString destItemID)	//设置目标图元ID;
{
	   m_DestItemID=destItemID;
}

void	CVirLinkItem::setMasterSlaveIEDKey(QString masterIEDKey, QString slaveIEDKey)
{
	//m_MasterIEDKey = masterIEDKey;
	//m_SlaveIEDKey = slaveIEDKey;
	strIedKeyMaster = masterIEDKey;
	strIedKeySlave = slaveIEDKey;
}

void	CVirLinkItem::setCtrlBlockKey(QString cbKey)
{
	m_CtrlBlockKey = cbKey;
}

QString CVirLinkItem::getMasterIEDKey()
{
	return strIedKeyMaster;
}

QString CVirLinkItem::getSlaveIEDKey()
{
	return strIedKeySlave;
}

QString CVirLinkItem::getCtrlBlockKey()
{
	return m_CtrlBlockKey;
}

void	CVirLinkItem::setChangeType(ItemChangeType changeType)	//设置变更类型;
{
	m_ChangeType = changeType;
	QString strDiffInfo = "";
	if (m_ChangeType == DIFFSTATE_EDITED)
	{
		m_toolTipItem = new CToolTipItem("images/modify_sign.png");
		strDiffInfo = "这是一条变更虚回路!";
		mainScene->addItem(m_toolTipItem);
	} 
	else if (m_ChangeType == DIFFSTATE_ADDED)
	{
		m_toolTipItem = new CToolTipItem("images/add_sign.png");
		strDiffInfo = "这是一条新增虚回路!";
		mainScene->addItem(m_toolTipItem);
	}
	else if(m_ChangeType == DIFFSTATE_REMOVED)
	{
		m_toolTipItem = new CToolTipItem("images/delete_sign.png");
		strDiffInfo = "这是一条删除虚回路!";
		mainScene->addItem(m_toolTipItem);
	}
	if (m_ChangeType != DIFFSTATE_NORMAL && m_type == 2)
	{
		m_toolTipItem->setDiffInfo(strDiffInfo);
	}
}

void CVirLinkItem::setToolTipPos(int xPos, int yPos)
{
	QRectF rect = boundingRect();
	int xDis = abs(m_PtEnd.x() - m_PtStart.x())/2;
	int yDis = (m_PtEnd.y() - m_PtStart.y())/2;
	QPointF imagePos;
	if ((m_PtEnd.x() - m_PtStart.x()) > 0)
	{
		imagePos = QPointF(xPos + xDis - 6, yPos + yDis - 5);
	} 
	else
	{
		imagePos = QPointF(xPos + xDis - 6, yPos - yDis - 5);
	}

	if (m_toolTipItem != NULL)
	{
		m_toolTipItem->setPos(imagePos);
	}
}

void CVirLinkItem::setDiffInfo(QString diffInfo)
{
	if (m_toolTipItem != NULL)
	{
		QString strDiffInfo;
		int index=diffInfo.indexOf("#");
		QString count=diffInfo.mid(0,index);
		QString desc=diffInfo.mid(index+1);
		if (m_ChangeType == ItemUpdate)
		{
			strDiffInfo = "变更虚链接 : " + count + "条";
		} 
		else if(m_ChangeType == ItemAdd)
		{
			strDiffInfo = "新增虚链接 : " + count + "条";
		}
		else if (m_ChangeType == ItemDelete)
		{
			strDiffInfo = "删除虚链接 : " + count + "条";
		}
		QString text;
		if(index!=-1)
	        text=desc+"\n"+strDiffInfo;
		else
		    text=strDiffInfo;
		m_toolTipItem->setDiffInfo(text);
	}
}

void  CVirLinkItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{

	QRectF rect = boundingRect();
	// 求得箭头方向
	qreal arrowDirection=m_PtEnd.x()-m_PtStart.x();   //> 0 箭头向右  <0 箭头向左
	painter->setRenderHint(QPainter::Antialiasing);

	// 求出高度差
	qreal y_diff=m_PtEnd.y()-m_PtStart.y(); 
	//qreal drawLength=rect.width()/cos(abs(y_diff)/rect.width());
	qreal drawLength=sqrt(abs(arrowDirection*arrowDirection)+abs(y_diff*y_diff));
	QPointF  ptStart_change(0,rect.height()/2);
	QPointF  ptEnd_change(drawLength,rect.height()/2);
	//求偏转角度
	//qreal deflectionAngle=atan(abs(y_diff)/rect.width())/Pi*180;

	
	QGraphicsLineItem line;
	line.setLine(QLineF(ptStart_change,ptEnd_change));
	qreal arrowSize = 10;
	QBrush brush;
	QPen pen(Qt::NoPen);


	if(m_type==0)
		brush=QColor(4,192,249);
	else if(m_type==1)
		brush=QColor(255,0,0);
	else if(m_type==2)
		brush=QColor(113,113,113);

	if(!m_IsInRect)
	{
	   
	}
	else
	{
		pen = QColor(253,217,0);
		pen.setWidth(1);
		//brush=QColor(4,192,249);
	}
	painter->setBrush(brush);
	painter->setPen(pen);
	double angle = ::acos(line.line().dx() / line.line().length());
	if (line.line().dy() >= 0)
		angle = (Pi * 2) - angle;
	 QPolygonF arrowHead;

	if (arrowDirection<-0.0001&&!m_arrowBoth)
	{
		QPoint ptArrow(line.line().p1().x(),line.line().p1().y());
		QPointF arrowP0 = ptArrow + QPointF(sin(angle + Pi / 3) * arrowSize,
			cos(angle + Pi / 3) * arrowSize);
		QPointF arrowP2 = ptArrow + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
			cos(angle + Pi - Pi / 3) * arrowSize);
		QPointF pt3 = QPointF(arrowP2.x(),arrowP2.y() - (arrowP2.y()-arrowP0.y())/2.5);
		QPointF pt4 = QPointF(rect.right(),pt3.y());
		QPointF pt5 = QPointF(rect.right(),arrowP0.y() + (arrowP2.y()-arrowP0.y())/2.5);
		QPointF pt6 = QPointF(arrowP0.x(),pt5.y());
		arrowHead.clear();
		arrowHead << arrowP0 << ptArrow << arrowP2 << pt3 << pt4 << pt5 << pt6;
		painter->drawPolygon(arrowHead);

	}
	else if (arrowDirection>0.0001&&!m_arrowBoth)
	{
	
		QPoint ptArrow(line.line().p2().x(),line.line().p2().y());
		QPointF arrowP0 = ptArrow + QPointF(sin(angle - Pi / 3) * arrowSize,
			cos(angle - Pi / 3) * arrowSize);//below
		QPointF arrowP2 = ptArrow + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
			cos(angle - Pi + Pi / 3) * arrowSize);//above
		QPointF pt3 = QPointF(arrowP2.x(),arrowP2.y() + abs(arrowP2.y()-arrowP0.y())/2.5);
		QPointF pt4 = QPointF(rect.left(),pt3.y());
		QPointF pt5 = QPointF(rect.left(),arrowP0.y() - abs(arrowP2.y()-arrowP0.y())/2.5);
		QPointF pt6 = QPointF(arrowP0.x(),pt5.y());
		arrowHead.clear();
		arrowHead << arrowP0 << ptArrow << arrowP2 << pt3 << pt4 << pt5 << pt6;
		painter->drawPolygon(arrowHead);
	}
	else if (m_arrowBoth)
	{
	  
		QPoint ptArrow(line.line().p1().x(),line.line().p1().y());
		QPointF arrowP2 = ptArrow + QPointF(sin(angle + Pi / 3) * arrowSize,
			cos(angle + Pi / 3) * arrowSize);
		QPointF arrowP0 = ptArrow + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
			cos(angle + Pi - Pi / 3) * arrowSize);
		QPointF pt3 = QPointF(arrowP2.x(),arrowP2.y() - (arrowP2.y()-arrowP0.y())/2.5);
		QPointF pt9 = QPointF(arrowP0.x(),arrowP0.y() + (arrowP2.y()-arrowP0.y())/2.5);
		QPoint ptArrow2(rect.right(),line.line().p1().y());
		QPointF arrowP5 = ptArrow2 + QPointF(sin(angle - Pi / 3) * arrowSize,
			cos(angle - Pi / 3) * arrowSize);//below
		QPointF arrowP7 = ptArrow2 + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
			cos(angle - Pi + Pi / 3) * arrowSize);//above
		QPointF pt8 = QPointF(arrowP7.x(),pt9.y());
		QPointF pt4 = QPointF(arrowP5.x(),pt3.y());
		arrowHead.clear();
		arrowHead << arrowP0 << ptArrow << arrowP2 << pt3 << pt4 << arrowP5 << ptArrow2 << arrowP7 << pt8 << pt9;
		painter->drawPolygon(arrowHead);
	}
}
QRectF CVirLinkItem::boundingRect()const
{
	return QRectF(0,-3,sqrt(abs((m_PtEnd.x()-m_PtStart.x())*(m_PtEnd.x()-m_PtStart.x()))+abs((m_PtEnd.y()-m_PtStart.y())*(m_PtEnd.y()-m_PtStart.y()))),10);
}

void    CVirLinkItem::setRoateangle()
{
	qreal arrowDirection=m_PtEnd.x()-m_PtStart.x();   //> 0 箭头向右  <0 箭头向左
    qreal y_diff=m_PtEnd.y()-m_PtStart.y(); 
	QRectF rect = boundingRect();
	qreal deflectionAngle=atan(y_diff/arrowDirection)/Pi*180;
	if(y_diff>0.0001||y_diff<-0.0001)
	{
	    setTransformOriginPoint(QPointF(0,rect.height()/2));//设置矩阵旋转坐标原点为小矩形中心原点 ，防止翻转时变形;
	    setRotation(deflectionAngle);
	}
}

void    CVirLinkItem::setTypeLine(int type)
{
	m_type = type;
}

QString CVirLinkItem::GetVLToolTip()
{
	QString toolTipStr = "";
	//return toolTipStr;

	QVector<QVirtualLink> virLinks;
	mainScene->GetGraphicModel()->GetSignalInfosByIEDKeyAndCBKey(strIedKeyMaster, strIedKeySlave, m_CtrlBlockKey, &virLinks);

	for(uint i = 0; i != virLinks.size(); i++)
	{
		QVirtualLink a_vlink = virLinks.at(i);
		QString strSrcSignal = a_vlink.srcSignal;
		QString strDesSignal = a_vlink.destSignal;

		QSignalInfo srcSigInfo, desSigInfo;
		mainScene->GetGraphicModel()->GetInfoOfOutSignal(strSrcSignal,&srcSigInfo);
		mainScene->GetGraphicModel()->GetInfoOfInSignal(strDesSignal,&desSigInfo);

		QString srcStr = srcSigInfo.desc;
		QString desStr = desSigInfo.desc;
		if (srcStr.compare("") == 0)
		{
			srcStr = srcSigInfo.name;
		}
		if (desStr.compare("") == 0)
		{
			desStr = desSigInfo.name;
		}

		QString tempVirStr;
		if (i == virLinks.size() - 1)
		{
			tempVirStr = srcStr + " ---> " + desStr;
		}
		else
		{
			tempVirStr = srcStr + " ---> " + desStr + "\n";
		}
		toolTipStr += tempVirStr;
	}

	return toolTipStr;
}
