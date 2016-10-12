#include "VoltageLevelRectItem.h"
#include "VoltageLevelAnalysor.h"

CVoltageLevelRectItem::CVoltageLevelRectItem(HmiGraphicsScene *hmiScene)
{
	mainScene = hmiScene;
	m_VoltageLevel = -1;
	m_VoltageLevelName = "";
	m_BrushClr = QColor(Qt::white);
	m_Width = 0;
	m_Height = 0;

	QString textFontName = QObject::tr("宋体");
	textFont.setFamily(textFontName);
	textFont.setPixelSize(12);
	textFont.setBold(true);		//加粗;
}

CVoltageLevelRectItem::~CVoltageLevelRectItem(void)
{
}

void CVoltageLevelRectItem::setVoltageLevel(int voltageLevel)
{
	m_VoltageLevel = voltageLevel;
}

void CVoltageLevelRectItem::setItemWidth(int width)
{
	m_Width = width;
}

void CVoltageLevelRectItem::setItemHeight(int height)
{
	m_Height = height;
}

void CVoltageLevelRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	//绘制矩形;
	QRectF itemRect = boundingRect();
	painter->setBrush(QBrush(m_BrushClr));
	painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
	painter->drawRoundedRect(itemRect, 3, 3);

	//绘制显示字;
	painter->setPen(QPen(m_TextClr, 1, Qt::SolidLine));
	painter->drawText(itemRect, m_VoltageLevelName, Qt::AlignHCenter | Qt::AlignVCenter);
}

QRectF CVoltageLevelRectItem::boundingRect() const
{
	return QRectF(0, 0, m_Width, m_Height);
}

bool CVoltageLevelRectItem::save(QDataStream &stream)
{
	stream << m_VoltageLevel;
	stream << m_Width;
	stream << m_Height;
	return true;
}

bool CVoltageLevelRectItem::load(QDataStream &stream)
{
	stream >> m_VoltageLevel;
	setBrushColorByVoltageLevel();
	setVoltageLevelNameByVoltageLevel();
	stream >> m_Width;
	stream >> m_Height;
	return true;
}

void CVoltageLevelRectItem::setBrushColorByVoltageLevel()
{
	switch (m_VoltageLevel)
	{
	case VOLTAGELEVEL_MAINTRANS:
		m_BrushClr = QColor("#82c84a");		//淡绿;
		m_TextClr = QColor(Qt::black);
		break;
	case VOLTAGELEVEL_OTHERS:
		m_BrushClr = QColor("#ed7da2");		//淡红;
		m_TextClr = QColor(Qt::black);
		break;
	case VOLTAGELEVEL_6KV:
		m_BrushClr = QColor("#0000ff");		//蓝色;
		m_TextClr = QColor(Qt::white);
		break;
	case VOLTAGELEVEL_10KV:
		m_BrushClr = QColor("#A52A2A");		//绛红;
		m_TextClr = QColor(Qt::white);
		break;
	case VOLTAGELEVEL_35KV:
		m_BrushClr = QColor("#ffff00");		//鲜黄;
		m_TextClr = QColor(Qt::black);
		break;
	case VOLTAGELEVEL_66KV:
		m_BrushClr = QColor(255,128,0);		//橙黄;
		m_TextClr = QColor(Qt::black);
		break;
	case VOLTAGELEVEL_110KV:
		m_BrushClr = QColor(240,65,85);		//朱红;
		m_TextClr = QColor(Qt::black);
		break;
	case VOLTAGELEVEL_220KV:
		m_BrushClr = QColor(139,0,255);		//紫色;
		m_TextClr = QColor(Qt::white);
		break;
	case VOLTAGELEVEL_330KV:
		m_BrushClr = QColor("#FFFFFF");		//纯白;
		m_TextClr = QColor(Qt::black);
		break;
	case VOLTAGELEVEL_500KV:
		m_BrushClr = QColor("#F7EED6");		//米黄;
		m_TextClr = QColor(Qt::black);
		break;
	case VOLTAGELEVEL_750KV:
		m_BrushClr = QColor("#1fc2c0");		//蓝绿(非标准色);
		m_TextClr = QColor(Qt::black);
		break;
	case VOLTAGELEVEL_1000KV:
		m_BrushClr = QColor("#0000cd");		//中蓝;
		m_TextClr = QColor(Qt::white);
		break;
	}
}

void CVoltageLevelRectItem::setVoltageLevelNameByVoltageLevel()
{
	switch (m_VoltageLevel)
	{
	case VOLTAGELEVEL_MAINTRANS:
		m_VoltageLevelName = "主变";
		break;
	case VOLTAGELEVEL_OTHERS:
		m_VoltageLevelName = "其他";
		break;
	case VOLTAGELEVEL_6KV:
		m_VoltageLevelName = "6kV";
		break;
	case VOLTAGELEVEL_10KV:
		m_VoltageLevelName = "10kV";
		break;
	case VOLTAGELEVEL_35KV:
		m_VoltageLevelName = "35kV";
		break;
	case VOLTAGELEVEL_66KV:
		m_VoltageLevelName = "66kV";
		break;
	case VOLTAGELEVEL_110KV:
		m_VoltageLevelName = "110kV";
		break;
	case VOLTAGELEVEL_220KV:
		m_VoltageLevelName = "220kV";
		break;
	case VOLTAGELEVEL_330KV:
		m_VoltageLevelName = "330kV";
		break;
	case VOLTAGELEVEL_500KV:
		m_VoltageLevelName = "500kV";
		break;
	case VOLTAGELEVEL_750KV:
		m_VoltageLevelName = "750kV";
		break;
	case VOLTAGELEVEL_1000KV:
		m_VoltageLevelName = "1000kV";
		break;
	}
}
