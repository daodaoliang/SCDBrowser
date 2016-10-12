#include "DrawSubnetGraphicWithDiff.h"
#include "SubnetItem.h"
#include "IEDSubnetItem.h"
#include "iedtypeanalysor.h"
#include "VoltageLevelAnalysor.h"
#include "VoltageLevelRectItem.h"
#include "MyGraphicRectItem.h"

CDrawSubnetGraphicWithDiff::CDrawSubnetGraphicWithDiff(void)
{
}

CDrawSubnetGraphicWithDiff::CDrawSubnetGraphicWithDiff(HmiGraphicsScene *scene, const QString &subNetKey)
{
	mainScene = scene;
	m_subNetKey = subNetKey;

	sclNameTranslator = new SCLNameTranslator();

	m_IEDItemWidth = 170;	//Ĭ��IED�ߴ�Ϊ170X50����;
	m_IEDItemHeight = 50;
	m_VtlItemWidth = 80;	//Ĭ�ϵ�ѹ�ȼ�ͼԪ�ߴ�Ϊ80X25����;
	m_VtlItemHeight = 25;
	leftBorder = 20;		//Ĭ�Ͼ����20����;
	topBorder = 40;			//Ĭ�Ͼඥ��40����;
	m_rowDis = 50;			//Ĭ���м��25����;
	m_columnDis = 80;		//Ĭ���м��80����;
	m_rectMargin = 17;		//���߿��Ե,Լ�����м�������֮һ;
	m_columnCount = 0;
	m_maxRowCount = 0;
	
	//��ȡIED�б�;
	mainScene->GetGraphicModel()->GetIEDListBySubNetKey(subNetKey, &m_subIEDs);
	sortSubIEDs();			//������������;
}

CDrawSubnetGraphicWithDiff::~CDrawSubnetGraphicWithDiff(void)
{
	delete sclNameTranslator;
}

void CDrawSubnetGraphicWithDiff::setRowDistance(int rowDis)
{
	m_rowDis = rowDis;
}

void CDrawSubnetGraphicWithDiff::setColumnDistance(int columnDis)
{
	m_columnDis = columnDis;
}

void CDrawSubnetGraphicWithDiff::DrawSubnetGraphic()
{
	if (m_subIEDs.size() == 0)
	{
		return;
	}

	const int circleRadius = 20;	//Բ�뾶,��ֵ;

	//��������ͷ����;
	DrawSubnetLine();

	//����IED��ʼ����;
	int IEDXPos = leftBorder + m_rectMargin;
	int IEDYPos = topBorder + circleRadius + m_VtlItemHeight + m_rowDis*2;
	
	//�ֵ�ѹ�ȼ�����IED;
	DrawSubnetIEDs(mainTransIEDs, VOLTAGELEVEL_MAINTRANS, IEDXPos, IEDYPos);	//����;
	DrawSubnetIEDs(IEDs_1000kV, VOLTAGELEVEL_1000KV, IEDXPos, IEDYPos);			//1000ǧ��;
	DrawSubnetIEDs(IEDs_750kV, VOLTAGELEVEL_750KV, IEDXPos, IEDYPos);			//750ǧ��;
	DrawSubnetIEDs(IEDs_500kV, VOLTAGELEVEL_500KV, IEDXPos, IEDYPos);			//500ǧ��;
	DrawSubnetIEDs(IEDs_330kV, VOLTAGELEVEL_330KV, IEDXPos, IEDYPos);			//330ǧ��;
	DrawSubnetIEDs(IEDs_220kV, VOLTAGELEVEL_220KV, IEDXPos, IEDYPos);			//220ǧ��;
	DrawSubnetIEDs(IEDs_110kV, VOLTAGELEVEL_110KV, IEDXPos, IEDYPos);			//110ǧ��;
	DrawSubnetIEDs(IEDs_66kV, VOLTAGELEVEL_66KV, IEDXPos, IEDYPos);				//66ǧ��;
	DrawSubnetIEDs(IEDs_35kV, VOLTAGELEVEL_35KV, IEDXPos, IEDYPos);				//35ǧ��;
	DrawSubnetIEDs(IEDs_10kV, VOLTAGELEVEL_10KV, IEDXPos, IEDYPos);				//10ǧ��;
	DrawSubnetIEDs(IEDs_6kV, VOLTAGELEVEL_6KV, IEDXPos, IEDYPos);				//6ǧ��;
	DrawSubnetIEDs(otherTransIEDs, VOLTAGELEVEL_OTHERS, IEDXPos, IEDYPos);		//����;

	//���û�ͼ����;
	int tempRowCount = m_maxRowCount;
	int tempColumnCount = m_columnCount;
	if (m_columnCount < 4)
	{
		tempColumnCount = 4;
	}
	if (m_maxRowCount < 6)
	{
		tempRowCount = 6;
	}

	int graphicWidth = leftBorder + tempColumnCount*(m_columnDis + m_IEDItemWidth);	//��ȡͼ��;
	int graphicHeight = topBorder + tempRowCount*(m_rowDis + m_IEDItemHeight) + m_rowDis;	//��ȡͼ��;

	QRectF recScene = mainScene->sceneRect();
	recScene.setRight(graphicWidth + 50);
	recScene.setBottom(graphicHeight + 100);
	mainScene->setSceneRect(recScene);
}

void CDrawSubnetGraphicWithDiff::DrawSubnetLine()
{
	//��ȡ��������;
	QString subNetName = sclNameTranslator->GetNameFromKey(m_subNetKey.toLocal8Bit().constData()).c_str();

	//��������ͼͼͷ;
	CSubnetItem *snItem = new CSubnetItem(NULL,mainScene,3);
	snItem->setSubNetName(subNetName);
	mainScene->addItem(snItem);
	snItem->setPos(leftBorder, topBorder);

	if (m_subIEDs.count() == 0)
	{
		return;
	}

	const int circleRadius = 20;	//Բ�ΰ뾶,��ֵ;

	//���û���;
	QPen linePen(QColor(229, 229, 229), Qt::SolidLine);
	linePen.setWidth(2);

	//������߳���;
	//��ʽ ; (����-1)*(IED���+�м��) + IED���/2;
	int hLineLength = (m_columnCount - 1)*(m_IEDItemWidth + m_columnDis) + m_IEDItemWidth/2 + m_rectMargin;

	//������;
	int hLineXPos = leftBorder;					//�������X����;
	int hLineYPos = topBorder + circleRadius;	//�������Y����;
	QGraphicsLineItem *horizontalLine = new QGraphicsLineItem(hLineXPos, hLineYPos, hLineXPos + hLineLength, hLineYPos);
	horizontalLine->setPen(linePen);
	mainScene->addItem(horizontalLine);

	//������;
	int vLineXPos = leftBorder + m_IEDItemWidth/2 + m_rectMargin;	//�������X����;
	int vLineYPos = topBorder + circleRadius;						//�������Y����;

	QVector<int> *tempVec = NULL;
	for (int i = 0; i != 12; i++)	//����12����ѹ�ȼ�����;
	{
		tempVec = GetVectorPoint(i);	//��ȡ��Ӧ����;
		if (tempVec->count() == 0)
		{
			continue;
		}

		//�������߳���;
		//��ʽ : ����*(IED�߶�+�м��) + ��ѹ�ȼ�ͼԪ�߶� + �м��;
		int vLineLength = (m_IEDItemHeight + m_rowDis)*tempVec->count() + m_VtlItemHeight + m_rowDis;

		//��������;
		QGraphicsLineItem *verticalLine = new QGraphicsLineItem(vLineXPos, vLineYPos, vLineXPos, vLineYPos + vLineLength);
		verticalLine->setPen(linePen);
		mainScene->addItem(verticalLine);

		//���º�����;
		vLineXPos = vLineXPos + m_IEDItemWidth + m_columnDis;
	}
}

void CDrawSubnetGraphicWithDiff::DrawSubnetIEDs(QVector<int>& IEDsVec, int vltName, int& xPos, int& yPos)
{
	if (IEDsVec.size() == 0)
	{
		return;
	}

	IEDTypeAnalysor iedTypeAnalysor;
	int topYPos = yPos;	//�洢����IED������;

	//�����ѹ�ȼ�ͼԪλ��;
	int vltXPos = xPos + (m_IEDItemWidth - m_VtlItemWidth)/2;
	int vltYPos = yPos - m_rowDis - m_VtlItemHeight;

	//��ӵ�ѹ�ȼ�ͼԪ;
	CVoltageLevelRectItem *vltItem = new CVoltageLevelRectItem(mainScene);
	vltItem->setVoltageLevel(vltName);
	vltItem->setItemWidth(m_VtlItemWidth);
	vltItem->setItemHeight(m_VtlItemHeight);
	mainScene->addItem(vltItem);
	vltItem->setPos(vltXPos, vltYPos);

	IEDType lastIEDType = NO;						//��һ��IED����;
	int typeRectWidth = m_IEDItemWidth + 2*m_rectMargin;	//���߿���;
	int lastTypeSum = 1;									//��һ����IED����;
	int typeRectXPos = xPos - m_rectMargin;					//���߿��ʼ������;
	int typeRectYPos = yPos - m_rectMargin;					//���߿��ʼ������;

	//����IED;
	for (int i = 0; i != IEDsVec.size(); i++)
	{
		//����IED,���ظ�IED����;
		IEDType thisIEDType = DrawOneIEDItem(m_subIEDs.at(IEDsVec.at(i)), xPos, yPos);

		if (i != 0)	//��һ�α�����������;
		{
			if (thisIEDType != lastIEDType)	//�����ǰIED��ǰһIED���Ͳ�ͬ;
			{
				//�������߿�;
				DrawDotLineRect(lastTypeSum, typeRectWidth, lastIEDType, typeRectXPos, typeRectYPos);
				typeRectYPos = yPos - m_rectMargin;		//�������߿�������;
				lastTypeSum = 1;						//ǰһ������Ŀ��һ;
			}
			else							//�����ǰIED��ǰһIED������ͬ;
			{
				lastTypeSum++;	//ǰһ������Ŀ����;
			}
		}
		if (i == IEDsVec.size() - 1)		//���һ��IED;
		{
			//�����������߿�;
			DrawDotLineRect(lastTypeSum, typeRectWidth, lastIEDType, typeRectXPos, typeRectYPos);
		}

		lastIEDType = thisIEDType;
		yPos = yPos + m_IEDItemHeight + m_rowDis;	//����������;
	}

	//���ú�������;
	xPos = xPos + m_IEDItemWidth + m_columnDis;
	yPos = topYPos;
}

IEDType CDrawSubnetGraphicWithDiff::DrawOneIEDItem(const QString& iedKey, int xPos, int yPos)
{
	QIEDInfo iedInfo;
	mainScene->GetGraphicModel()->GetIEDInfo(iedKey, iedInfo);
	//QString iedDesc = QString("[") + iedInfo.name + QString("]") + iedInfo.desc;	//ƴIED���ֺ�����;

	CIEDSubnetItem *iedItem = new CIEDSubnetItem(mainScene);
	
	//����IEDͼԪ����;
	iedItem->setWidth(m_IEDItemWidth);
	iedItem->setHeight(m_IEDItemHeight);
	iedItem->setIEDName("[" + iedInfo.name + "]");
	iedItem->setIEDDesc(iedInfo.desc);
	iedItem->setMasterKey(iedKey);
	iedItem->setLinkType(link_type_msgflow);	//��ת����Ϣ��ͼ;

	IEDTypeAnalysor typeAnalysor;
	IEDType iedType =  typeAnalysor.parseIEDType(iedInfo.name, iedInfo.desc);
	iedItem->setIEDType(iedType);	//����IED����;

	mainScene->addItem(iedItem);
	iedItem->setPos(xPos, yPos);

	return iedType;
}

void CDrawSubnetGraphicWithDiff::DrawDotLineRect(const int typeIEDSum, int rectWidth, IEDType iedType, int xPos, int yPos)
{
	//�������߿�߶�;
	//��ʽ : ���߿��Ե*2 + ������IED����*(IED�߶�+�м��) - �м��;
	int typeRectHeight = 2*m_rectMargin + typeIEDSum*(m_IEDItemHeight + m_rowDis) - m_rowDis;

	CMyGraphicRectItem *recItem = new CMyGraphicRectItem(mainScene);
	
	//��������;
	recItem->setRectWidth(rectWidth);
	recItem->setRectHeight(typeRectHeight);
	switch (iedType)
	{
	case P:
		recItem->setRectBGColor(MuXianColor);
		break;
	case ST:
		recItem->setRectBGColor(DianKangColor);
		break;
	case PC:
		recItem->setRectBGColor(DianRongQiColor);
		break;
	case C:
		recItem->setRectBGColor(DuanLuQiColor);
		break;
	case MS:
		recItem->setRectBGColor(QiTaColor);
		break;
	case MU:
		recItem->setRectBGColor(XianLuColor);
		break;
	case NO:
		break;
	}
	mainScene->addItem(recItem);
	recItem->setPos(xPos, yPos);
}

void CDrawSubnetGraphicWithDiff::sortSubIEDs()
{
	if (m_subIEDs.count() == 0)
	{
		return;
	}

	//�����ͼ�ֵ��;
	//Key   : m_subIEDs�е����;
	//value : QIEDInfo;
	QMap<int, QIEDInfo> PIEDs;		//����;
	QMap<int, QIEDInfo> CIEDs;		//���;
	QMap<int, QIEDInfo> STIEDs;		//�����ն�;
	QMap<int, QIEDInfo> MUIEDs;		//�ϲ���Ԫ;
	QMap<int, QIEDInfo> MSIEDs;		//�ϲ���Ԫ�������ն�һ��װ��;
	QMap<int, QIEDInfo> PCIEDs;		//����һ��װ��;
	QMap<int, QIEDInfo> NOIEDs;		//����������װ��;

	QIEDInfo iedInfo;
	IEDTypeAnalysor iedTypeAnalysor;

	for (int i = 0; i != m_subIEDs.size(); i++)	//����m_subIEDs;
	{
		mainScene->GetGraphicModel()->GetIEDInfo(m_subIEDs.at(i), iedInfo);	//��ȡIED��Ϣ;
		IEDType iedType = iedTypeAnalysor.parseIEDType(iedInfo.name, iedInfo.desc);	//����װ������;
		switch (iedType)
		{
		case P:		//����;
			PIEDs.insert(i, iedInfo);
			break;
		case C:		//���;
			CIEDs.insert(i, iedInfo);
			break;
		case ST:	//�����ն�;
			STIEDs.insert(i, iedInfo);
			break;
		case MU:	//�ϲ�;
			MUIEDs.insert(i, iedInfo);
			break;
		case MS:	//�Ǻ�;
			MSIEDs.insert(i, iedInfo);
			break;
		case PC:	//����;
			PCIEDs.insert(i, iedInfo);
			break;
		case NO:	//����;
			NOIEDs.insert(i, iedInfo);
			break;
		}
	}

	sortSubIEDsByVoltageLevel(PIEDs);	//�ַ�����װ��;
	sortSubIEDsByVoltageLevel(CIEDs);	//�ַ����װ��;
	sortSubIEDsByVoltageLevel(STIEDs);	//�ַ������ն�;
	sortSubIEDsByVoltageLevel(MUIEDs);	//�ַ��ϲ���Ԫ;
	sortSubIEDsByVoltageLevel(MSIEDs);	//�ַ��Ǻ�һ��;
	sortSubIEDsByVoltageLevel(PCIEDs);	//�ַ�����һ��;
	sortSubIEDsByVoltageLevel(NOIEDs);	//�ַ�����װ��;

	GetColumnCount();	//��ȡ����;
	GetMaxRowCount();	//��ȡ����;
}

void CDrawSubnetGraphicWithDiff::sortSubIEDsByVoltageLevel(QMap<int, QIEDInfo>& iedMap)
{
	if (iedMap.isEmpty())
	{
		return;
	}

	CVoltageLevelAnalysor vtlAnalysor;		//��ѹ�ȼ�������;
	QMap<int, QIEDInfo>::iterator mapIter;	//QMap������;
	for (mapIter = iedMap.begin(); mapIter != iedMap.end(); mapIter++)	//����Map;
	{
		//��ȡ��ѹ�ȼ�;
		int vtlType = vtlAnalysor.analysisVoltageLevelByIED(mapIter.value().name, mapIter.value().desc);
		switch (vtlType)
		{
		case VOLTAGELEVEL_MAINTRANS:	//����;
			mainTransIEDs.append(mapIter.key());
			break;
		case VOLTAGELEVEL_OTHERS:		//����;
			otherTransIEDs.append(mapIter.key());
			break;
		case VOLTAGELEVEL_6KV:			//6ǧ��;
			IEDs_6kV.append(mapIter.key());
			break;
		case VOLTAGELEVEL_10KV:			//10ǧ��;
			IEDs_10kV.append(mapIter.key());
			break;
		case VOLTAGELEVEL_35KV:			//35ǧ��;
			IEDs_35kV.append(mapIter.key());
			break;
		case VOLTAGELEVEL_66KV:			//66ǧ��;
			IEDs_66kV.append(mapIter.key());
			break;
		case VOLTAGELEVEL_110KV:		//110ǧ��;
			IEDs_110kV.append(mapIter.key());
			break;
		case VOLTAGELEVEL_220KV:		//220ǧ��;
			IEDs_220kV.append(mapIter.key());
			break;
		case VOLTAGELEVEL_330KV:		//330ǧ��;
			IEDs_330kV.append(mapIter.key());
			break;
		case VOLTAGELEVEL_500KV:		//500ǧ��;
			IEDs_500kV.append(mapIter.key());
			break;
		case VOLTAGELEVEL_750KV:		//750ǧ��;
			IEDs_750kV.append(mapIter.key());
			break;
		case VOLTAGELEVEL_1000KV:		//1000ǧ��;
			IEDs_1000kV.append(mapIter.key());
			break;
		}
	}
}

void CDrawSubnetGraphicWithDiff::GetColumnCount()
{
	if (mainTransIEDs.count() != 0)
	{
		m_columnCount++;
	}
	if (otherTransIEDs.count() != 0)
	{
		m_columnCount++;
	}
	if (IEDs_6kV.count() != 0)
	{
		m_columnCount++;
	}
	if (IEDs_10kV.count() != 0)
	{
		m_columnCount++;
	}
	if (IEDs_35kV.count() != 0)
	{
		m_columnCount++;
	}
	if (IEDs_66kV.count() != 0)
	{
		m_columnCount++;
	}
	if (IEDs_110kV.count() != 0)
	{
		m_columnCount++;
	}
	if (IEDs_220kV.count() != 0)
	{
		m_columnCount++;
	}
	if (IEDs_330kV.count() != 0)
	{
		m_columnCount++;
	}
	if (IEDs_500kV.count() != 0)
	{
		m_columnCount++;
	}
	if (IEDs_750kV.count() != 0)
	{
		m_columnCount++;
	}
	if (IEDs_1000kV.count() != 0)
	{
		m_columnCount++;
	}
}

void CDrawSubnetGraphicWithDiff::GetMaxRowCount()
{
	m_maxRowCount = qMax(mainTransIEDs.count(), otherTransIEDs.count());
	m_maxRowCount = qMax(m_maxRowCount, IEDs_6kV.count());
	m_maxRowCount = qMax(m_maxRowCount, IEDs_10kV.count());
	m_maxRowCount = qMax(m_maxRowCount, IEDs_35kV.count());
	m_maxRowCount = qMax(m_maxRowCount, IEDs_66kV.count());
	m_maxRowCount = qMax(m_maxRowCount, IEDs_110kV.count());
	m_maxRowCount = qMax(m_maxRowCount, IEDs_220kV.count());
	m_maxRowCount = qMax(m_maxRowCount, IEDs_330kV.count());
	m_maxRowCount = qMax(m_maxRowCount, IEDs_500kV.count());
	m_maxRowCount = qMax(m_maxRowCount, IEDs_750kV.count());
	m_maxRowCount = qMax(m_maxRowCount, IEDs_1000kV.count());
}

QVector<int>* CDrawSubnetGraphicWithDiff::GetVectorPoint(int index)
{
	QVector<int> *tempP;
	switch (index)
	{
	case 0:
		tempP = &mainTransIEDs;
		break;
	case 1:
		tempP = &IEDs_1000kV;
		break;
	case 2:
		tempP = &IEDs_750kV;
		break;
	case 3:
		tempP = &IEDs_500kV;
		break;
	case 4:
		tempP = &IEDs_330kV;
		break;
	case 5:
		tempP = &IEDs_220kV;
		break;
	case 6:
		tempP = &IEDs_110kV;
		break;
	case 7:
		tempP = &IEDs_66kV;
		break;
	case 8:
		tempP = &IEDs_35kV;
		break;
	case 9:
		tempP = &IEDs_10kV;
		break;
	case 10:
		tempP = &IEDs_6kV;
		break;
	case 11:
		tempP = &otherTransIEDs;
		break;
	}
	return tempP;
}
