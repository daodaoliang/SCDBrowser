#ifndef MYTABBAR_H
#define MYTABBAR_H

#include <QTabBar>
#include <QMouseEvent>
#include <QPoint>
#include <QMainWindow>
#include <QWidget>

class MyTabBar : public QTabBar
{
	Q_OBJECT

public:
	MyTabBar(int i,QWidget *parent=0,QMainWindow *a=0);
	~MyTabBar();
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

private slots:
	 void slt_current(int);
signals:
	 void sendsignal(QPoint F);
private:
	QMainWindow *s;
	//QWidget *senterWidget;;
	int child_zong;
};

#endif // MYTABBAR_H
