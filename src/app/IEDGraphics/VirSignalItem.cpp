#include "VirSignalItem.h"
#include <QPainter>

CVirSignalItem::CVirSignalItem(HmiGraphicsScene *hmiScene)
{
	mainScene = hmiScene;
	setAcceptHoverEvents(true);
	m_IsInRect=false;
	middleWay=false;
	isRightImage = false;
	isDescEmpty = false;
    m_Ord = 0;
}


CVirSignalItem::~CVirSignalItem(void)
{
}
//序列化方法;
bool	CVirSignalItem::save(QDataStream &stream)
{
	stream << m_ItemID;
	stream << m_Rect.left();
	stream << m_Rect.top();
	stream << m_Rect.right() - m_Rect.left();
	stream << m_Rect.bottom() - m_Rect.top();
	stream << m_VirSignalName;
    stream << m_SignalPath;
    stream << m_Ord;
	stream << m_Height;
	stream << m_Width;
	stream << m_assoIDs.size();
	for (int i = 0; i != m_assoIDs.size(); i++)
	{
		stream << m_assoIDs.at(i);
	}
	int type = m_ChangeType;
	stream << type;
	stream << middleWay;
	stream << isRightImage;
	stream << isDescEmpty;
	return true;
}
bool	CVirSignalItem::load(QDataStream &stream)
{
	stream >> m_ItemID;
	float l,t,w,h;
	stream >> l;
	stream >> t;
	stream >> w;
	stream >> h;
	m_Rect = QRectF(l,t,w,h);
	boundingRect();
	stream >> m_VirSignalName;
    stream >> m_SignalPath;
    stream >> m_Ord;
	stream >>m_Height;
	stream >>m_Width;
	int aSize;
	stream >> aSize;
	for (int i = 0; i != aSize; i++)
	{
		QString tempStr;
		stream >> tempStr;
		m_assoIDs.append(tempStr);
	}
	int type;
	stream >>type;
	switch(type)
	{
	case    NoChange :	//保护;
		m_ChangeType=NoChange;
		break;
	case    ItemUpdate :			//测控;
		m_ChangeType=ItemUpdate;
		break;
	case	ItemAdd:			//智能;
		m_ChangeType=ItemAdd;
		break;
	case	ItemDelete:			//合并;
		m_ChangeType=ItemDelete;
		break;
	}
	stream >> middleWay;
	stream >> isRightImage;
	stream >> isDescEmpty;
	return true;
}

//鼠标动作响应方法;
void	CVirSignalItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
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
			mainScene->assoHighLight(m_ItemID, m_assoIDs);
		}
	}
}

void	CVirSignalItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	
}

void	CVirSignalItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

}
void	CVirSignalItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	highLight();
}
void	CVirSignalItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	highLight();
}

//被选中时高亮与取消高亮;
void	CVirSignalItem::highLight()
{
	m_IsInRect = !m_IsInRect;
	update();
}
void	CVirSignalItem::alwaysHighLight()
{
	m_IsInRect = true;
	update();
	setAcceptHoverEvents(false);
}
void	CVirSignalItem::highLightCancel()
{
	setAcceptHoverEvents(true);
	m_IsInRect = false;
	update();
}

void	CVirSignalItem::setVirSignalName(QString virsignalName)	//设置虚信号名称;
{
	m_VirSignalName=virsignalName;
} 
void	CVirSignalItem::setHeight(int height)	//设置高度;
{
	m_Height=height;
}
void	CVirSignalItem::setWidth(int width)	//设置高度;
{
	m_Width=width;
}
void	CVirSignalItem::setChangeType(ItemChangeType changeType)		//设置变更类型;
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

void	CVirSignalItem::setRightImage(bool isRight)
{
	isRightImage = isRight;
}

void	CVirSignalItem::setDescEmpty(bool isDEmpty)
{
	isDescEmpty = isDEmpty;
	m_toolTipItem = new CToolTipItem("images/modify_sign.png");
	mainScene->addItem(m_toolTipItem);
}

void	CVirSignalItem::setToolTipPos(int xPos, int yPos)
{
	QPointF imagePos;
	if (isRightImage)
	{
		imagePos = QPointF(xPos + m_Width - 6, yPos - 7);
	} 
	else
	{
		imagePos = QPointF(xPos - 7, yPos - 7);
	}
	if (isDescEmpty)
	{
		imagePos = QPointF(xPos + m_Width - 7, yPos + 10);
	}
	if (m_toolTipItem != NULL)
	{
		m_toolTipItem->setPos(imagePos);
	}
}

void	CVirSignalItem::appendAssoID(QString assoID)
{
	m_assoIDs.append(assoID);
}

QVector<QString> CVirSignalItem::getAssoIDs()
{
	return m_assoIDs;
}

int CVirSignalItem::getHeight()
{
	return m_Height;
}

void CVirSignalItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
	m_Rect=QRectF(0,0,m_Width,m_Height);
	QRectF rect=boundingRect();
	QPen pen(QColor(155,155,155));
	QBrush bursh;
	if(!m_IsInRect)
		bursh=QColor(247,252,255);
	else
		bursh=QColor(253,217,0);
	painter->setPen(pen);
	painter->setBrush(bursh);
	painter->drawRoundedRect(rect,2,2);

    // 标记序号, 对于输入信号不展示
    if (m_Ord != 0)
    {
        QBrush ordBursh;
        ordBursh=QColor(182, 235, 251);
        QRectF ordRect = QRectF(rect.x() + 7, rect.y() + 3, 26, m_Height-5);
        painter->setBrush(ordBursh);
        painter->drawRoundedRect(ordRect, 2, 2);
        painter->drawText(ordRect, QString::number(m_Ord), Qt::AlignVCenter|Qt::AlignHCenter);
    }

	QPen textPen(QColor(0,0,0));
	QFont textFont;
	QString textFontName = QObject::tr("宋体");
	textFont.setFamily(textFontName);
	textFont.setPointSize(20);
	textFont.setPixelSize(12);
	painter->setFont(textFont);
	painter->setPen(textPen);

    QString signalInfo = m_VirSignalName;
    if (m_SignalPath.size() > 0)
    {
        signalInfo = "[" + m_SignalPath + "]" + m_VirSignalName;
    }
    
	if(middleWay)
	{
		rect.setLeft(rect.x()+7);
		painter->drawText(rect,signalInfo,Qt::AlignVCenter|Qt::AlignLeft);
	}
	else
	{
		rect.setRight(rect.right()-7);
		painter->drawText(rect,signalInfo,Qt::AlignVCenter|Qt::AlignRight);
	}
}
QRectF CVirSignalItem::boundingRect() const
{
	return QRectF(0,0,m_Width,m_Height);
}

void CVirSignalItem::SetSignalOrd(int ord)
{
    m_Ord = ord;
}

void CVirSignalItem::SetSignalPath(QString path)
{
    // 将信号进行分解，组织成新形式
    m_SignalPath = ChangeSignalPathToNormal(path);
}

QString CVirSignalItem::ChangeSignalPathToNormal(QString path)
{
    // path 格式为IE1101&&RPIT$$GOINGGIO2$$SPCS07$$stVal
    QStringList sigList = path.split("&&");
    QString newPath = path;
    if (sigList.size() > 1)
    {
        QStringList inSigList = sigList[1].split("$$");
        if (inSigList.size() >= 3)
        {
            newPath = inSigList[0] + "/";
            for (int i = 1; i < inSigList.size() - 1; i++)
            {
                newPath += inSigList[i] + ".";
            }
            if (inSigList[inSigList.size() - 1] == "")
            {
                newPath = newPath.left(newPath.lastIndexOf('.'));
            }
            else
            {
                newPath += inSigList[inSigList.size() - 1];
            }
            
        }
    }
    return newPath;
}
