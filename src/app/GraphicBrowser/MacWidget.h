#ifndef MACWIDGET_H
#define MACWIDGET_H

//#include <QTableWidget>
#include "MyTableWidget.h"

//����ͼMAC��ǩҳ�ؼ�;
class CMacWidget : public CMyTableWidget
{
public:
	CMacWidget(QWidget *parent = 0);
	~CMacWidget(void);

private:
	void FillTable();
};

#endif