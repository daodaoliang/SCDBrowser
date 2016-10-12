#include "DrawIEDSelf.h"
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include "cvirtualterminallineitem.h"
#include "csofttripitem.h"
#include "copticalfilberitem.h"

CDrawIEDSelf::CDrawIEDSelf(HmiGraphicsScene *scene,QMenu *menu,QString &name,VTList &list)
{
	mainScene = scene;
	itemMenu = menu;

	m_nSpace = 50;
	m_nLength = 800;
	m_vtList = list;
	m_strIED_Name = name;
}

CDrawIEDSelf::~CDrawIEDSelf(void)
{
}

void CDrawIEDSelf::Draw()
{
	int count = m_vtList.size();
	QRectF	rectFrame(100,100,m_nLength,m_nSpace * (count + 1));
	QGraphicsRectItem *frameItem = new QGraphicsRectItem(rectFrame);
	mainScene->addItem(frameItem);
	frameItem->setSelected(true);
	frameItem->setFlags(/*QGraphicsItem::ItemIsMovable | */QGraphicsItem::ItemIsSelectable);

	QGraphicsLineItem *lineItem = new QGraphicsLineItem(QLine(100,100+m_nSpace*2,100+m_nLength,100+m_nSpace*2));
	mainScene->addItem(lineItem);
	
	QGraphicsTextItem *titleItem = new QGraphicsTextItem("李山线合并单元A");
	titleItem->setPos(QPoint(m_nLength/2,100+m_nSpace/2));
	QFont *textFont = new QFont();
	textFont->setFamily(QObject::tr("宋体"));
	textFont->setPointSize(22);
	textFont->setWeight(QFont::Normal);
	textFont->setItalic(false);
	textFont->setUnderline(false);
	titleItem->setFont(*textFont);
	mainScene->addItem(titleItem);

	for (int i = 1;i < count-1;i ++)
	{
		CVirtualTerminalLineItem *termItem;
		if (i > 3 && i < 14)
		{
			termItem = new CVirtualTerminalLineItem(itemMenu,
				QPointF(120,100+m_nSpace*2 + i*m_nSpace),QPointF(120+m_nLength,100+m_nSpace*2 + i*m_nSpace),
				mainScene);
		}
		else
		{
			termItem = new CVirtualTerminalLineItem(itemMenu,
				QPointF(120,100+m_nSpace*2 + i*m_nSpace),QPointF(120+m_nLength+50,100+m_nSpace*2 + i*m_nSpace),
				mainScene);
		}
		mainScene->addItem(termItem);
		termItem->strName = (*m_vtList.at(i)).strName;
		termItem->strDesc = (*m_vtList.at(i)).strDesc;
		termItem->strPath = (*m_vtList.at(i)).strPath;
		termItem->strUri = (*m_vtList.at(i)).strUri;
		//termItem->setSelected(true);
		//termItem->setFlags(QGraphicsItem::ItemIsSelectable);

		if (i < 4 || i >= 14)
		{
			//绘制软压板
			CSoftTripItem *tripItem = new CSoftTripItem(itemMenu,0,mainScene);
			mainScene->addItem(tripItem);
			tripItem->setPos(QPointF(120+m_nLength+50,100+m_nSpace*2 + i*m_nSpace));

			if (i == 1 || i == 14)
			{
				QGraphicsLineItem *lineItem1 = new QGraphicsLineItem(
					QLine(120+m_nLength+50+40,100+m_nSpace*2 + i*m_nSpace,
						  120+m_nLength+50+40+50,100+m_nSpace*2 + i*m_nSpace));
				mainScene->addItem(lineItem1);
				if (i == 1)//绘制纵向光纤 
				{
					COpticalFilberItem *fiberItem = new COpticalFilberItem(itemMenu,
						QPointF(120+m_nLength+50+40+50,100+m_nSpace*2 + i*m_nSpace),
						QPointF(120+m_nLength+50+40+50,100+m_nSpace*2 + i*m_nSpace+1000),
						mainScene);
					mainScene->addItem(fiberItem);
					//绘制物理端口
					CSoftTripItem *phyItem = new CSoftTripItem(itemMenu,0,mainScene);
					mainScene->addItem(phyItem);
					phyItem->setPos(QPointF(120+m_nLength+50+40+50-20,100+m_nSpace*2 + i*m_nSpace+1000+60));
				}
			}
			else if (i == 2 || i == 15)
			{
				QGraphicsLineItem *lineItem2 = new QGraphicsLineItem(
					QLine(120+m_nLength+50+40,100+m_nSpace*2 + i*m_nSpace,
						  120+m_nLength+50+40+100,100+m_nSpace*2 + i*m_nSpace));
				mainScene->addItem(lineItem2);
				if (i == 2)//绘制纵向光纤 
				{
					COpticalFilberItem *fiberItem = new COpticalFilberItem(itemMenu,
						QPointF(120+m_nLength+50+40+100,100+m_nSpace*2 + i*m_nSpace),
						QPointF(120+m_nLength+50+40+100,100+m_nSpace*2 + i*m_nSpace+1000-m_nSpace),
						mainScene);
					mainScene->addItem(fiberItem);
					//绘制物理端口
					CSoftTripItem *phyItem = new CSoftTripItem(itemMenu,0,mainScene);
					mainScene->addItem(phyItem);
					phyItem->setPos(QPointF(120+m_nLength+50+40+100-20,100+m_nSpace*2 + i*m_nSpace+1000-m_nSpace+60));
				}
			}
			else if (i == 3 || i == 16)
			{
				QGraphicsLineItem *lineItem3 = new QGraphicsLineItem(
					QLine(120+m_nLength+50+40,100+m_nSpace*2 + i*m_nSpace,
						  120+m_nLength+50+40+150,100+m_nSpace*2 + i*m_nSpace));
				mainScene->addItem(lineItem3);
				if (i == 3)//绘制纵向光纤 
				{
					COpticalFilberItem *fiberItem = new COpticalFilberItem(itemMenu,
						QPointF(120+m_nLength+50+40+150,100+m_nSpace*2 + i*m_nSpace),
						QPointF(120+m_nLength+50+40+150,100+m_nSpace*2 + i*m_nSpace+1000-m_nSpace*2),
						mainScene);
					mainScene->addItem(fiberItem);
					//绘制物理端口
					CSoftTripItem *phyItem = new CSoftTripItem(itemMenu,0,mainScene);
					mainScene->addItem(phyItem);
					phyItem->setPos(QPointF(120+m_nLength+50+40+150-20,100+m_nSpace*2 + i*m_nSpace+1000-m_nSpace*2+60));
				}
			}
		}
	}
}