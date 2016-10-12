#include "MyTableWidget.h"
#include "TableSCLModelImpl.h"

CMyTableWidget::CMyTableWidget(QWidget *parent)
	: QTableWidget(parent)
{
	m_subnetKey = "";
	m_subIEDs.clear();
	m_tableTitle.clear();
}

CMyTableWidget::~CMyTableWidget(void)
{
}

void CMyTableWidget::setSubnetKey(const QString& subnetKey)
{
	m_subnetKey = subnetKey;
	GetIEDList();
	FillTable();
}

void CMyTableWidget::GetIEDList()
{
	m_subIEDs.clear();
	QVector<QString> tempVec;
	CTableSCLModelImpl tableImpl;
	tableImpl.GetIEDListBySubnetKey(m_subnetKey, tempVec);
	sortIEDList(tempVec);
}

void CMyTableWidget::sortIEDList(const QVector<QString>& iedKeyVec)
{
	int iedCount = iedKeyVec.size();
	QList<QString> tempList = iedKeyVec.toList();

	int index = 0;
	for (int i = 0; i < iedCount - 1; i++)
	{
		index = i;
		QString sName = tempList.at(i);
		for (int j = i + 1; j < iedCount; j++)
		{
			QString dName = tempList.at(j);
			//if (strncmp(sName.toLocal8Bit().constData(),dName.toLocal8Bit().constData(),256)>0)
			if (sName.compare(dName) > 0)
			{
				index = j;
				sName = dName;
			}
		}
		if (index != i)
		{
			tempList.swap(i,index);
		}
	}
	m_subIEDs = tempList.toVector();
}
