#include "MyGraphicRectItem.h"


CMyGraphicRectItem::CMyGraphicRectItem(void)
{
}

CMyGraphicRectItem::CMyGraphicRectItem(HmiGraphicsScene *hmiScene)
{
	mainScene = hmiScene;
	rectPen = QPen(Qt::gray, 2, Qt::DotLine);
	rectBrush = QBrush(Qt::white);
	rectFont.setFamily(QObject::tr("宋体"));
	rectFont.setPixelSize(9);
	m_rectWidth = 0;
	m_rectHeight = 0;
	m_rectBGColor = QiTaColor;
	m_rectName = "";
}

CMyGraphicRectItem::~CMyGraphicRectItem(void)
{
}

bool CMyGraphicRectItem::save(QDataStream &stream)
{
	stream << m_rectWidth;
	stream << m_rectHeight;
	int bgColotType = m_rectBGColor;
	stream << bgColotType;
	stream << m_rectName;
	return true;
}

bool CMyGraphicRectItem::load(QDataStream &stream)
{
	stream >> m_rectWidth;
	stream >> m_rectHeight;
	int bgColorType;
	stream >> bgColorType;
	setRectBGColor((RectTypeEnum)bgColorType);
	stream >> m_rectName;
	return true;
}

void CMyGraphicRectItem::setRectWidth(const int rectWidth)
{
	m_rectWidth = rectWidth;
}

void CMyGraphicRectItem::setRectHeight(const int rectHeight)
{
	m_rectHeight = rectHeight;
}

void CMyGraphicRectItem::setRectBGColor(RectTypeEnum colorType)
{
	m_rectBGColor = colorType;

	//所有颜色都设置为20%透明;
	// 此处从CDrawSubnetGraphicWithDiff::DrawDotLineRect调用，颜色名称存在误解
	switch (colorType)
	{
	case MuXianColor:
		rectBrush.setColor(QColor(0,0,155,20));
		break;
	case XianLuColor:
		rectBrush.setColor(QColor(0,155,0,20));
		break;
	case DuanLuQiColor:
		rectBrush.setColor(QColor(155,0,0,20));
		break;
	case DianRongQiColor:
		rectBrush.setColor(QColor(155,155,0,20));
		break;
	case DianKangColor:
		rectBrush.setColor(QColor(0,155,155,20));
		break;
	case QiTaColor:
		rectBrush.setColor(QColor(155,155,155,20));
		break;
	}
}

void CMyGraphicRectItem::setRectName(const QString& rectName)
{
	m_rectName = rectName;
}

void CMyGraphicRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget/* = 0*/)
{
	painter->setBrush(rectBrush);
	painter->setPen(rectPen);
	painter->setFont(rectFont);
	painter->drawRoundedRect(0,0,m_rectWidth,m_rectHeight,3,3);
	QPen textPen(Qt::black, 1, Qt::SolidLine);	//字体画笔;
	painter->setPen(textPen);
	QRectF textRect(0,0,m_rectWidth,16);
	painter->drawText(textRect,"   " + m_rectName,Qt::AlignLeft | Qt::AlignVCenter);
}

QRectF CMyGraphicRectItem::boundingRect() const
{
	return QRectF(0, 0, m_rectWidth, m_rectHeight);
}
