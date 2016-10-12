#include "csofttripitem.h"
#include <QtGui>

CSoftTripItem::CSoftTripItem()
{
	setFlags(QGraphicsItem::ItemIsSelectable);
}

CSoftTripItem::CSoftTripItem( QMenu *contextMenu,
						 QGraphicsItem *parent, QGraphicsScene *scene)
						 : QGraphicsItem(parent, scene)
{
	myContextMenu = contextMenu;
	mainScene = dynamic_cast<HmiGraphicsScene *>(scene);

	setFlags(QGraphicsItem::ItemIsSelectable);
}

CSoftTripItem::~CSoftTripItem()
{

}

void CSoftTripItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	mainScene->clearSelection();
	setSelected(true);
	myContextMenu->exec(event->screenPos());
}
/** 
  * @brief    mousePressEvent
  * 
  * 无作用
  * 
  * @class    CSoftTripItem
  * @param    QGraphicsSceneMouseEvent *event :
  * @return   void
  * @author   zhang-xch
  * @date     2015/3/20
  * @remarks  
  */ 
void CSoftTripItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	
}

bool CSoftTripItem::GetStateFlag()
{
	return bCloseFlag;
}

void CSoftTripItem::SetStateFlag(bool flag)
{
	bCloseFlag = flag;
}

void CSoftTripItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->drawImage(QPointF(0,0), QImage("images/SoftTripItem.png"));
}

bool CSoftTripItem::load(QDataStream &stream)
{
	stream >> bCloseFlag;
	QPointF pt;
	stream >> pt;
	setPos(pt);
	stream >> strKeyValue;
	setToolTip(strKeyValue);
	return true;
}

bool CSoftTripItem::save(QDataStream &stream)
{
	stream << bCloseFlag;
	QPointF pt;
	pt = pos();
	stream << pt;
	stream << strKeyValue;
	return true;
}

QRectF CSoftTripItem::boundingRect() const
{
	return QRectF(0,0,34,14);
}

void CSoftTripItem::SetKeyValue(QString str)
{
    QString path = ChangeSignalPathToNormal(str);
	strKeyValue = path;
	setToolTip(strKeyValue);
}

QString CSoftTripItem::GetKeyValue()
{
	return strKeyValue;
}

QString CSoftTripItem::ChangeSignalPathToNormal(QString path)
{
    // path 格式为RPIT$$GOINGGIO2$$SPCS07$$stVal
    QStringList inSigList = path.split("$$");
    QString newPath = path;
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
    return newPath;
}
