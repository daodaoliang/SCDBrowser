#ifndef MYTABLEWIDGET_H
#define MYTABLEWIDGET_H

#include <QTableWidget>

class CMyTableWidget : public QTableWidget
{
	Q_OBJECT

public:
	CMyTableWidget(QWidget *parent = 0);
	~CMyTableWidget(void);

	virtual void setSubnetKey(const QString& subnetKey);

protected:
	virtual void GetIEDList();
	virtual void FillTable() = 0;

private:
	void sortIEDList(const QVector<QString>& iedKeyVec);

protected:
	QString m_subnetKey;
	QVector<QString> m_subIEDs;
	QStringList m_tableTitle;
};

#endif
