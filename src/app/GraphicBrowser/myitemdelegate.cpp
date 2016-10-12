#include "myitemdelegate.h"

#define  StateHasFocus 0x00000100
#define  StateSelect 0x00008000
#include <QPainter>
#include <QDebug>
#include <QStyledItemDelegate>
#include <QComboBox>

MyitemDelegate::MyitemDelegate(QObject *parent)
	: QItemDelegate(parent)
{

}
void MyitemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index)const
{
	
	QStyleOptionViewItem  viewOption(option);  
	//高亮显示与普通显示时的前景色一致（即选中行和为选中时候的文字颜色一样）;
	viewOption.palette.setColor(QPalette::HighlightedText, index.data(Qt::ForegroundRole).value<QColor>());  
	QItemDelegate::paint(painter, viewOption, index);
}
	
MyitemDelegate::~MyitemDelegate()
{

}
