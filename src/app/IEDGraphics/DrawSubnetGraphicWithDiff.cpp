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

	m_IEDItemWidth = 170;	//默认IED尺寸为170X50像素;
	m_IEDItemHeight = 50;
	m_VtlItemWidth = 80;	//默认电压等级图元尺寸为80X25像素;
	m_VtlItemHeight = 25;
	leftBorder = 20;		//默认距左边20像素;
	topBorder = 40;			//默认距顶边40像素;
	m_rowDis = 50;			//默认行间距25像素;
	m_columnDis = 80;		//默认列间距80像素;
	m_rectMargin = 17;		//虚线框边缘,约等于行间距的三分之一;
	m_columnCount = 0;
	m_maxRowCount = 0;
	
	//获取IED列表;
	mainScene->GetGraphicModel()->GetIEDListBySubNetKey(subNetKey, &m_subIEDs);
	sortSubIEDs();			//根据类型排序;
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

	const int circleRadius = 20;	//圆半径,定值;

	//绘制子网头与线;
	DrawSubnetLine();

	//计算IED起始坐标;
	int IEDXPos = leftBorder + m_rectMargin;
	int IEDYPos = topBorder + circleRadius + m_VtlItemHeight + m_rowDis*2;
	
	//分电压等级绘制IED;
	DrawSubnetIEDs(mainTransIEDs, VOLTAGELEVEL_MAINTRANS, IEDXPos, IEDYPos);	//主变;
	DrawSubnetIEDs(IEDs_1000kV, VOLTAGELEVEL_1000KV, IEDXPos, IEDYPos);			//1000千伏;
	DrawSubnetIEDs(IEDs_750kV, VOLTAGELEVEL_750KV, IEDXPos, IEDYPos);			//750千伏;
	DrawSubnetIEDs(IEDs_500kV, VOLTAGELEVEL_500KV, IEDXPos, IEDYPos);			//500千伏;
	DrawSubnetIEDs(IEDs_330kV, VOLTAGELEVEL_330KV, IEDXPos, IEDYPos);			//330千伏;
	DrawSubnetIEDs(IEDs_220kV, VOLTAGELEVEL_220KV, IEDXPos, IEDYPos);			//220千伏;
	DrawSubnetIEDs(IEDs_110kV, VOLTAGELEVEL_110KV, IEDXPos, IEDYPos);			//110千伏;
	DrawSubnetIEDs(IEDs_66kV, VOLTAGELEVEL_66KV, IEDXPos, IEDYPos);				//66千伏;
	DrawSubnetIEDs(IEDs_35kV, VOLTAGELEVEL_35KV, IEDXPos, IEDYPos);				//35千伏;
	DrawSubnetIEDs(IEDs_10kV, VOLTAGELEVEL_10KV, IEDXPos, IEDYPos);				//10千伏;
	DrawSubnetIEDs(IEDs_6kV, VOLTAGELEVEL_6KV, IEDXPos, IEDYPos);				//6千伏;
	DrawSubnetIEDs(otherTransIEDs, VOLTAGELEVEL_OTHERS, IEDXPos, IEDYPos);		//其他;

	//重置绘图区域;
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

	int graphicWidth = leftBorder + tempColumnCount*(m_columnDis + m_IEDItemWidth);	//获取图宽;
	int graphicHeight = topBorder + tempRowCount*(m_rowDis + m_IEDItemHeight) + m_rowDis;	//获取图高;

	QRectF recScene = mainScene->sceneRect();
	recScene.setRight(graphicWidth + 50);
	recScene.setBottom(graphicHeight + 100);
	mainScene->setSceneRect(recScene);
}

void CDrawSubnetGraphicWithDiff::DrawSubnetLine()
{
	//获取子网名称;
	QString subNetName = sclNameTranslator->GetNameFromKey(m_subNetKey.toLocal8Bit().constData()).c_str();

	//绘制子网图图头;
	CSubnetItem *snItem = new CSubnetItem(NULL,mainScene,3);
	snItem->setSubNetName(subNetName);
	mainScene->addItem(snItem);
	snItem->setPos(leftBorder, topBorder);

	if (m_subIEDs.count() == 0)
	{
		return;
	}

	const int circleRadius = 20;	//圆形半径,定值;

	//设置画笔;
	QPen linePen(QColor(229, 229, 229), Qt::SolidLine);
	linePen.setWidth(2);

	//计算横线长度;
	//公式 ; (列数-1)*(IED宽度+列间距) + IED宽度/2;
	int hLineLength = (m_columnCount - 1)*(m_IEDItemWidth + m_columnDis) + m_IEDItemWidth/2 + m_rectMargin;

	//画横线;
	int hLineXPos = leftBorder;					//横线起点X坐标;
	int hLineYPos = topBorder + circleRadius;	//横线起点Y坐标;
	QGraphicsLineItem *horizontalLine = new QGraphicsLineItem(hLineXPos, hLineYPos, hLineXPos + hLineLength, hLineYPos);
	horizontalLine->setPen(linePen);
	mainScene->addItem(horizontalLine);

	//画竖线;
	int vLineXPos = leftBorder + m_IEDItemWidth/2 + m_rectMargin;	//竖线起点X坐标;
	int vLineYPos = topBorder + circleRadius;						//竖线起点Y坐标;

	QVector<int> *tempVec = NULL;
	for (int i = 0; i != 12; i++)	//遍历12个电压等级数组;
	{
		tempVec = GetVectorPoint(i);	//获取相应数组;
		if (tempVec->count() == 0)
		{
			continue;
		}

		//计算竖线长度;
		//公式 : 行数*(IED高度+行间距) + 电压等级图元高度 + 行间距;
		int vLineLength = (m_IEDItemHeight + m_rowDis)*tempVec->count() + m_VtlItemHeight + m_rowDis;

		//绘制竖线;
		QGraphicsLineItem *verticalLine = new QGraphicsLineItem(vLineXPos, vLineYPos, vLineXPos, vLineYPos + vLineLength);
		verticalLine->setPen(linePen);
		mainScene->addItem(verticalLine);

		//更新横坐标;
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
	int topYPos = yPos;	//存储顶部IED纵坐标;

	//计算电压等级图元位置;
	int vltXPos = xPos + (m_IEDItemWidth - m_VtlItemWidth)/2;
	int vltYPos = yPos - m_rowDis - m_VtlItemHeight;

	//添加电压等级图元;
	CVoltageLevelRectItem *vltItem = new CVoltageLevelRectItem(mainScene);
	vltItem->setVoltageLevel(vltName);
	vltItem->setItemWidth(m_VtlItemWidth);
	vltItem->setItemHeight(m_VtlItemHeight);
	mainScene->addItem(vltItem);
	vltItem->setPos(vltXPos, vltYPos);

	IEDType lastIEDType = NO;						//上一个IED类型;
	int typeRectWidth = m_IEDItemWidth + 2*m_rectMargin;	//虚线框宽度;
	int lastTypeSum = 1;									//上一类型IED数量;
	int typeRectXPos = xPos - m_rectMargin;					//虚线框初始横坐标;
	int typeRectYPos = yPos - m_rectMargin;					//虚线框初始纵坐标;

	//绘制IED;
	for (int i = 0; i != IEDsVec.size(); i++)
	{
		//绘制IED,返回该IED类型;
		IEDType thisIEDType = DrawOneIEDItem(m_subIEDs.at(IEDsVec.at(i)), xPos, yPos);

		if (i != 0)	//第一次遍历不做处理;
		{
			if (thisIEDType != lastIEDType)	//如果当前IED与前一IED类型不同;
			{
				//绘制虚线框;
				DrawDotLineRect(lastTypeSum, typeRectWidth, lastIEDType, typeRectXPos, typeRectYPos);
				typeRectYPos = yPos - m_rectMargin;		//更新虚线框纵坐标;
				lastTypeSum = 1;						//前一类型数目归一;
			}
			else							//如果当前IED与前一IED类型相同;
			{
				lastTypeSum++;	//前一类型数目递增;
			}
		}
		if (i == IEDsVec.size() - 1)		//最后一个IED;
		{
			//绘制最终虚线框;
			DrawDotLineRect(lastTypeSum, typeRectWidth, lastIEDType, typeRectXPos, typeRectYPos);
		}

		lastIEDType = thisIEDType;
		yPos = yPos + m_IEDItemHeight + m_rowDis;	//纵坐标下移;
	}

	//重置横纵坐标;
	xPos = xPos + m_IEDItemWidth + m_columnDis;
	yPos = topYPos;
}

IEDType CDrawSubnetGraphicWithDiff::DrawOneIEDItem(const QString& iedKey, int xPos, int yPos)
{
	QIEDInfo iedInfo;
	mainScene->GetGraphicModel()->GetIEDInfo(iedKey, iedInfo);
	//QString iedDesc = QString("[") + iedInfo.name + QString("]") + iedInfo.desc;	//拼IED名字和描述;

	CIEDSubnetItem *iedItem = new CIEDSubnetItem(mainScene);
	
	//设置IED图元属性;
	iedItem->setWidth(m_IEDItemWidth);
	iedItem->setHeight(m_IEDItemHeight);
	iedItem->setIEDName("[" + iedInfo.name + "]");
	iedItem->setIEDDesc(iedInfo.desc);
	iedItem->setMasterKey(iedKey);
	iedItem->setLinkType(link_type_msgflow);	//跳转到信息流图;

	IEDTypeAnalysor typeAnalysor;
	IEDType iedType =  typeAnalysor.parseIEDType(iedInfo.name, iedInfo.desc);
	iedItem->setIEDType(iedType);	//设置IED类型;

	mainScene->addItem(iedItem);
	iedItem->setPos(xPos, yPos);

	return iedType;
}

void CDrawSubnetGraphicWithDiff::DrawDotLineRect(const int typeIEDSum, int rectWidth, IEDType iedType, int xPos, int yPos)
{
	//计算虚线框高度;
	//公式 : 虚线框边缘*2 + 该类型IED数量*(IED高度+行间距) - 行间距;
	int typeRectHeight = 2*m_rectMargin + typeIEDSum*(m_IEDItemHeight + m_rowDis) - m_rowDis;

	CMyGraphicRectItem *recItem = new CMyGraphicRectItem(mainScene);
	
	//设置属性;
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

	//各类型键值对;
	//Key   : m_subIEDs中的序号;
	//value : QIEDInfo;
	QMap<int, QIEDInfo> PIEDs;		//保护;
	QMap<int, QIEDInfo> CIEDs;		//测控;
	QMap<int, QIEDInfo> STIEDs;		//智能终端;
	QMap<int, QIEDInfo> MUIEDs;		//合并单元;
	QMap<int, QIEDInfo> MSIEDs;		//合并单元与智能终端一体装置;
	QMap<int, QIEDInfo> PCIEDs;		//保测一体装置;
	QMap<int, QIEDInfo> NOIEDs;		//其他无类型装置;

	QIEDInfo iedInfo;
	IEDTypeAnalysor iedTypeAnalysor;

	for (int i = 0; i != m_subIEDs.size(); i++)	//遍历m_subIEDs;
	{
		mainScene->GetGraphicModel()->GetIEDInfo(m_subIEDs.at(i), iedInfo);	//获取IED信息;
		IEDType iedType = iedTypeAnalysor.parseIEDType(iedInfo.name, iedInfo.desc);	//分析装置类型;
		switch (iedType)
		{
		case P:		//保护;
			PIEDs.insert(i, iedInfo);
			break;
		case C:		//测控;
			CIEDs.insert(i, iedInfo);
			break;
		case ST:	//智能终端;
			STIEDs.insert(i, iedInfo);
			break;
		case MU:	//合并;
			MUIEDs.insert(i, iedInfo);
			break;
		case MS:	//智合;
			MSIEDs.insert(i, iedInfo);
			break;
		case PC:	//保测;
			PCIEDs.insert(i, iedInfo);
			break;
		case NO:	//其他;
			NOIEDs.insert(i, iedInfo);
			break;
		}
	}

	sortSubIEDsByVoltageLevel(PIEDs);	//分发保护装置;
	sortSubIEDsByVoltageLevel(CIEDs);	//分发测控装置;
	sortSubIEDsByVoltageLevel(STIEDs);	//分发智能终端;
	sortSubIEDsByVoltageLevel(MUIEDs);	//分发合并单元;
	sortSubIEDsByVoltageLevel(MSIEDs);	//分发智合一体;
	sortSubIEDsByVoltageLevel(PCIEDs);	//分发保测一体;
	sortSubIEDsByVoltageLevel(NOIEDs);	//分发其他装置;

	GetColumnCount();	//获取列数;
	GetMaxRowCount();	//获取行数;
}

void CDrawSubnetGraphicWithDiff::sortSubIEDsByVoltageLevel(QMap<int, QIEDInfo>& iedMap)
{
	if (iedMap.isEmpty())
	{
		return;
	}

	CVoltageLevelAnalysor vtlAnalysor;		//电压等级分析器;
	QMap<int, QIEDInfo>::iterator mapIter;	//QMap迭代器;
	for (mapIter = iedMap.begin(); mapIter != iedMap.end(); mapIter++)	//遍历Map;
	{
		//获取电压等级;
		int vtlType = vtlAnalysor.analysisVoltageLevelByIED(mapIter.value().name, mapIter.value().desc);
		switch (vtlType)
		{
		case VOLTAGELEVEL_MAINTRANS:	//主变;
			mainTransIEDs.append(mapIter.key());
			break;
		case VOLTAGELEVEL_OTHERS:		//其他;
			otherTransIEDs.append(mapIter.key());
			break;
		case VOLTAGELEVEL_6KV:			//6千伏;
			IEDs_6kV.append(mapIter.key());
			break;
		case VOLTAGELEVEL_10KV:			//10千伏;
			IEDs_10kV.append(mapIter.key());
			break;
		case VOLTAGELEVEL_35KV:			//35千伏;
			IEDs_35kV.append(mapIter.key());
			break;
		case VOLTAGELEVEL_66KV:			//66千伏;
			IEDs_66kV.append(mapIter.key());
			break;
		case VOLTAGELEVEL_110KV:		//110千伏;
			IEDs_110kV.append(mapIter.key());
			break;
		case VOLTAGELEVEL_220KV:		//220千伏;
			IEDs_220kV.append(mapIter.key());
			break;
		case VOLTAGELEVEL_330KV:		//330千伏;
			IEDs_330kV.append(mapIter.key());
			break;
		case VOLTAGELEVEL_500KV:		//500千伏;
			IEDs_500kV.append(mapIter.key());
			break;
		case VOLTAGELEVEL_750KV:		//750千伏;
			IEDs_750kV.append(mapIter.key());
			break;
		case VOLTAGELEVEL_1000KV:		//1000千伏;
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
