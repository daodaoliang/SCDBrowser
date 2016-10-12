#include "myprogress.h"

Myprogress::Myprogress(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	ui.pushButton->setToolTip("ת����̨����");
	ui.pushButton->setDisabled(true);
	ui.pushButton->hide();
	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(backRunButton()));
	ui.progressBar->setRange(0,100);
	setModal(true);
	//ui.label->adjustSize();
	//ui.label->setWordWrap(true);
	//ui.label->setGeometry(QRect(40, 80, 329, 27*4));  //�ı��о�
	//ui.label->setAlignment(Qt::AlignTop);
	ui.label_4->hide();

}

void Myprogress::backRunButton()
{
	  hide();
	  emit sendHideState();
}
void Myprogress::setLabeCurrentText(QString text)
{
	ui.label_3->setText(text);

}
void Myprogress::setPorgressValue(int value)
{
	  ui.progressBar->setValue(value);
}

void Myprogress::setFileText(QString text,int current,int total)
{
	 ui.label->setText(text);
	/* ui.label->setText(text);
	 if(total!=0)
	 {
		 QString TextDesc="����%1��SCD��Ҫ���룺";
	     QString desc="%1�ļ����ڵ�����(%2/%3)";

		 QString Total=QString::number(total,10);
		 QString Current=QString::number(current+1,10);
		 QString Text="%1/%2";
		 ui.label->setText(desc.arg(text).arg(Current).arg(Total));
		 ui.label_4->setText(TextDesc.arg(Total));
	
	 }*/
}
Myprogress::~Myprogress()
{

}
