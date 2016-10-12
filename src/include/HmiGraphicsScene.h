#ifndef CMYGRAPHICSSCENE_H
#define CMYGRAPHICSSCENE_H

#include <QGraphicsScene>
#include "GlobalDef.h"
#include <QtGui>
#include "hmidll_global.h"
#include "SCLModelSvrAdapterInterface.h"
#include "SCDDifferCompare.h"
#include "GraphicModelInterface.h"
#include "GraphicSCLModelImpl.h"
#include "GraphicModelDiffImpl.h"
#include "MyGraphItem.h"

class HMIDLL_EXPORT HmiGraphicsScene : public QGraphicsScene
{
	Q_OBJECT

public:
	HmiGraphicsScene();
	HmiGraphicsScene(QMenu *itemMenu,const QRectF &Rect,QObject *parent, bool isDiff = false);
	HmiGraphicsScene(const QRectF &Rect,QObject *parent, SCDDiffCompare *scdDiffer, bool isDiff = false);
	~HmiGraphicsScene();

	void	SwitchOpen();
	void	SwitchClose();
	SCLModelSvrAdapterInterface *	GetSclModel() {return SCLModel;}
	GraphicModelInterface* GetGraphicModel()  {return GraphicModel;}

	bool	ExportSvgProc();
	QRectF	load(QDataStream &stream);
	bool	save(QDataStream &stream);
	void	DrawSelfProc(QString strIedKey);
	void	DrawTwoProc(QString strIedKey1,QString strIedKey2);
	void	DrawAllProc(QString strIedKey);
	
	//niuziming add;
	void	DrawMsgFlowProc(QString strIedKey);
	void	DrawVirtualLoopProc(QString strIedKey1,QString strIedKey2,QSignalTypeEnum sigType);
	void	DrawTerminalProc(QString strIedKey);
	void	DrawSubnetProc(QString subnetKey);

	void	DrawMsgFlowDiffProc(QString IEDKey, QString otherIEDKey);
	void	DrawVirtualLoopDiffProc(QString strIedKey1,QString strIedKey2,QSignalTypeEnum sigType);
	void	DrawTerminalDiffProc(QString IEDKey, QString otherIEDKey);
	//niuziming add;
	
	void	assoHighLight(const QString srcItemID, const QVector<QString> assoIDs);
	virtual void procInfos(int msgType,QString masterKey, QString slaveKey, int type);

	void	setDrag(bool isD);
	bool	getDrag();
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent  *event);

	SCLModelSvrAdapterInterface   *SCLModel;
	GraphicModelInterface *GraphicModel;
	CGraphicSCLModelImpl *GraphicSCLModel;
	GraphicModelDiffImpl *GraphicDiffModel;

private:
	QMenu *myItemMenu;
	QGraphicsItem *currentItem;
	bool isDrag;
};

#endif // CMYGRAPHICSSCENE_H
