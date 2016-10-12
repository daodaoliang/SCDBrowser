#ifndef FIXEDPROCESSBAR_H
#define FIXEDPROCESSBAR_H

#include <QWidget>
#include "ui_ExportProcessBar.h"

class FixedProcessBar : public QWidget
{
	Q_OBJECT

public:
	FixedProcessBar(QWidget *parent = 0);
	~FixedProcessBar();

private:
	Ui::ExportProcessBar bar;
};

#endif // FIXEDPROCESSBAR_H
