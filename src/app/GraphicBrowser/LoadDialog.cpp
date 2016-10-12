#include "LoadDialog.h"


CLoadDialog::CLoadDialog(QWidget *parent) : QDialog(parent)
{
	strWait = "Waiting";
	m_label = new QLabel(this);
	m_timer = new QTimer();
    this->setFixedSize(68,68); 
	
    setWindowOpacity(0.5); //����͸���ã� 
    this->setWindowFlags(Qt::Dialog|Qt::CustomizeWindowHint|Qt::FramelessWindowHint); 
      
      
    //ȡ���Ի������ͱ߿� 
   this->setAutoFillBackground(true); 
   this->setContentsMargins(0,0,0,0); 
    m_label->setContentsMargins(0,0,0,0); 

   m_movie = new QMovie("images/loader.gif"); 
   m_label->setMovie(m_movie); 
    m_movie->start();
}

CLoadDialog::~CLoadDialog(void)
{
	delete m_label;
	delete m_timer;
	delete m_movie;
}
