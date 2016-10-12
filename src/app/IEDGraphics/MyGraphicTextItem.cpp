#include "MyGraphicTextItem.h"


CMyGraphicTextItem::CMyGraphicTextItem(void)
{
}

CMyGraphicTextItem::CMyGraphicTextItem(HmiGraphicsScene *hmiScene)
{
	setAcceptHoverEvents(true);
	mainScene = hmiScene;
	m_Text = "";
	m_SubnetKey = "";
	textPen.setColor(Qt::blue);
	textFont.setFamily(QObject::tr("宋体"));
	textFont.setItalic(true);		//斜体;
	textFont.setUnderline(true);	//下划线;
}

CMyGraphicTextItem::~CMyGraphicTextItem(void)
{
}

bool CMyGraphicTextItem::save(QDataStream &stream)
{
	stream << m_Text;
	stream << m_SubnetKey;
	stream << m_FontSize;
	return true;
}

bool CMyGraphicTextItem::load(QDataStream &stream)
{
	stream >> m_Text;
	stream >> m_SubnetKey;
	stream >> m_FontSize;
	return true;
}

void CMyGraphicTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		mainScene->procInfos(0, m_SubnetKey, "", 0);	//单击跳转到子网图;
	}
}

void CMyGraphicTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}

void CMyGraphicTextItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	setCursor(Qt::PointingHandCursor);	//鼠标指针变为手型;
}

void CMyGraphicTextItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	setCursor(Qt::ArrowCursor);			//鼠标指针变为箭头;
}

void CMyGraphicTextItem::setDisplayText(const QString& disText)
{
	m_Text = disText;
}

void CMyGraphicTextItem::setSubnetKey(const QString& subnetKey)
{
	m_SubnetKey = subnetKey;
}

void CMyGraphicTextItem::setFontSize(const int fontSize)
{
	m_FontSize = fontSize;
}

int CMyGraphicTextItem::getDisplayTextLength()
{
	QFontMetrics fontMatrx(textFont);
	int disTextWidth = fontMatrx.width(m_Text);
	return disTextWidth;
}

QRectF CMyGraphicTextItem::boundingRect() const
{
	QFontMetrics fontMatrx(textFont);
	int disTextWidth = fontMatrx.width(m_Text) + 5;
	int disTextHeight = fontMatrx.height() + 5;
	return QRectF(0, 0, disTextWidth, disTextHeight);
}

void CMyGraphicTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */)
{
	QRectF textRect = boundingRect();
	painter->setPen(textPen);
	painter->setFont(textFont);
	painter->drawText(textRect, m_Text, Qt::AlignLeft | Qt::AlignVCenter);
}
