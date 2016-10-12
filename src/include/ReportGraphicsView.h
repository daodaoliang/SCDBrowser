#ifndef REPORTGRAPHICSVIEW_H
#define REPORTGRAPHICSVIEW_H
#include "reportdll_global.h"

#include <QGraphicsView>
#include "ReportGraphicsScene.h"
class REPORTDLL_EXPORT ReportGraphicsView : public QGraphicsView
{
	Q_OBJECT

public:
	ReportGraphicsView(QWidget *parent = 0);
	~ReportGraphicsView();
	void mousePressEvent(QMouseEvent *event);

	ReportGraphicsScene *mScene;
private:
	void wheelEvent(QWheelEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);

	void	SetMouseMoveCursor(const QPoint &point);
	void ClipCursor(const QRectF &rect,QPointF &point);
};

#endif // REPORTGRAPHICSVIEW_H
