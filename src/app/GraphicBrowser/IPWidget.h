#ifndef IPWIDGET_H
#define IPWIDGET_H

//#include <QTableWidget>
#include "MyTableWidget.h"

//����ͼIP��ǩҳ�ؼ�;
class CIPWidget : public CMyTableWidget
{
public:
	CIPWidget(QWidget *parent = 0);
	~CIPWidget(void);

private:
	void FillTable();
};

#endif