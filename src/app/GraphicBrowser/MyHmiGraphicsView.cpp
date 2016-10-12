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
	if (QApplication::keyboardModifiers() == Qt::ControlModifier)
	{
		double numDegrees = -event->delta() / 8.0;
		double numSteps = numDegrees / 15.0;
		double factor = std::pow(1.125, numSteps);
		if (factor < 0.25)
			factor = 0.25;
		if (factor > 20)
			factor = 20;
		scale(factor, factor);

	}
	else
		QGraphicsView::wheelEvent(event);
}

void HmiGraphicsView::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	mScene->clearSelection();
	//scene()->setSelected(true);
	//myContextMenu->exec(event->screenPos());
}