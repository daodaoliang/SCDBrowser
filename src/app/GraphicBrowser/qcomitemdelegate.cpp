#include "qcomitemdelegate.h"
#include <QComboBox>
#include <QDebug>
#include <QLineEdit>

bool BasicFlag;
bool OtherFlag;
QString current;

qComItemDelegate::qComItemDelegate(QObject *parent)
	: QItemDelegate(parent)
{
	 BasicFlag=false;
	 OtherFlag=false;
}
void qComItemDelegate::GetStrCount(bool mBasicflag,bool mOtherFlag)
{
	 BasicFlag=mBasicflag;
	 OtherFlag=mOtherFlag;
}
void qComItemDelegate::initFlag()
{
	 BasicFlag=false;
	 OtherFlag=false;
}
QWidget *qComItemDelegate::createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	if(index.column()==0)
	{
		QComboBox *box=new QComboBox(parent);
		QStringList str;
		if(BasicFlag&&OtherFlag)
			str<<"��ѡ��";
		else if(!BasicFlag&&OtherFlag)
			str<<"��׼�ļ�"<<"��ѡ��";
		else if(BasicFlag&&!OtherFlag)
			str<<"�Ƚ��ļ�"<<"��ѡ��";
		else if(!BasicFlag&&!OtherFlag)
			str<<"��׼�ļ�"<<"�Ƚ��ļ�"<<"��ѡ��";
		box->addItems(str);
		return box;
	}else
	{
		current= index.data().toString();
		QItemDelegate::createEditor(parent,option,index);
		return NULL;
	}

}

void	qComItemDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
{
	if(index.column()==0)
	{
		QComboBox *combox=(QComboBox*)editor;
		QString text=combox->currentText();
		model->setData(index,text,0);
	}else 
		model->setData(index,current,0);

}
qComItemDelegate::~qComItemDelegate()
{

}
