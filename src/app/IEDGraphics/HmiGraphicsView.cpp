#include "HmiGraphicsview.h"
#include <QtGui>
#include <cmath>

using namespace std;

HmiGraphicsView::HmiGraphicsView(QWidget *parent)
	: QGraphicsView(parent)
{

}

HmiGraphicsView::~HmiGraphicsView()
{

}

void HmiGraphicsView::wheelEvent(QWheelEvent *event)
{
	double numDegrees = event->delta() / 8.0;
	double numSteps = numDegrees / 15.0;
	double factor = std::pow(1.125, numSteps);
	if (factor < 0.25)
		factor = 0.25;
	if (factor > 20)
		factor = 20;
	scale(factor, factor);
}

void HmiGraphicsView::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{

}
void HmiGraphicsView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::MiddleButton)
	{
		setDragMode(ScrollHandDrag);
		lastMousePos = event->pos();
	}
	if (dragMode() == ScrollHandDrag)
	{
		lastMousePos = event->pos();
	}
	QGraphicsView::mousePressEvent(event);
}

void HmiGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::MiddleButton)
	{
		setDragMode(RubberBandDrag);
	}
	QGraphicsView::mouseReleaseEvent(event);
}

void HmiGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (items(event->pos()).size() == 0)
		{
			if (dragMode() == ScrollHandDrag)
			{
				setDragMode(RubberBandDrag);
			}
			else
			{
				setDragMode(ScrollHandDrag);
			}
		}		
	}	
	QGraphicsView::mouseDoubleClickEvent(event);
}

void HmiGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
	if (dragMode() == QGraphicsView::ScrollHandDrag)
	{
		if (event->buttons() & Qt::MiddleButton || event->buttons() & Qt::LeftButton)
		{
			QScrollBar *hBar = horizontalScrollBar();
			QScrollBar *vBar = verticalScrollBar();
			QPoint delta = event->pos() - lastMousePos;
			lastMousePos = event->pos();
			hBar->setValue(hBar->value() + (isRightToLeft() ? delta.x() : -delta.x()));
			vBar->setValue(vBar->value() - delta.y());
		}
	}
	QGraphicsView::mouseMoveEvent(event);
}
