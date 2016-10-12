#include "mytabbar.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QDebug>


MyTabBar::MyTabBar(int i,QWidget *parent,QMainWindow *a)
{
	//senterWidget=parent;
	child_zong=i;
	s=a;
	connect(this,SIGNAL(sendsignal(QPoint)),a,SLOT(receiveslot(QPoint))); 

}

MyTabBar::~MyTabBar()
{

}
 void MyTabBar::slt_current(int index)
 {
	 //qDebug()<<"change"<<index;
 }
void MyTabBar::mousePressEvent(QMouseEvent *event)
{
	   setCursor(Qt::OpenHandCursor);
	   QTabBar::mousePressEvent(event);
}
void MyTabBar::mouseMoveEvent(QMouseEvent *event)
{
	  
           QSize size=s->centralWidget()->size();
	       QPoint abouslout=mapTo(s->centralWidget(),event->pos());
		   qDebug()<<"center size"<<size<<abouslout;

	     /* if(event->pos().x()>0&&event->pos().x()<size.width()/2&&event->pos().y()>0&&event->pos().y()<size.height()/2)
	       {
	       setCursor(Qt::ForbiddenCursor);
		   return;
	       }*/


	          if(child_zong==0)
		    {

			  if(abouslout.y()>50&&abouslout.y()<size.height()&&abouslout.x()>0&&abouslout.x()<size.width())
				  setCursor(Qt::SizeAllCursor);
			  else
				  setCursor(Qt::ForbiddenCursor);
			
		    }
		  
		   /*  else if (child_zong==1)
		    {

			  if(abouslout.y()<50&abouslout.y()>0&&abouslout.x()>0&&abouslout.x()<size.width())
				  setCursor(Qt::SizeAllCursor);
			  else
				  setCursor(Qt::ForbiddenCursor);
		    }*/
			  else if (child_zong==1)
			  {
				   if(abouslout.y()>0&&abouslout.y()<size.height()&&abouslout.x()>0&&abouslout.x()<size.width())
				   {
					   setCursor(Qt::SizeAllCursor);
				   }
				   else 
				   {

					   setCursor(Qt::ForbiddenCursor);
				   }
			  }	  
			 QTabBar::mouseMoveEvent(event);
}
void MyTabBar::mouseReleaseEvent(QMouseEvent *event)
{

	
	//QSize size=senterWidget->size();
	//QPoint abouslout=mapTo(senterWidget,event->pos());
	QSize size=s->centralWidget()->size();
	//QSize size=senterWidget->size();
	QPoint abouslout=mapTo(s->centralWidget(),event->pos());
	int index=currentIndex();
	if(event->pos().x()>0&&event->pos().x()<size.width()/2&&event->pos().y()>0&&event->pos().y()<size.height()/2)
	{
		setCursor(Qt::ArrowCursor);
		return;
	}
	if(child_zong==0)  // 合并或者分立标志位  0 fl
	{
		if(abouslout.y()>50&&abouslout.y()<size.height()&&abouslout.x()>0&&abouslout.x()<size.width())
		{
		    emit sendsignal(abouslout);
			QString drag_drog=QString::number(child_zong);
			QByteArray a=drag_drog.toAscii();
			QDrag *drag = new QDrag(this);
			QMimeData *mime = new QMimeData;
			drag->setMimeData(mime);
			mime->setData(drag_drog,a);
			mime->setText(QString::number(index));
			//	mime->setData()
			drag->exec();
		}
   }
	else if(child_zong==1) // 1  hebing 
	{
		
		 if(abouslout.y()>0&&abouslout.y()<size.height()&&abouslout.x()>0&&abouslout.x()<size.width())
		{
			emit sendsignal(abouslout); //发送当前鼠标映射cennterwidget 位置 到mani主窗口;
			QString drag_drog=QString::number(child_zong);
			QByteArray a=drag_drog.toAscii();
			QDrag *drag = new QDrag(this);
			QMimeData *mime = new QMimeData;
			drag->setMimeData(mime);
			mime->setText(tabText(index));
			mime->setData(drag_drog,a);
			drag->exec();
		}
	}
	setCursor(Qt::ArrowCursor);
	QTabBar::mouseReleaseEvent(event);
}

