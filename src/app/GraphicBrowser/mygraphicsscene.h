#ifndef MYGRAPHICSSCENE_H_1
#define MYGRAPHICSSCENE_H_1

#include "ReportGraphicsScene.h"

class QGraphicsLineItem;

class MyGraphicsScene : public ReportGraphicsScene
{
	Q_OBJECT

public:
	MyGraphicsScene(const QRectF &Rect,QObject *parent=0);
	~MyGraphicsScene();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
};

#endif // MYGRAPHICSSCENE_H_1
