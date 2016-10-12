/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      myitemdelegate.h
  * Author:         sl
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   用于表格
  */ 
#ifndef MYITEMDELEGATE_H
#define MYITEMDELEGATE_H

#include <QItemDelegate>

class MyitemDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	MyitemDelegate(QObject *parent);
	~MyitemDelegate();
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index)const;
private:
	
};

#endif // MYITEMDELEGATE_H
