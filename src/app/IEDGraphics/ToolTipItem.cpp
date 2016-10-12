#include "ToolTipItem.h"
#include <QtGui>


CToolTipItem::CToolTipItem(const QString imagePath)//, const QRectF rect)
{
	m_imagePath = imagePath;
	diffWidgetList.clear();
}

CToolTipItem::CToolTipItem(void)
{
}

CToolTipItem::~CToolTipItem(void)
{
}

void CToolTipItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
{
	painter->drawImage(QPointF(0,0), QImage(m_imagePath));
}

QRectF CToolTipItem::boundingRect() const
{
	return QRectF(0,0,18,18);//m_rect;
}

void CToolTipItem::setPath(QString imagePath)
{
	m_imagePath = imagePath;
}

void CToolTipItem::setDiffInfo(QString diffInfo)
{
	m_diffInfo = diffInfo;
}

bool CToolTipItem::save(QDataStream &stream)
{
	stream << m_imagePath;
	stream << m_diffInfo;
	return true;
}

bool CToolTipItem::load(QDataStream &stream)
{
	stream >> m_imagePath;
	stream >> m_diffInfo;
	return true;
}

void CToolTipItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		for(int i=0;i<diffWidgetList.count();i++)
		{
			if(diffWidgetList.at(i)->compareInfo(m_diffInfo))
				return;
		}
		CDiffInfoDialog *diffDlg = new CDiffInfoDialog();
		connect(diffDlg,SIGNAL(CLose()),SLOT(diffwidgetClose()));
		diffDlg->setDiffInfo(m_diffInfo);
		diffDlg->show();
		diffWidgetList.append(diffDlg);
	}
}

void CToolTipItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}
void CToolTipItem::diffwidgetClose()
{
	for(int i=0;i<diffWidgetList.count();i++)
	{
		if(diffWidgetList.at(i)==sender())
			diffWidgetList.removeAt(i);
	}
}