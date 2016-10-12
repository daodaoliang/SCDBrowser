#include "fixedprocessbar.h"


FixedProcessBar::FixedProcessBar(QWidget *parent)
{
	bar.setupUi(this);
	setWindowFlags(Qt::CustomizeWindowHint);
}

FixedProcessBar::~FixedProcessBar()
{

}
