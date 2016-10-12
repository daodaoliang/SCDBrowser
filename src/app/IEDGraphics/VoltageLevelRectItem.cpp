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

	QString textFontName = QObject::tr("����");
	textFont.setFamily(textFontName);
	textFont.setPixelSize(12);
	textFont.setBold(true);		//�Ӵ�;
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
	//���ƾ���;
	QRectF itemRect = boundingRect();
	painter->setBrush(QBrush(m_BrushClr));
	painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
	painter->drawRoundedRect(itemRect, 3, 3);

	//������ʾ��;
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
		m_BrushClr = QColor("#82c84a");		//����;
		m_TextClr = QColor(Qt::black);
		break;
	case VOLTAGELEVEL_OTHERS:
		m_BrushClr = QColor("#ed7da2");		//����;
		m_TextClr = QColor(Qt::black);
		break;
	case VOLTAGELEVEL_6KV:
		m_BrushClr = QColor("#0000ff");		//��ɫ;
		m_TextClr = QColor(Qt::white);
		break;
	case VOLTAGELEVEL_10KV:
		m_BrushClr = QColor("#A52A2A");		//筺�;
		m_TextClr = QColor(Qt::white);
		break;
	case VOLTAGELEVEL_35KV:
		m_BrushClr = QColor("#ffff00");		//�ʻ�;
		m_TextClr = QColor(Qt::black);
		break;
	case VOLTAGELEVEL_66KV:
		m_BrushClr = QColor(255,128,0);		//�Ȼ�;
		m_TextClr = QColor(Qt::black);
		break;
	case VOLTAGELEVEL_110KV:
		m_BrushClr = QColor(240,65,85);		//���;
		m_TextClr = QColor(Qt::black);
		break;
	case VOLTAGELEVEL_220KV:
		m_BrushClr = QColor(139,0,255);		//��ɫ;
		m_TextClr = QColor(Qt::white);
		break;
	case VOLTAGELEVEL_330KV:
		m_BrushClr = QColor("#FFFFFF");		//����;
		m_TextClr = QColor(Qt::black);
		break;
	case VOLTAGELEVEL_500KV:
		m_BrushClr = QColor("#F7EED6");		//�׻�;
		m_TextClr = QColor(Qt::black);
		break;
	case VOLTAGELEVEL_750KV:
		m_BrushClr = QColor("#1fc2c0");		//����(�Ǳ�׼ɫ);
		m_TextClr = QColor(Qt::black);
		break;
	case VOLTAGELEVEL_1000KV:
		m_BrushClr = QColor("#0000cd");		//����;
		m_TextClr = QColor(Qt::white);
		break;
	}
}

void CVoltageLevelRectItem::setVoltageLevelNameByVoltageLevel()
{
	switch (m_VoltageLevel)
	{
	case VOLTAGELEVEL_MAINTRANS:
		m_VoltageLevelName = "����";
		break;
	case VOLTAGELEVEL_OTHERS:
		m_VoltageLevelName = "����";
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
