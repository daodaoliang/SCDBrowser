#include "mytabwidget.h"

#include <QDebug>
#include <QWidget>
MyTabWidget::MyTabWidget(int i,QWidget *parent,QMainWindow *a)
	: QTabWidget(parent)
{
     qDebug()<<parent;
	 m_tabbar=new MyTabBar(i,parent,a);
	 setTabBar(m_tabbar);
	 m_IEDKey == "";
}

MyTabWidget::~MyTabWidget()
{

}

void MyTabWidget::setIEDKey(const QString& IEDKey)
{
	m_IEDKey = IEDKey;
}

QString MyTabWidget::getIEDKey()
{
	return m_IEDKey;
}

void MyTabWidget::tabbarhide()
{
	m_tabbar->hide();
}

void MyTabWidget::tabBarShow()
{
	m_tabbar->show();
}