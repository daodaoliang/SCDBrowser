#include "GraphicTitleTable.h"
#include "MyGraphicTextItem.h"

CGraphicTitleTable::CGraphicTitleTable(HmiGraphicsScene *mScene)
{
	mainScene = mScene;
	xPtValue = 0;
	yPtValue = 0;
	rowHeight = 18;		//默认行高18像素;
	columnWidth = 240;	//默认列宽200像素;
	rowNum = 5;			//默认行数5行;
	fontPtSize = 9;		//默认字号9号;

	//设置字体;
	textFont = new QFont();
	textFont->setFamily(QObject::tr("宋体"));
	textFont->setPointSize(fontPtSize);
	textFont->setWeight(QFont::Normal);
	textFont->setBold(false);
	textFont->setItalic(false);
	textFont->setUnderline(false);
}

CGraphicTitleTable::~CGraphicTitleTable(void)
{
}

void CGraphicTitleTable::setOrigenPt(const int xVal, const int yVal)
{
	xPtValue = xVal;
	yPtValue = yVal;
}

void CGraphicTitleTable::setOrigenPt(const QPoint& origenPt)
{
	xPtValue = origenPt.x();
	yPtValue = origenPt.y();
}

void CGraphicTitleTable::setRowHeight(const int rHeight)
{
	rowHeight = rHeight;
}

void CGraphicTitleTable::setColumnWidth(const int cWidth)
{
	columnWidth = cWidth;
}
void CGraphicTitleTable::setRowNum(const int rNum)
{
	rowNum = rNum;
}

void CGraphicTitleTable::setFontPointSize(const int fSize)
{
	fontPtSize = fSize;
}

int CGraphicTitleTable::getRowHeight()
{
	return rowHeight;
}

int CGraphicTitleTable::getColumnWidth()
{
	return columnWidth;
}

void CGraphicTitleTable::DrawTitleInfo(const QString& masterIEDKey, const IEDTitleInfo& masterIEDInfo)
{
	//绘制标题栏框顶线;
	QGraphicsLineItem *topLine = new QGraphicsLineItem(xPtValue,yPtValue,xPtValue+columnWidth*2,yPtValue);
	mainScene->addItem(topLine);

	//绘制标题栏各行线;
	for (int i = 1; i != rowNum; i++)
	{
		QGraphicsLineItem *rowLine = new QGraphicsLineItem(xPtValue,yPtValue+rowHeight*i,xPtValue+columnWidth*2,yPtValue+rowHeight*i);
		mainScene->addItem(rowLine);
	}

	//填充标题栏表格;
	//添加标题栏标题;
	QFontMetrics fontMatrx(*textFont);
	QString graphicTitle = QString("[") + masterIEDInfo.IEDName + QString("]") + masterIEDInfo.desc + QString(" 的 信息流图");
	int titleLeng = fontMatrx.width(graphicTitle);
	QGraphicsTextItem *gTitleItem = new QGraphicsTextItem(graphicTitle);
	gTitleItem->setFont(*textFont);
	gTitleItem->setPos(xPtValue+columnWidth-titleLeng/2,yPtValue-1);	//横向居中;
	mainScene->addItem(gTitleItem);

	//填充属性;
	int valueCount = rowNum*2-1;		//属性个数;
	bool isLeftCell = true;				//是否为左边单元格;
	for (int j = 1; j != valueCount; j++)
	{
		QString IEDInfoStr;
		switch (j)
		{
		case 1:		//厂家;
			IEDInfoStr = QString("厂家 : ") + masterIEDInfo.manufac;
			break;
		case 2:		//类型;
			IEDInfoStr = QString("类型 : ") + masterIEDInfo.IEDTypeStr;
			break;
		case 3:		//型号;
			IEDInfoStr = QString("型号 : ") + masterIEDInfo.IEDModel;
			break;
		case 4:		//版本;
			IEDInfoStr = QString("版本 : ") + masterIEDInfo.confVer;
			break;
		case 5:		//CRC;
			IEDInfoStr = QString("CRC : ") + masterIEDInfo.CRCCode;
			break;
		case 6:		//采样;
			IEDInfoStr = QString("采样 : ") + masterIEDInfo.smv;
			break;
		case 7:		//GOOSE输入输出;
			IEDInfoStr = QString("GOOSE输入输出 : ") + masterIEDInfo.GooseInOut;
			break;
		case 8:		//接入子网;
			IEDInfoStr = QString("接入网 : ");
			break;
		default:
			break;
		}

		int tempRowNum = (j+1)/2;		//获取所在行数;

		QGraphicsTextItem *gValueItem = new QGraphicsTextItem(IEDInfoStr);
		gValueItem->setFont(*textFont);

		int subNetXPos = 0;
		QFontMetrics tempMetrix(*textFont);
		const int JIRRU_LENGTH = tempMetrix.width("接入网 : ") + 3;		//"接入网 : "字符串的长度;
		if (isLeftCell)
		{
			//左侧单元格坐标;
			gValueItem->setPos(xPtValue+1, yPtValue+rowHeight*tempRowNum);
			subNetXPos = xPtValue + 1 + JIRRU_LENGTH;
		}
		else
		{
			//右侧单元格坐标;
			gValueItem->setPos(xPtValue+columnWidth+1, yPtValue+rowHeight*tempRowNum);
			subNetXPos = xPtValue + columnWidth + 1 + JIRRU_LENGTH;
		}
		mainScene->addItem(gValueItem);

		if (j == 8)		//子网;
		{
			for (int k = 0; k != masterIEDInfo.subNetName.size(); k++)
			{
				//添加子网显示字图元;
				CMyGraphicTextItem *subNetNameItem = new CMyGraphicTextItem(mainScene);
				QString disName = mainScene->GetSclModel()->GetNameFromKey(masterIEDInfo.subNetName.at(k));
				subNetNameItem->setDisplayText(disName);
				subNetNameItem->setSubnetKey(masterIEDInfo.subNetName.at(k));
				mainScene->addItem(subNetNameItem);
				if (subNetXPos + subNetNameItem->getDisplayTextLength() + 10 > xPtValue + columnWidth*2)//换行;
				{
					rowNum++;
					tempRowNum++;
					subNetXPos = xPtValue + columnWidth + 1 + JIRRU_LENGTH;
				}
				subNetNameItem->setPos(subNetXPos, yPtValue+rowHeight*tempRowNum);
				subNetXPos += subNetNameItem->getDisplayTextLength() + 10;
			}
		}
		isLeftCell = !isLeftCell;
	}

	//绘制标题栏框底线;
	QGraphicsLineItem *buttomLine = new QGraphicsLineItem(xPtValue,yPtValue+rowHeight*rowNum,xPtValue+columnWidth*2,yPtValue+rowHeight*rowNum);
	mainScene->addItem(buttomLine);

	//绘制标题栏框左线;
	QGraphicsLineItem *leftLine = new QGraphicsLineItem(xPtValue,yPtValue,xPtValue,yPtValue+rowHeight*rowNum);
	mainScene->addItem(leftLine);

	//绘制标题栏框右线;
	QGraphicsLineItem *rightLine = new QGraphicsLineItem(xPtValue+columnWidth*2,yPtValue,xPtValue+columnWidth*2,yPtValue+rowHeight*rowNum);
	mainScene->addItem(rightLine);

	//绘制标题栏框中竖线;
	QGraphicsLineItem *middleLine = new QGraphicsLineItem(xPtValue+columnWidth,yPtValue+rowHeight,xPtValue+columnWidth,yPtValue+rowHeight*rowNum);
	mainScene->addItem(middleLine);
}