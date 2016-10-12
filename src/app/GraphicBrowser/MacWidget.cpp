#include "MacWidget.h"
#include "TableSCLModelImpl.h"

CMacWidget::CMacWidget(QWidget *parent)
	: CMyTableWidget(parent)
{
	m_tableTitle<<"IED名称"<<"控制块"<<"MAC地址";
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSelectionMode(QAbstractItemView::SingleSelection);
}

CMacWidget::~CMacWidget(void)
{

}

void CMacWidget::FillTable()
{
	QStringList tempList;
	this->clear();
	setColumnCount(3);
	CTableSCLModelImpl tableImpl;
	int tableRow = 0;
	QString tempKey = "";
	for (uint i = 0; i != m_subIEDs.size(); i++)
	{
		if (tempKey.compare(m_subIEDs.at(i)) == 0)
		{
			continue;
		}
		tempKey = m_subIEDs.at(i);
		//this->setItem(tableRow, 0, new QTableWidgetItem(m_subIEDs.at(i)));
		int tempRow = 0;

		QVector<QString> gooseCBVec, svCBVec;
		tableImpl.GetGooseCtrlBlockListByIED(m_subIEDs.at(i), &gooseCBVec);
		tableImpl.GetSVCtrlBlockListByIED(m_subIEDs.at(i), &svCBVec);
		int gsSize = gooseCBVec.size();
		int svSize = svCBVec.size();

		QString iedName = tableImpl.GetNameFromKey(m_subIEDs.at(i));
		IEDStru iedInfo;
		tableImpl.GetIEDInfo(m_subIEDs.at(i), iedInfo);
		QString nameDesc = QString("[") + iedName + QString("]") + iedInfo.IEDDesc.baseAttributeName;

		for (uint gIndex = 0; gIndex != gsSize; gIndex++)
		{
			GOOSECBStru gsCBInfo;
			tableImpl.GetCtrlBlockInfo(gooseCBVec.at(gIndex), gsCBInfo);
			tempList<<nameDesc<<gsCBInfo.AppID.baseAttributeName<<gsCBInfo.MACAddr.baseAttributeName;
			//this->setItem(tableRow + tempRow, 0, new QTableWidgetItem(nameDesc));
			//this->setItem(tableRow + tempRow, 1, new QTableWidgetItem(gsCBInfo.AppID.baseAttributeName));
			//this->setItem(tableRow + tempRow, 2, new QTableWidgetItem(gsCBInfo.MACAddr.baseAttributeName));
			tempRow++;
		}

		for (uint sIndex = 0; sIndex != svSize; sIndex++)
		{
			SVCBStru svCBInfo;
			tableImpl.GetCtrlBlockInfo(svCBVec.at(sIndex), svCBInfo);
			tempList<<nameDesc<<svCBInfo.AppID.baseAttributeName<<svCBInfo.MACAddr.baseAttributeName;
			//this->setItem(tableRow + tempRow, 0, new QTableWidgetItem(nameDesc));
			//this->setItem(tableRow + tempRow, 1, new QTableWidgetItem(svCBInfo.AppID.baseAttributeName));
			//this->setItem(tableRow + tempRow, 2, new QTableWidgetItem(svCBInfo.MACAddr.baseAttributeName));
			tempRow++;
		}

		//合并单元格;
		if (gsSize + svSize > 1)
		{
			//this->setSpan(tableRow, 0, tempRow, 1);
		}
		tableRow += tempRow;
	}
	setRowCount(tableRow);
	for (uint i = 0; i < tableRow; i++)
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
