#ifndef MACWIDGET_H
#define MACWIDGET_H

//#include <QTableWidget>
#include "MyTableWidget.h"

//×ÓÍøÍ¼MAC±êÇ©Ò³¿Ø¼ş;
class CMacWidget : public CMyTableWidget
{
public:
	CMacWidget(QWidget *parent = 0);
	~CMacWidget(void);

private:
	void FillTable();
};

#endif