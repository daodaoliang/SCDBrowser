#ifndef QCOMITEMDELEGATE_H
#define QCOMITEMDELEGATE_H

#include <QItemDelegate>

class qComItemDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	qComItemDelegate(QObject *parent);
	~qComItemDelegate();
	void initFlag();
	QWidget *createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
	void	setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const;

private:
private slots:
	void GetStrCount(bool,bool);
	
};

#endif // QCOMITEMDELEGATE_H
