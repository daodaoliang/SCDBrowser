#ifndef IPWIDGET_H
#define IPWIDGET_H

//#include <QTableWidget>
#include "MyTableWidget.h"

//×ÓÍøÍ¼IP±êÇ©Ò³¿Ø¼ş;
class CIPWidget : public CMyTableWidget
{
public:
	CIPWidget(QWidget *parent = 0);
	~CIPWidget(void);

private:
	void FillTable();
};

#endif