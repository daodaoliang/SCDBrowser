#include "GraphicTitleTable.h"
#include "MyGraphicTextItem.h"

CGraphicTitleTable::CGraphicTitleTable(HmiGraphicsScene *mScene)
{
	mainScene = mScene;
	xPtValue = 0;
	yPtValue = 0;
	rowHeight = 18;		//Ĭ���и�18����;
	columnWidth = 240;	//Ĭ���п�200����;
	rowNum = 5;			//Ĭ������5��;
	fontPtSize = 9;		//Ĭ���ֺ�9��;

	//��������;
	textFont = new QFont();
	textFont->setFamily(QObject::tr("����"));
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
	//���Ʊ���������;
	QGraphicsLineItem *topLine = new QGraphicsLineItem(xPtValue,yPtValue,xPtValue+columnWidth*2,yPtValue);
	mainScene->addItem(topLine);

	//���Ʊ�����������;
	for (int i = 1; i != rowNum; i++)
	{
		QGraphicsLineItem *rowLine = new QGraphicsLineItem(xPtValue,yPtValue+rowHeight*i,xPtValue+columnWidth*2,yPtValue+rowHeight*i);
		mainScene->addItem(rowLine);
	}

	//�����������;
	//��ӱ���������;
	QFontMetrics fontMatrx(*textFont);
	QString graphicTitle = QString("[") + masterIEDInfo.IEDName + QString("]") + masterIEDInfo.desc + QString(" �� ��Ϣ��ͼ");
	int titleLeng = fontMatrx.width(graphicTitle);
	QGraphicsTextItem *gTitleItem = new QGraphicsTextItem(graphicTitle);
	gTitleItem->setFont(*textFont);
	gTitleItem->setPos(xPtValue+columnWidth-titleLeng/2,yPtValue-1);	//�������;
	mainScene->addItem(gTitleItem);

	//�������;
	int valueCount = rowNum*2-1;		//���Ը���;
	bool isLeftCell = true;				//�Ƿ�Ϊ��ߵ�Ԫ��;
	for (int j = 1; j != valueCount; j++)
	{
		QString IEDInfoStr;
		switch (j)
		{
		case 1:		//����;
			IEDInfoStr = QString("���� : ") + masterIEDInfo.manufac;
			break;
		case 2:		//����;
			IEDInfoStr = QString("���� : ") + masterIEDInfo.IEDTypeStr;
			break;
		case 3:		//�ͺ�;
			IEDInfoStr = QString("�ͺ� : ") + masterIEDInfo.IEDModel;
			break;
		case 4:		//�汾;
			IEDInfoStr = QString("�汾 : ") + masterIEDInfo.confVer;
			break;
		case 5:		//CRC;
			IEDInfoStr = QString("CRC : ") + masterIEDInfo.CRCCode;
			break;
		case 6:		//����;
			IEDInfoStr = QString("���� : ") + masterIEDInfo.smv;
			break;
		case 7:		//GOOSE�������;
			IEDInfoStr = QString("GOOSE������� : ") + masterIEDInfo.GooseInOut;
			break;
		case 8:		//��������;
			IEDInfoStr = QString("������ : ");
			break;
		default:
			break;
		}

		int tempRowNum = (j+1)/2;		//��ȡ��������;

		QGraphicsTextItem *gValueItem = new QGraphicsTextItem(IEDInfoStr);
		gValueItem->setFont(*textFont);

		int subNetXPos = 0;
		QFontMetrics tempMetrix(*textFont);
		const int JIRRU_LENGTH = tempMetrix.width("������ : ") + 3;		//"������ : "�ַ����ĳ���;
		if (isLeftCell)
		{
			//��൥Ԫ������;
			gValueItem->setPos(xPtValue+1, yPtValue+rowHeight*tempRowNum);
			subNetXPos = xPtValue + 1 + JIRRU_LENGTH;
		}
		else
		{
			//�Ҳ൥Ԫ������;
			gValueItem->setPos(xPtValue+columnWidth+1, yPtValue+rowHeight*tempRowNum);
			subNetXPos = xPtValue + columnWidth + 1 + JIRRU_LENGTH;
		}
		mainScene->addItem(gValueItem);

		if (j == 8)		//����;
		{
			for (int k = 0; k != masterIEDInfo.subNetName.size(); k++)
			{
				//���������ʾ��ͼԪ;
				CMyGraphicTextItem *subNetNameItem = new CMyGraphicTextItem(mainScene);
				QString disName = mainScene->GetSclModel()->GetNameFromKey(masterIEDInfo.subNetName.at(k));
				subNetNameItem->setDisplayText(disName);
				subNetNameItem->setSubnetKey(masterIEDInfo.subNetName.at(k));
				mainScene->addItem(subNetNameItem);
				if (subNetXPos + subNetNameItem->getDisplayTextLength() + 10 > xPtValue + columnWidth*2)//����;
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

	//���Ʊ����������;
	QGraphicsLineItem *buttomLine = new QGraphicsLineItem(xPtValue,yPtValue+rowHeight*rowNum,xPtValue+columnWidth*2,yPtValue+rowHeight*rowNum);
	mainScene->addItem(buttomLine);

	//���Ʊ�����������;
	QGraphicsLineItem *leftLine = new QGraphicsLineItem(xPtValue,yPtValue,xPtValue,yPtValue+rowHeight*rowNum);
	mainScene->addItem(leftLine);

	//���Ʊ�����������;
	QGraphicsLineItem *rightLine = new QGraphicsLineItem(xPtValue+columnWidth*2,yPtValue,xPtValue+columnWidth*2,yPtValue+rowHeight*rowNum);
	mainScene->addItem(rightLine);

	//���Ʊ�������������;
	QGraphicsLineItem *middleLine = new QGraphicsLineItem(xPtValue+columnWidth,yPtValue+rowHeight,xPtValue+columnWidth,yPtValue+rowHeight*rowNum);
	mainScene->addItem(middleLine);
}