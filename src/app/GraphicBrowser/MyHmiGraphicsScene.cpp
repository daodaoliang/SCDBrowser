#include "MyHmiGraphicsScene.h"
//#include "SecurityMeasuresEditorFrm.h"
#include "SCLModelSvrAdapter.h"
#include "SCDDifferCompare.h"
//#include "..\IEDGraphics\csofttripitem.h"
//#include "..\IEDGraphics\cvirtualterminallineitem.h"
//#include "..\IEDGraphics\copticalfilberitem.h"

#define		GRAPHIC_ITEM_CLICK_MSG_INDEX		0
#define		GRAPHIC_ITEM_CLICK_MSG_TRIP         1

MyHmiGraphicsScene::MyHmiGraphicsScene()
	//: HmiGraphicsView()
{
	SCDDiffCompare *scdDifCompare = SCDDiffCompare::GetInstance();
	GraphicSCLModel = new CGraphicSCLModelImpl();
	GraphicDiffModel = new GraphicModelDiffImpl(scdDifCompare);
	GraphicModel = GraphicSCLModel;
}

MyHmiGraphicsScene::MyHmiGraphicsScene(QMenu *itemMenu,const QRectF &Rect,QObject *parent)
//: HmiGraphicsView(parent)
{
	setSceneRect(Rect);
	SCLModel   = SCLModelAdapter::GetInstance();
	SCDDiffCompare *scdDifCompare = SCDDiffCompare::GetInstance();
	GraphicSCLModel = new CGraphicSCLModelImpl();
	GraphicDiffModel = new GraphicModelDiffImpl(scdDifCompare);
	GraphicModel = GraphicSCLModel;
	//currentItem = 0;
}

void MyHmiGraphicsScene::procInfos(int msgType,QString masterKey, QString slaveKey, int type)
{
	if(msgType == GRAPHIC_ITEM_CLICK_MSG_TRIP)
	{
		emit tripClicked(masterKey,slaveKey,type);
	}
	else if (msgType == GRAPHIC_ITEM_CLICK_MSG_INDEX)
	{
		//switch (type)
		//{
		//case link_type_msgflow:
		//	DrawMsgFlowProc(masterKey);
		//	break;
		//case link_type_vloop_goose:
		//	DrawVirtualLoopProc(masterKey,slaveKey,typeGoose_);
		//	break;
		//case link_type_vloop_sv:
		//	DrawVirtualLoopProc(masterKey,slaveKey,typeSV_);
		//	break;
		//case link_type_vterm:
		//	DrawTerminalProc(masterKey);
		//	break;
		//default:
		//	break;
		//}
		//emit indexClicked();
		emit tripClicked(masterKey,slaveKey,type);
	}
}

void MyHmiGraphicsScene::changeModel(bool isDiff)
{
	if (isDiff)
	{
		GraphicModel = GraphicDiffModel;
	} 
	else
	{
		GraphicModel = GraphicSCLModel;
	}
}

//void MyHmiGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
//{
//	QGraphicsItem *item = itemAt(mouseEvent->scenePos());
//	if (item)
//	{
//		if (mouseEvent->button() != Qt::LeftButton)
//			return;
///*		CVirtualTerminalLineItem *item1 = 0;
//		item1 = dynamic_cast<CVirtualTerminalLineItem *>(item);
//		CSoftTripItem *item2 = 0;
//		item2 = dynamic_cast<CSoftTripItem *>(item);
//		COpticalFilberItem *item3 = 0;
//		item3 = dynamic_cast<COpticalFilberItem *>(item);
//		QString strInfo;
//		if (item1)
//			strInfo = "虚端子对象";
//		else if (item2)
//			strInfo = "软压板/光纤插头对象";
//		else if (item3)
//			strInfo = "物理光纤对象";
//		else
//			strInfo = "";
//		if (strInfo != "")
//			int r = QMessageBox::warning(NULL, tr("警告"),
//			strInfo,
//			QMessageBox::Ok);*/
//	}
//	QGraphicsScene::mousePressEvent(mouseEvent);
//}

MyHmiGraphicsScene::~MyHmiGraphicsScene()
{

}