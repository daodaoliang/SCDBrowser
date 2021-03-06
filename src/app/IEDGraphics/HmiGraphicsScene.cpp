#include "HmiGraphicsScene.h"
#include "csofttripitem.h"
#include <QSvgGenerator>
#include "SCLModelSvrAdapter.h"//chenxin add
#include "DrawSubnetGraphicWithDiff.h"
#include "DrawInfoFlowGraphicWithDiff.h"
#include "DrawLoopGraphicWithDiff.h"
#include "DrawTerminalGraphicWithDiff.h"

#include "SubnetItem.h"
#include "IedItem.h"
#include "MyGraphItem.h"
#include "MyGraphicTextItem.h"
#include "MyGraphicRectItem.h"
#include "IEDMasterItem.h"
#include "IEDSlaveItem.h"
#include "IEDSubnetItem.h"
#include "CtrlBlockItem.h"
#include "VirLinkItem.h"
#include "VirSignalItem.h"
#include "PhyLinkVirLinkItem.h"
#include "PhyPortItem.h"
#include "VoltageLevelRectItem.h"

#define		SERIALIZE_TYPE_OpticalFilber		1
#define		SERIALIZE_TYPE_VirtualTerminalLine	2
#define		SERIALIZE_TYPE_SoftTrip				3
#define		SERIALIZE_TYPE_COMMON_RECT			4
#define		SERIALIZE_TYPE_COMMON_LINE			5
#define		SERIALIZE_TYPE_COMMON_TEXT			6
#define		SERIALIZE_TYPE_COMMON_PLINE			7
#define		SERIALIZE_TYPE_SUBNET_ITEM			8
#define		SERIALIZE_TYPE_ACCESSPOINT_ITEM		9
#define		SERIALIZE_TYPE_IED_ITEM				10
#define		SERIALIZE_TYPE_IEDDETAIL_ITEM		11
#define		SERIALIZE_TYPE_LINK_ITEM			12
#define		SERIZLIZE_TYPE_HOTPOINT_ITEM		13
#define		SERIZLIZE_TYPE_VIRTUAL_TERM_ITEM	14
#define		SERIALIZE_TYPE_MASTERIED_ITEM		15
#define		SERIALIZE_TYPE_SVLINK_ITEM			16
#define		SERIALIZE_TYPE_VLHEADER_ITEM		17
#define		SERIALIZE_TYPE_TOOLTIP_ITEM			18
#define		SERIALIZE_TYPE_PIXMAP_ITEM			19
#define		SERIALIZE_TYPE_IEDMASTER_ITEM		20
#define		SERIALIZE_TYPE_IEDSLAVE_ITEM		21
#define		SERIALIZE_TYPE_CTRL_BLOCK_ITEM		22
#define		SERIALIZE_TYPE_VIRLINK_ITEM			23
#define		SERIALIZE_TYPE_VIRSIGNAL_ITEM		24
#define		SERIALIZE_TYPE_PHYVIRLINK_ITEM		25
#define		SERIALIZE_TYPE_PHYPORT_ITEM			26
#define		SERIALIZE_TYPE_IEDSUBNET_ITEM		27
#define		SERIALIZE_TYPE_VOLTAGELEVELRECT_ITEM	28
#define		SERIALIZE_TYPE_MYTEXT_ITEM			29
#define		SERIALIZE_TYPE_MYRECT_ITEM			30

#define		GRAPHIC_ITEM_CLICK_MSG_INDEX		0
#define		GRAPHIC_ITEM_CLICK_MSG_TRIP         1	

HmiGraphicsScene::HmiGraphicsScene()
	: QGraphicsScene()
{
	
}

HmiGraphicsScene::HmiGraphicsScene(QMenu *itemMenu,const QRectF &Rect,QObject *parent, bool isDiff)
: QGraphicsScene(parent)
{
	setSceneRect(Rect);
	myItemMenu = itemMenu;
	currentItem = 0;
	isDrag = false;
	SCLModel   = SCLModelAdapter::GetInstance();
	SCDDiffCompare *scdDifCompare = SCDDiffCompare::GetInstance();
	GraphicSCLModel = new CGraphicSCLModelImpl();
	GraphicDiffModel = new GraphicModelDiffImpl(scdDifCompare);
	if (isDiff)
	{
		GraphicModel = GraphicDiffModel;
	}
	else
	{
		GraphicModel = GraphicSCLModel;
	}
}

HmiGraphicsScene::HmiGraphicsScene(const QRectF &Rect,QObject *parent, SCDDiffCompare *scdDiffer, bool isDiff)
: QGraphicsScene(parent)
{
	setSceneRect(Rect);
	currentItem = 0;
	SCLModel   = SCLModelAdapter::GetInstance();
	GraphicSCLModel = new CGraphicSCLModelImpl();
	GraphicDiffModel = new GraphicModelDiffImpl(scdDiffer);
	if (isDiff)
	{
		GraphicModel = GraphicDiffModel;
	}
	else
	{
		GraphicModel = GraphicSCLModel;
	}
}

HmiGraphicsScene::~HmiGraphicsScene()
{

}

void HmiGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	if (mouseEvent->button() == Qt::LeftButton)
	{
		QList<QGraphicsItem *> allItems = items();
		for (int i = 0; i != allItems.size(); i++)
		{
			CMyGraphItem *tempItem = dynamic_cast<CMyGraphItem *>(allItems.at(i));
			if (tempItem == NULL)
			{
				continue;
			}
			tempItem->highLightCancel();
		}
	}

	QGraphicsScene::mousePressEvent(mouseEvent);
}

void HmiGraphicsScene::procInfos(int msgType,QString masterKey, QString slaveKey, int type)
{
	if (msgType == GRAPHIC_ITEM_CLICK_MSG_INDEX)
	{
		switch (type)
		{
		case link_type_msgflow:
			DrawMsgFlowProc(masterKey);
			break;
		case link_type_vloop_goose:
			DrawVirtualLoopProc(masterKey,slaveKey,typeGoose_);
			break;
		case link_type_vloop_sv:
			DrawVirtualLoopProc(masterKey,slaveKey,typeSV_);
			break;
		case link_type_vterm:
			DrawTerminalProc(masterKey);
			break;
		case link_type_subnet:
			DrawSubnetProc(masterKey);
			break;
		default:
			break;
		}
	}
}

void HmiGraphicsScene::setDrag(bool isD)
{
	isDrag = isD;
}

bool HmiGraphicsScene::getDrag()
{
	return isDrag;
}

void HmiGraphicsScene::assoHighLight(const QString srcItemID, const QVector<QString> assoIDs)
{
	QList<QGraphicsItem *> allItems = items();

	for (int i = 0; i != allItems.size(); i++)
	{
		bool isAsso = false;
		CMyGraphItem *tempItem = dynamic_cast<CMyGraphItem *>(allItems.at(i));
		if (tempItem == NULL)
		{
			continue;
		}
		QString s1 = tempItem->getItemID();
		for (int j = 0; j != assoIDs.size(); j++)
		{
			QString s2 = assoIDs.at(j);
			
			if (s1 == s2)
			{
				isAsso = true;
				tempItem->alwaysHighLight();
				break;
			}
		}
		if (srcItemID == s1)
		{
			isAsso = true;
		}
		if (!isAsso)
		{
			tempItem->highLightCancel();
		}
	}
}

void HmiGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void HmiGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void HmiGraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent  *event)
{
	QGraphicsItem *item = itemAt(event->scenePos());
	if (item)
	{
		currentItem = item;
	}
	QGraphicsScene::contextMenuEvent(event);
}

void HmiGraphicsScene::SwitchOpen()
{
	if (currentItem)
	{
		CSoftTripItem *trip = qgraphicsitem_cast<CSoftTripItem *>(currentItem);
		if (trip)
		{
			trip->SetStateFlag(false);
			update();
		}
	}
}

void HmiGraphicsScene::SwitchClose()
{
	if (currentItem)
	{
		CSoftTripItem *trip = dynamic_cast<CSoftTripItem *>(currentItem);
		if (trip)
		{
			trip->SetStateFlag(true);
			update();
		}
	}
}

bool HmiGraphicsScene::ExportSvgProc()
{
	QString path;
	QString newPath = "C:\\test1.svg";

	if (newPath.isEmpty())
		return false;

	path = newPath;

	//![configure SVG generator]
	QSvgGenerator generator;
	generator.setFileName(path);
	generator.setSize(QSize(sceneRect().width(),sceneRect().height()));
	generator.setViewBox(sceneRect());
	generator.setTitle(tr("SVG Generator Example Drawing"));
	generator.setDescription(tr("An SVG drawing created by the SVG Generator "
		"Example provided with Qt."));

	QPainter painter;
	painter.begin(&generator);
	render(&painter);
	painter.end();

	return true;
}

bool HmiGraphicsScene::save(QDataStream &stream)
{
	QList<QGraphicsItem *> list = items();
	int count = list.size();
	stream << count;
	QList<QGraphicsItem *>::const_iterator it;
	for (it = list.begin();it != list.end(); ++it)
	{
		CSoftTripItem *p3 = dynamic_cast<CSoftTripItem *>(*it);
		if (p3)
		{
			stream << SERIALIZE_TYPE_SoftTrip;
			p3->save(stream);
			continue;
		}
		QGraphicsRectItem *p4 = dynamic_cast<QGraphicsRectItem*>(*it);
		if (p4)
		{
			stream << SERIALIZE_TYPE_COMMON_RECT;
			QRectF rc = p4->rect();
			stream << p4->rect().left();
			stream << p4->rect().top();
			stream << p4->rect().right() - p4->rect().left();
			stream << p4->rect().bottom() - p4->rect().top();
			QBrush bru = p4->brush();
			QColor bruClr = bru.color();
			stream << bruClr;
			QPen pen = p4->pen();
			QColor clr = pen.color();
			int width = pen.width();
			stream << clr;
			stream << width;

			continue;
		}
		QGraphicsLineItem *p5 = dynamic_cast<QGraphicsLineItem*>(*it);
		if (p5)
		{
			stream << SERIALIZE_TYPE_COMMON_LINE;
			stream << p5->line().p1();
			stream << p5->line().p2();
			QPen pen = p5->pen();
			QColor clr = pen.color();
			int width = pen.width();
			stream << clr;
			stream << width;

			continue;
		}
		QGraphicsTextItem *p6 = dynamic_cast<QGraphicsTextItem*>(*it);
		if (p6)
		{
			QPointF pt = p6->pos();
			stream << SERIALIZE_TYPE_COMMON_TEXT;
			stream << pt;
			QString sName = p6->toPlainText();
			//sName = p6->toHtml();
			stream << sName;
			int fontSize = p6->font().pointSize();
			stream << fontSize;
			QColor clr = p6->defaultTextColor();
			stream << clr;
			continue;
		}
		QGraphicsPolygonItem *p7 = dynamic_cast<QGraphicsPolygonItem*>(*it);
		if (p7)
		{
			stream << SERIALIZE_TYPE_COMMON_PLINE;
			QPolygonF pV = p7->polygon();
			stream << pV.size();
			for (QPolygonF::Iterator it = pV.begin();it != pV.end(); ++it)
			{
				stream << *it;
			}
			continue;
		}
		CSubnetItem *p8 = dynamic_cast<CSubnetItem *>(*it);
		if (p8)
		{
			stream << SERIALIZE_TYPE_SUBNET_ITEM;
			p8->save(stream);
			QPointF pt = p8->pos();
			stream << pt;
			continue;
		}
		CIedItem *p9 = dynamic_cast<CIedItem *>(*it);
		if (p9)
		{
			stream << SERIALIZE_TYPE_IED_ITEM;
			p9->save(stream);
			QPointF pt = p9->pos();
			stream << pt;
			continue;
		}
		CMyLinkIndex *p11 = dynamic_cast<CMyLinkIndex *>(*it);
		if (p11)
		{
			stream << SERIZLIZE_TYPE_HOTPOINT_ITEM;
			p11->save(stream);
			QPointF pt = p11->pos();
			stream << pt;
			continue;
		}
		CToolTipItem *p17 = dynamic_cast<CToolTipItem *>(*it);
		if (p17)
		{
			stream << SERIALIZE_TYPE_TOOLTIP_ITEM;
			p17->save(stream);
			QPointF pt = p17->pos();
			stream << pt;
			continue;
		}
		CIEDMasterItem *p19 = dynamic_cast<CIEDMasterItem *>(*it);
		if (p19)
		{
			stream << SERIALIZE_TYPE_IEDMASTER_ITEM;
			p19->save(stream);
			QPointF pt = p19->pos();
			stream << pt;
			continue;
		}
		CIEDSlaveItem *p20 = dynamic_cast<CIEDSlaveItem *>(*it);
		if (p20)
		{
			stream << SERIALIZE_TYPE_IEDSLAVE_ITEM;
			p20->save(stream);
			QPointF pt = p20->pos();
			stream << pt;
			continue;
		}
		CtrlBlockItem *p21 = dynamic_cast<CtrlBlockItem *>(*it);
		if (p21)
		{
			stream << SERIALIZE_TYPE_CTRL_BLOCK_ITEM;
			p21->save(stream);
			QPointF pt = p21->pos();
			stream << pt;
			continue;
		}
		CVirLinkItem *p22 = dynamic_cast<CVirLinkItem *>(*it);
		if (p22)
		{
			stream << SERIALIZE_TYPE_VIRLINK_ITEM;
			p22->save(stream);
			QPointF pt = p22->pos();
			stream << pt;
			continue;
		}
		CVirSignalItem *p23 = dynamic_cast<CVirSignalItem *>(*it);
		if (p23)
		{
			stream << SERIALIZE_TYPE_VIRSIGNAL_ITEM;
			p23->save(stream);
			QPointF pt = p23->pos();
			stream << pt;
			continue;
		}
		CPhyLinkVirLinkItem *p24 = dynamic_cast<CPhyLinkVirLinkItem *>(*it);
		if (p24)
		{
			stream << SERIALIZE_TYPE_PHYVIRLINK_ITEM;
			p24->save(stream);
			QPointF pt = p24->pos();
			stream << pt;
			continue;
		}
		CPhyPortItem *p25 = dynamic_cast<CPhyPortItem *>(*it);
		if (p25)
		{
			stream << SERIALIZE_TYPE_PHYPORT_ITEM;
			p25->save(stream);
			QPointF pt = p25->pos();
			stream << pt;
			continue;
		}
		CIEDSubnetItem *p26 = dynamic_cast<CIEDSubnetItem *>(*it);
		if (p26)
		{
			stream << SERIALIZE_TYPE_IEDSUBNET_ITEM;
			p26->save(stream);
			QPointF pt = p26->pos();
			stream << pt;
			continue;
		}
		CVoltageLevelRectItem *p27 = dynamic_cast<CVoltageLevelRectItem *>(*it);
		if (p27)
		{
			stream << SERIALIZE_TYPE_VOLTAGELEVELRECT_ITEM;
			p27->save(stream);
			QPointF pt = p27->pos();
			stream << pt;
			continue;
		}
		CMyGraphicTextItem *p28 = dynamic_cast<CMyGraphicTextItem *>(*it);
		if (p28)
		{
			stream << SERIALIZE_TYPE_MYTEXT_ITEM;
			p28->save(stream);
			QPointF pt = p28->pos();
			stream << pt;
			continue;
		}
		CMyGraphicRectItem *p29 = dynamic_cast<CMyGraphicRectItem *>(*it);
		if (p29)
		{
			stream << SERIALIZE_TYPE_MYRECT_ITEM;
			p29->save(stream);
			QPointF pt = p29->pos();
			stream << pt;
			continue;
		}

		stream << 0;
	}
	QRectF rect = sceneRect();
	stream << rect;
	return true;
}

QRectF HmiGraphicsScene::load(QDataStream &stream)
{
	int itemCount;
	stream >> itemCount;
	for (int i = 0;i < itemCount;i ++)
	{
		int type;
		stream >> type;
		switch (type)
		{
		case SERIALIZE_TYPE_SoftTrip:
			{
				CSoftTripItem *p = new CSoftTripItem(myItemMenu,0,this);
				p->load(stream);
				p->setZValue(1);
				addItem(p);
			}
			break;
		case SERIALIZE_TYPE_COMMON_RECT:
			{
				float l,t,w,h;
				stream >> l;
				stream >> t;
				stream >> w;
				stream >> h;
				QRectF rct(l,t,w,h);
				QGraphicsRectItem *frameItem = new QGraphicsRectItem(rct);
				addItem(frameItem);
				QBrush bru;
				QColor btuColor;
				stream >> btuColor;
				frameItem->setBrush(btuColor);
				QPen pen;
				QColor clr;
				int width;
				stream >> clr;
				stream >> width;
				pen.setColor(clr);
				pen.setWidth(width);
				frameItem->setPen(pen);
			}
			break;
		case SERIALIZE_TYPE_COMMON_LINE:
			{
				QPointF p1,p2;
				stream >> p1;
				stream >> p2;
				QGraphicsLineItem *lineItem = new QGraphicsLineItem(QLine(p1.x(),p1.y(),p2.x(),p2.y()));
				addItem(lineItem);
				QPen pen;;
				QColor clr;
				int width;
				stream >> clr;
				stream >> width;
				pen.setColor(clr);
				pen.setWidth(width);
				lineItem->setPen(pen);
			}
			break;
		case SERIALIZE_TYPE_COMMON_TEXT:
			{
				QPointF pt;
				stream >> pt;
				QString txt;
				stream >> txt;
				int txtSize;
				stream >> txtSize;
				QFont *textFont = new QFont();
				textFont->setFamily(QObject::tr("宋体"));
				textFont->setPointSize(txtSize);
				textFont->setWeight(QFont::Normal);
				textFont->setItalic(false);
				textFont->setUnderline(false);
				QGraphicsTextItem *txtItem = new QGraphicsTextItem(txt);
				txtItem->setPos(pt);
				txtItem->setFont(*textFont);
				QColor clr;
				stream >> clr;
				txtItem->setDefaultTextColor(clr);
				addItem(txtItem);
				//txtItem->setDefaultTextColor(Qt::black);
				txtItem->setZValue(100);
			}
			break;
		case SERIALIZE_TYPE_COMMON_PLINE:
			{
				int nPointCount;
				stream >> nPointCount;
				QVector<QPointF> pts;
				for (int i = 0;i < nPointCount;i ++)
				{
					QPointF pt;
					stream >> pt;
					pts.push_back(pt);
				}
				QGraphicsPolygonItem *polyline = new QGraphicsPolygonItem(QPolygonF(pts));
				polyline->setPen(QColor(255,0,0));
				polyline->setFlag(QGraphicsItem::ItemIsSelectable);
				addItem(polyline);
			}
			break;
		case SERIALIZE_TYPE_SUBNET_ITEM:
			{
				CSubnetItem *p = new CSubnetItem(0,this,0);
				p->load(stream);
				addItem(p);
				QPointF ptLt;
				stream >> ptLt;
				p->setPos(ptLt);
				p->setZValue(1);
			}
			break;
		case SERIALIZE_TYPE_IED_ITEM:
			{
				CIedItem *p = new CIedItem(0,this,0);
				p->load(stream);
				addItem(p);
				QPointF pt;
				stream >> pt;
				p->setPos(pt);
			}
			break;
		case SERIZLIZE_TYPE_HOTPOINT_ITEM:
			{
				float l,t,w,h;
				stream >> l;
				stream >> t;
				stream >> w;
				stream >> h;
				QRectF rect(l,t,w,h);
				CMyLinkIndex *p =new CMyLinkIndex(rect);
				p->load(stream);
				addItem(p);
				QPointF pt;
				stream >> pt;
				p->setPos(pt);
				p->setZValue(101);
			}
			break;
		case SERIALIZE_TYPE_TOOLTIP_ITEM:
			{
				CToolTipItem *p = new CToolTipItem();
				p->load(stream);
				addItem(p);
				QPointF pt;
				stream >> pt;
				p->setPos(pt);
			}
			break;
		case SERIALIZE_TYPE_IEDMASTER_ITEM:
			{
				CIEDMasterItem *p = new CIEDMasterItem(this);
				p->load(stream);
				addItem(p);
				QPointF pt;
				stream >> pt;
				p->setPos(pt);
			}
			break;
		case SERIALIZE_TYPE_IEDSLAVE_ITEM:
			{
				CIEDSlaveItem *p = new CIEDSlaveItem(this);
				p->load(stream);
				addItem(p);
				QPointF pt;
				stream >> pt;
				p->setPos(pt);
			}
			break;
		case SERIALIZE_TYPE_CTRL_BLOCK_ITEM:
			{
				CtrlBlockItem *p = new CtrlBlockItem(this);
				p->load(stream);
				addItem(p);
				QPointF pt;
				stream >> pt;
				p->setPos(pt);
			}
			break;
		case SERIALIZE_TYPE_VIRLINK_ITEM:
			{
				CVirLinkItem *p = new CVirLinkItem(this);
				p->load(stream);
				addItem(p);
				QPointF pt;
				stream >> pt;
				p->setPos(pt);
				p->setRoateangle();
			}
			break;
		case SERIALIZE_TYPE_VIRSIGNAL_ITEM:
			{
				CVirSignalItem *p = new CVirSignalItem(this);
				p->load(stream);
				addItem(p);
				QPointF pt;
				stream >> pt;
				p->setPos(pt);
			}
			break;
		case SERIALIZE_TYPE_PHYVIRLINK_ITEM:
			{
				CPhyLinkVirLinkItem *p = new CPhyLinkVirLinkItem(this);
				p->load(stream);
				addItem(p);
				QPointF pt;
				stream >> pt;
				p->setPos(pt);
				p->setZValue(1);
			}
			break;
		case SERIALIZE_TYPE_PHYPORT_ITEM:
			{
				CPhyPortItem *p = new CPhyPortItem(this);
				p->load(stream);
				addItem(p);
				QPointF pt;
				stream >> pt;
				p->setPos(pt);
			}
			break;
		case SERIALIZE_TYPE_IEDSUBNET_ITEM:
			{
				CIEDSubnetItem *p = new CIEDSubnetItem(this);
				p->load(stream);
				addItem(p);
				QPointF pt;
				stream >> pt;
				p->setPos(pt);
				p->setZValue(1);
			}
			break;
		case SERIALIZE_TYPE_VOLTAGELEVELRECT_ITEM:
			{
				CVoltageLevelRectItem *p = new CVoltageLevelRectItem(this);
				p->load(stream);
				addItem(p);
				QPointF pt;
				stream >> pt;
				p->setPos(pt);
				p->setZValue(1);
			}
			break;
		case SERIALIZE_TYPE_MYTEXT_ITEM:
			{
				CMyGraphicTextItem *p = new CMyGraphicTextItem(this);
				p->load(stream);
				addItem(p);
				QPointF pt;
				stream >> pt;
				p->setPos(pt);
			}
			break;
		case SERIALIZE_TYPE_MYRECT_ITEM:
			{
				CMyGraphicRectItem *p = new CMyGraphicRectItem(this);
				p->load(stream);
				addItem(p);
				QPointF pt;
				stream >> pt;
				p->setPos(pt);
				p->setZValue(0.5);
			}
			break;
		}
	}
	QRectF sRect;
	stream >> sRect;
	return sRect;
}

void HmiGraphicsScene::DrawMsgFlowProc(QString strIedKey)
{
	DrawInfoFlowGraphicWithDiff *difgd = new DrawInfoFlowGraphicWithDiff(this, strIedKey);
	difgd->DrawGraph();
	delete difgd;
}

void HmiGraphicsScene::DrawSubnetProc(QString subnetKey)
{
	CDrawSubnetGraphicWithDiff *dsgwd = new CDrawSubnetGraphicWithDiff(this, subnetKey);
	dsgwd->DrawSubnetGraphic();
	delete dsgwd;
}

void HmiGraphicsScene::DrawTerminalProc(QString strIedKey)
{
	DrawTerminalGraphicWithDiff *dtgwd = new DrawTerminalGraphicWithDiff(this, strIedKey);
	dtgwd->Draw();
	delete dtgwd;
}

void HmiGraphicsScene::DrawVirtualLoopProc(QString strIedKey1,QString strIedKey2,QSignalTypeEnum sigType)
{
	DrawLoopGraphicWithDiff *dlgwd = new DrawLoopGraphicWithDiff(this, strIedKey1, strIedKey2,"", sigType,0);
	dlgwd->DrawIedHeaders();
	dlgwd->DrawAllVloops();
	delete dlgwd;
}

void HmiGraphicsScene::DrawMsgFlowDiffProc(QString IEDKey, QString otherIEDKey)
{
	DrawInfoFlowGraphicWithDiff *difgwd = new DrawInfoFlowGraphicWithDiff(this, IEDKey);
	difgwd->DrawGraph();
	delete difgwd;
}

void HmiGraphicsScene::DrawVirtualLoopDiffProc(QString strIedKey1,QString strIedKey2,QSignalTypeEnum sigType)
{
	DrawLoopGraphicWithDiff *dlgwd = new DrawLoopGraphicWithDiff(this, strIedKey1, strIedKey2,"", typeAll_,0);
	dlgwd->DrawIedHeaders();
	dlgwd->DrawAllVloops();
	delete dlgwd;
}

void HmiGraphicsScene::DrawTerminalDiffProc(QString IEDKey, QString otherIEDKey)
{
	OutputDebugStringA(IEDKey.toStdString().c_str());
    OutputDebugStringA("\n---------HmiGraphicsScene::DrawTerminalDiffProc------------\n");
	DrawTerminalGraphicWithDiff *dtgwd = new DrawTerminalGraphicWithDiff(this, IEDKey);
	dtgwd->Draw();
	delete dtgwd;
}
