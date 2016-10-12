#ifndef CMYGRAPHICSVIEW_H
#define CMYGRAPHICSVIEW_H

#include <QGraphicsView>
#include "HmiGraphicsScene.h"
#include "hmidll_global.h"

class HMIDLL_EXPORT HmiGraphicsView : public QGraphicsView
{
	Q_OBJECT

public:
	HmiGraphicsView(QWidget *parent = 0);
	~HmiGraphicsView();

protected:
	void wheelEvent(QWheelEvent *event);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	QPoint lastMousePos;
};

#endif // CMYGRAPHICSVIEW_H
