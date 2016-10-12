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
			str<<"不选择";
		else if(!BasicFlag&&OtherFlag)
			str<<"基准文件"<<"不选择";
		else if(BasicFlag&&!OtherFlag)
			str<<"比较文件"<<"不选择";
		else if(!BasicFlag&&!OtherFlag)
			str<<"基准文件"<<"比较文件"<<"不选择";
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
