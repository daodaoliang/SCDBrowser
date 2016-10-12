#ifndef MYCMYGRAPHICSSCENE_H
#define MYCMYGRAPHICSSCENE_H

#include "HmiGraphicsScene.h"

class MyHmiGraphicsScene : public HmiGraphicsScene
{
	Q_OBJECT
public:
	MyHmiGraphicsScene();
	MyHmiGraphicsScene(QMenu *itemMenu,const QRectF &Rect,QObject *parent);
	~MyHmiGraphicsScene();
	virtual void procInfos(int msgType,QString masterKey, QString slaveKey, int type);

	void changeModel(bool isDiff);

signals:
	void tripClicked(QString masterKey, QString slaveKey, int type);
	//void indexClicked();

protected:
	//void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
};

#endif // CMYGRAPHICSSCENE_H
