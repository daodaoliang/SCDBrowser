#include "IPWidget.h"
#include "TableSCLModelImpl.h"

CIPWidget::CIPWidget(QWidget *parent)
	: CMyTableWidget(parent)
{
	m_tableTitle<<"IED名称"<<"访问点"<<"IP地址";
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSelectionMode(QAbstractItemView::SingleSelection);
}

CIPWidget::~CIPWidget(void)
{
	
}

void CIPWidget::FillTable()
{
	this->clear();
	setColumnCount(3);
	CTableSCLModelImpl tableImpl;
	QMap<QString, QString> tempMap;
	for (uint i =0; i < m_subIEDs.size(); i++)
	{
		tempMap.insert(m_subIEDs.at(i), m_subIEDs.at(i));
	}
	QMap<QString, QString>::iterator it;
	int iRow = 0;
	QStringList tempList;
	for (it = tempMap.begin(); it != tempMap.end(); it++)
	{
		QString iedName = tableImpl.GetNameFromKey(it.value());
		IEDStru iedInfo;
		tableImpl.GetIEDInfo(it.value(), iedInfo);
		QString nameDesc = QString("[") + iedName + QString("]") + iedInfo.IEDDesc.baseAttributeName;

		QVector<QString> apList;
		tableImpl.GetAPListOfIEDFromSubnet(m_subnetKey, iedName, apList);
	
		for (uint j = 0; j != apList.size(); j++)
		{
			QAPInfo apInfo;
			tableImpl.GetApInfo(m_subnetKey, iedName, apList.at(j), apInfo);
			tempList<<nameDesc << apInfo.name<< apInfo.ipAddr;
			//this->setItem(iRow,0,new QTableWidgetItem(nameDesc));
			//this->setItem(iRow,1,new QTableWidgetItem(apInfo.name));
			//this->setItem(iRow,2,new QTableWidgetItem(apInfo.ipAddr));
			iRow++;
		}
	}

	setRowCount(iRow);
	for (uint i = 0; i < iRow; i++)
	{
		if (3*i+2 < tempList.size())
		{
			this->setItem(i, 0, new QTableWidgetItem(tempList[3*i]));
			this->setItem(i, 1, new QTableWidgetItem(tempList[3*i+1]));
			this->setItem(i, 2, new QTableWidgetItem(tempList[3*i+2]));
		}		
	}

	this->setHorizontalHeaderLabels(m_tableTitle);
	this->resizeColumnsToContents();
	this->resizeRowsToContents();
}
