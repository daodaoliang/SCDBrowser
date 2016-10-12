#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include "ReportGraphicsView.h"

class MyGraphicsView : public ReportGraphicsView
{
private:

protected:
	//void wheelEvent(QWheelEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // MYGRAPHICSVIEW_H
