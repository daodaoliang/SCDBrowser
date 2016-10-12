#ifndef MYTABWIDGET_H
#define MYTABWIDGET_H

#include <QTabWidget>
#include <QMainWindow>
#include "mytabbar.h"
class MyTabWidget : public QTabWidget
{
	Q_OBJECT

public:
	MyTabWidget(int i,QWidget *parent=0,QMainWindow *a=0);
	~MyTabWidget();
	void tabbarhide();
	void tabBarShow();

	void setIEDKey(const QString& IEDKey);
	QString getIEDKey();
private:
	MyTabBar *m_tabbar;
	QString m_IEDKey;
};

#endif // MYTABWIDGET_H
