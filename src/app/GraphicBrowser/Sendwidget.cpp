#include "Sendwidget.h"
#include <QDebug>
#include <QSplitter>
#include <QVBoxLayout>
#include <QMessageBox>
#include "myitemdelegate.h"
#include <QHeaderView>

GooseSendWidget::GooseSendWidget(QWidget *parent)
	: QWidget(parent)
{
	rowSelect = 0;
	QSplitter *splitter = new QSplitter(Qt::Vertical,this);
	QVBoxLayout *vboxLayout = new QVBoxLayout;
	gsCrlBlockListWidget = new QTableWidget;
	MyitemDelegate *item = new MyitemDelegate(this);
	gsCrlBlockListWidget->setItemDelegate(item);
	gsCrlBlockListWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行;
	gsCrlBlockListWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑;
	gsCrlBlockListWidget->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色;

	gsTabHead<<"控制块名称"<<"数据集"<<"APPID"<<"MAC地址"<<"信息点数"<<"VLAN-ID"
		<<"VLAN优先级"<<"配置版本号";

	//gsCrlBlockListWidget->setRowCount(5);
	connect(gsCrlBlockListWidget,SIGNAL(itemSelectionChanged()),this,SLOT(fillChannelListTab()));
	QTabWidget *virtualTerminalTab = new QTabWidget;

	ChannelListTabWidget = new QTableWidget; 
	ChannelListTabWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行;
	ChannelListTabWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑;
	ChannelListTabWidget->setFrameShape(QFrame::NoFrame); 
	QStringList ChannelListHead;
	ChannelListHead<<"序号"<<"类型"<<"描述";
	ChannelListTabWidget->setColumnCount(ChannelListHead.count());
	ChannelListTabWidget->setHorizontalHeaderLabels(ChannelListHead);
	
	virtualTerminalTabWidget = new QTableWidget;
	virtualTerminalTabWidget->setItemDelegate(item);
	virtualTerminalTabWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行;
	virtualTerminalTabWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑;
	virtualTerminalTabWidget->setFrameShape(QFrame::NoFrame); 
	virtualTerminalTabWidget->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色;
	//	virtualTerminalTab->addTab(ChannelListTabWidget,tr("通道列表"));
	virtualTerminalTab->addTab(virtualTerminalTabWidget,tr("虚端子列表"));

	virtualTerminalTabWidget->verticalHeader()->setVisible(false); // 隐藏Header

	splitter->addWidget(gsCrlBlockListWidget);
	splitter->addWidget(virtualTerminalTab);

	setLayout(vboxLayout);
	vboxLayout->addWidget(splitter);
}
void GooseSendWidget::setIEDKey(QString IEDkey)
{
	IEDKEY=IEDkey;
}
void GooseSendWidget::GetGooseCtrIndex(QVector<QString> ctrlBlockList,int type)
{
	m_ctrlBlockList.clear();
	gsCrlBlockListWidget->clear();
	currnetType = type;
	gsCrlBlockListWidget->setRowCount(ctrlBlockList.size());
	gsCrlBlockListWidget->setColumnCount(gsTabHead.count());
	gsCrlBlockListWidget->setHorizontalHeaderLabels(gsTabHead);

	m_ctrlBlockList = ctrlBlockList;
	if(ctrlBlockList.size() == 0)
	{  
		rowSelect = 0;
		clearCell();
		return;
	}

	for(int i = 0; i<ctrlBlockList.size(); i++)
	{
		fillGsCrlBlockListTab(ctrlBlockList.value(i),i,type);
	}

	if(ctrlBlockList.size()>=rowSelect)
	{
		gsCrlBlockListWidget->selectRow(rowSelect);
	}
	else 
	{
		gsCrlBlockListWidget->selectRow(0);
	}
	//fillChannelListTab();
	//GetCtrlBlockInfo(QString ctrlBlockKey, GOOSECBStru& cbInfo);//获取Goose控制块信息
}
void  GooseSendWidget::GetDate(GOOSECBStru gooseinfo,QList <QString> &data)
{
	if(gooseinfo.CBName.baseAttributeName.isEmpty())
		data<<"";
	else 
		data<<gooseinfo.CBName.baseAttributeName;

	if(gooseinfo.DataSetName.baseAttributeName.isEmpty())
		data<<"";
	else 
		data<<gooseinfo.DataSetName.baseAttributeName;

	if(gooseinfo.AppID.baseAttributeName.isEmpty())
		data<<"";
	else 
		data<<gooseinfo.AppID.baseAttributeName;

	if(gooseinfo.MACAddr.baseAttributeName.isEmpty())
		data<<"";
	else 
		data<<gooseinfo.MACAddr.baseAttributeName;

	data<<QString::number(gooseinfo.DataSetCount.baseAttributeName,10);

	if(gooseinfo.VLANID.baseAttributeName.isEmpty())
		data<<"";
	else 
		data<<gooseinfo.VLANID.baseAttributeName;


	if(gooseinfo.VLANPriority.baseAttributeName.isEmpty())
		data<<"";
	else 
		data<<gooseinfo.VLANPriority.baseAttributeName;


	if(gooseinfo.ConfVersion.baseAttributeName.isEmpty())
		data<<"";
	else
		data<<gooseinfo.ConfVersion.baseAttributeName;
}

void   GooseSendWidget::GetDateDiff(GOOSECBStru gooseInfo,QList <QTableWidgetItem *> &data)
{
	QTableWidgetItem *appIDItem = new QTableWidgetItem;
	QTableWidgetItem *MACItem = new QTableWidgetItem;
	QTableWidgetItem *dataSetDescItem = new QTableWidgetItem;
	QTableWidgetItem *VlanItem = new QTableWidgetItem;
	QTableWidgetItem *vlnPItem = new QTableWidgetItem;
	QTableWidgetItem *conVerItem = new QTableWidgetItem;
	QTableWidgetItem *ctlNameItem = new QTableWidgetItem;
	QTableWidgetItem *dataSetNameItem = new QTableWidgetItem;
	if(gooseInfo.AppID.state == DIFFSTATE_EDITED)
	{
		appIDItem->setIcon(QIcon("./images/bianji.png"));
		appIDItem->setText(gooseInfo.AppID.baseAttributeName);
		appIDItem->setToolTip(gooseInfo.AppID.otherAttributeName);
	}
	else if(gooseInfo.AppID.state == DIFFSTATE_ADDED)
	{
		appIDItem->setIcon(QIcon("./images/zengjia.png"));
		appIDItem->setText(gooseInfo.AppID.otherAttributeName);
	}
	else if(gooseInfo.AppID.state == DIFFSTATE_REMOVED)
	{
		appIDItem->setIcon(QIcon("./images/shanchu.png"));
		appIDItem->setText(gooseInfo.AppID.baseAttributeName);
	}
	else if(gooseInfo.AppID.state == DIFFSTATE_NORMAL)
	{
		appIDItem->setText(gooseInfo.AppID.baseAttributeName);
	}

	if(gooseInfo.MACAddr.state == DIFFSTATE_EDITED)
	{
		MACItem->setIcon(QIcon("./images/bianji.png"));
		MACItem->setText(gooseInfo.MACAddr.baseAttributeName);
		MACItem->setToolTip(gooseInfo.MACAddr.otherAttributeName);
	}
	else if(gooseInfo.MACAddr.state == DIFFSTATE_ADDED)
	{
		MACItem->setIcon(QIcon("./images/zengjia.png"));
		MACItem->setText(gooseInfo.MACAddr.otherAttributeName);
	}
	else if(gooseInfo.MACAddr.state == DIFFSTATE_REMOVED)
	{
		MACItem->setIcon(QIcon("./images/shanchu.png"));
		MACItem->setText(gooseInfo.MACAddr.baseAttributeName);
	}
	else if(gooseInfo.MACAddr.state == DIFFSTATE_NORMAL)
	{
		MACItem->setText(gooseInfo.AppID.baseAttributeName);
	}

	if(gooseInfo.DataSetCount.state == DIFFSTATE_EDITED)
	{
		dataSetDescItem->setIcon(QIcon("./images/bianji.png"));
		dataSetDescItem->setText(QString::number(gooseInfo.DataSetCount.baseAttributeName,10));
		dataSetDescItem->setToolTip(gooseInfo.DataSetDesc.otherAttributeName);

	}
	else if(gooseInfo.DataSetCount.state == DIFFSTATE_ADDED)
	{
		dataSetDescItem->setIcon(QIcon("./images/zengjia.png"));
		dataSetDescItem->setText(QString::number(gooseInfo.DataSetCount.otherAttributeName,10));
	}
	else if(gooseInfo.DataSetCount.state == DIFFSTATE_REMOVED)
	{
		dataSetDescItem->setIcon(QIcon("./images/shanchu.png"));
		dataSetDescItem->setText(QString::number(gooseInfo.DataSetCount.baseAttributeName,10));
	}
	else if(gooseInfo.DataSetCount.state == DIFFSTATE_NORMAL)
	{
		dataSetDescItem->setText(QString::number(gooseInfo.DataSetCount.baseAttributeName,10));
	}

	if(gooseInfo.VLANID.state == DIFFSTATE_EDITED)
	{
		VlanItem->setIcon(QIcon("./images/bianji.png"));
		VlanItem->setText(gooseInfo.VLANID.baseAttributeName);
		VlanItem->setToolTip(gooseInfo.VLANID.otherAttributeName);
	}
	else if(gooseInfo.VLANID.state == DIFFSTATE_ADDED)
	{
		VlanItem->setIcon(QIcon("./images/zengjia.png"));
		VlanItem->setText(gooseInfo.VLANID.otherAttributeName);
	}
	else if(gooseInfo.VLANID.state == DIFFSTATE_REMOVED)
	{
		VlanItem->setIcon(QIcon("./images/shanchu.png"));
		VlanItem->setText(gooseInfo.VLANID.baseAttributeName);
	}
	else if(gooseInfo.VLANID.state == DIFFSTATE_NORMAL)
	{
		VlanItem->setText(gooseInfo.VLANID.baseAttributeName);

	}
	if(gooseInfo.VLANPriority.state == DIFFSTATE_EDITED)
	{
		vlnPItem->setText(gooseInfo.VLANPriority.baseAttributeName);
		vlnPItem->setIcon(QIcon("./images/bianji.png"));
		vlnPItem->setToolTip(gooseInfo.VLANPriority.otherAttributeName);
	}
	else if(gooseInfo.VLANPriority.state == DIFFSTATE_ADDED)
	{
		vlnPItem->setText(gooseInfo.VLANPriority.otherAttributeName);
		vlnPItem->setIcon(QIcon("./images/zengjia.png"));
	}
	else if(gooseInfo.VLANPriority.state == DIFFSTATE_REMOVED)
	{
		vlnPItem->setText(gooseInfo.VLANPriority.baseAttributeName);
		vlnPItem->setIcon(QIcon("./images/shanchu.png"));
	}
	else if(gooseInfo.VLANPriority.state == DIFFSTATE_NORMAL)
	{
		vlnPItem->setText(gooseInfo.VLANPriority.baseAttributeName);
	}

	if(gooseInfo.ConfVersion.state == DIFFSTATE_EDITED)
	{
		conVerItem->setIcon(QIcon("./images/bianji.png"));
		conVerItem->setText(gooseInfo.ConfVersion.baseAttributeName);
		conVerItem->setToolTip(gooseInfo.ConfVersion.otherAttributeName);
	}
	else if(gooseInfo.ConfVersion.state == DIFFSTATE_ADDED)
	{
		conVerItem->setIcon(QIcon("./images/zengjia.png"));
		conVerItem->setText(gooseInfo.ConfVersion.otherAttributeName);
	}
	else if(gooseInfo.ConfVersion.state == DIFFSTATE_REMOVED)
	{
		conVerItem->setIcon(QIcon("./images/shanchu.png"));
		conVerItem->setText(gooseInfo.ConfVersion.baseAttributeName);
	}
	else if(gooseInfo.ConfVersion.state == DIFFSTATE_NORMAL)
	{
		conVerItem->setText(gooseInfo.ConfVersion.baseAttributeName);
	}

	if(gooseInfo.CBName.state == DIFFSTATE_EDITED)
	{
		ctlNameItem->setIcon(QIcon("./images/bianji.png"));
		ctlNameItem->setText(gooseInfo.CBName.baseAttributeName);
		ctlNameItem->setToolTip(gooseInfo.CBName.otherAttributeName);
	}
	else if(gooseInfo.CBName.state == DIFFSTATE_ADDED)
	{
		ctlNameItem->setIcon(QIcon("./images/zengjia.png"));
		ctlNameItem->setText(gooseInfo.CBName.otherAttributeName);
	}
	else if(gooseInfo.CBName.state == DIFFSTATE_REMOVED)
	{
		ctlNameItem->setIcon(QIcon("./images/shanchu.png"));
		ctlNameItem->setText(gooseInfo.CBName.baseAttributeName);
	}
	else if(gooseInfo.CBName.state == DIFFSTATE_NORMAL)
	{
		ctlNameItem->setText(gooseInfo.CBName.baseAttributeName);
	}

	if(gooseInfo.DataSetName.state == DIFFSTATE_EDITED)
	{
		dataSetNameItem->setIcon(QIcon("./images/bianji.png"));
		dataSetNameItem->setText(gooseInfo.DataSetName.baseAttributeName);
		dataSetNameItem->setToolTip(gooseInfo.DataSetName.otherAttributeName);
	}
	else if(gooseInfo.DataSetName.state == DIFFSTATE_ADDED)
	{
		dataSetNameItem->setIcon(QIcon("./images/zengjia.png"));
		dataSetNameItem->setText(gooseInfo.DataSetName.otherAttributeName);
	}
	else if(gooseInfo.DataSetName.state == DIFFSTATE_REMOVED)
	{
		dataSetNameItem->setIcon(QIcon("./images/shanchu.png"));
		dataSetNameItem->setText(gooseInfo.DataSetName.baseAttributeName);
	}
	else if(gooseInfo.DataSetName.state == DIFFSTATE_NORMAL)
	{
		dataSetNameItem->setText(gooseInfo.DataSetName.baseAttributeName);
	}

	data.append(ctlNameItem);
	data.append(dataSetNameItem);
	data.append(appIDItem);
	data.append(MACItem);
	data.append(dataSetDescItem);
	data.append(VlanItem);
	data.append(vlnPItem);
	data.append(conVerItem);
}

void GooseSendWidget::fillGsCrlBlockListTab(QString ctrlName,int row,int type)
{
	const int NON_DIFF_STATUS = 1;
	const int MAX_COLUMN_COUNT = 8;
	GOOSECBStru gooseInfo;
	if(type == NON_DIFF_STATUS) //非差异化;
	{
		CTableSCLModelImpl tableSCLImpl;
		tableSCLImpl.GetCtrlBlockInfo(ctrlName,gooseInfo);
		QList <QString> data;
		GetDate(gooseInfo,data);

		for (uint i = 0; i != MAX_COLUMN_COUNT; i++)
		{
			gsCrlBlockListWidget->setItem(row,i,new QTableWidgetItem(data.at(i)));
		}
	}else  //差异化;
	{
		TableModelDiffImpl diffmpl;
		ItemList.clear();
		diffmpl.GetCtrlBlockInfo(ctrlName,gooseInfo);
		GetDateDiff(gooseInfo,ItemList);

		for (uint j = 0; j != MAX_COLUMN_COUNT; j++)
		{
			gsCrlBlockListWidget->setItem(row,j,ItemList.at(j));
		}
	}

	gsCrlBlockListWidget->resizeColumnsToContents();
	gsCrlBlockListWidget->resizeRowsToContents();
}
void GooseSendWidget::fillChannelListTab()
{
	const int NON_DIFF_STATUS = 1;

	if(gsCrlBlockListWidget->selectedItems().size() == 0)
		return;
	virtualTerminalTabWidget->clear();
	virtualTerminalTabWidget->setRowCount(0);
	QStringList  virtualTerminalHead;
	virtualTerminalHead<<""<<"内部地址"<<"外部IED"<<"外部端子描述";
	virtualTerminalTabWidget->setColumnCount(virtualTerminalHead.count());
	virtualTerminalTabWidget->setHorizontalHeaderLabels(virtualTerminalHead);
	int m_row = 0;
	int m_iOrd = 0;

	if(currnetType == NON_DIFF_STATUS)   //非差异化;
	{
		CTableSCLModelImpl tableSCLImpl; 
		QVector <QString> outSignalKeys;
		SignalStru signalInf;
		int rw = gsCrlBlockListWidget->selectedItems().at(0)->row();
		tableSCLImpl.GetOutSignalsByCBKey("", m_ctrlBlockList.at(rw), &outSignalKeys);//通过控制块获取输出信号列表;
		for(int i = 0; i < outSignalKeys.count(); i++)
		{
			m_iOrd++;
			tableSCLImpl.GetInfoOfOutSignal(outSignalKeys.at(i),&signalInf);
			virtualTerminalTabWidget->insertRow(m_row);
			virtualTerminalTabWidget->setItem(m_row, 0,new QTableWidgetItem(QString::number(m_iOrd)));
			virtualTerminalTabWidget->setItem(m_row,1,new QTableWidgetItem(signalInf.Desc.baseAttributeName));
			QVector <QString> insignaKeys;
			SignalStru sglstu;
			tableSCLImpl.GetInSignalsOfOutsignal(outSignalKeys.at(i),&insignaKeys);

			if(insignaKeys.count() == 1)
			{
				tableSCLImpl.GetInfoOfInSignal(insignaKeys.at(0),&sglstu);
				SCLNameTranslator translator;
				QString IEDKey_1 = translator.GetParentKey(insignaKeys.at(0).toLocal8Bit().constData()).c_str();
				IEDStru  IEDInfo;
				tableSCLImpl.GetIEDInfo(IEDKey_1,IEDInfo);
				QString iedDesc;
				if(IEDInfo.ID.isEmpty())
					iedDesc=IEDInfo.IEDDesc.baseAttributeName;
				else
				{
					QStringList str=IEDInfo.ID.split(".");
					iedDesc="["+str.at(3)+"]"+IEDInfo.IEDDesc.baseAttributeName;
				}
				virtualTerminalTabWidget->setItem(m_row,2,new QTableWidgetItem(iedDesc));
				virtualTerminalTabWidget->setItem(m_row,3,new QTableWidgetItem(sglstu.Desc.baseAttributeName));
				m_row++;
			}
			else if(insignaKeys.count() == 0)
			{
				virtualTerminalTabWidget->setItem(m_row,3,new QTableWidgetItem);
				m_row++;
			}
			else
			{
				int mm_row = m_row;
				for(int y = 0; y < insignaKeys.count(); y++)
				{
					if(y != 0)
						virtualTerminalTabWidget->insertRow(m_row);	 
					tableSCLImpl.GetInfoOfInSignal(insignaKeys.at(y),&sglstu);
					SCLNameTranslator translator;
					QString IEDKey_1 = translator.GetParentKey(insignaKeys.at(y).toLocal8Bit().constData()).c_str();
					IEDStru  IEDInfo;
					tableSCLImpl.GetIEDInfo(IEDKey_1,IEDInfo);
					QString iedDesc;
					if(IEDInfo.ID.isEmpty())
						iedDesc=IEDInfo.IEDDesc.baseAttributeName;
					else 
					{
						QStringList str=IEDInfo.ID.split(".");
						iedDesc="["+str.at(3)+"]"+IEDInfo.IEDDesc.baseAttributeName;
					}
					virtualTerminalTabWidget->setItem(m_row,2,new QTableWidgetItem(iedDesc));
					virtualTerminalTabWidget->setItem(m_row,3,new QTableWidgetItem(sglstu.Desc.baseAttributeName));
					m_row++;
				}
				int mb_row = m_row;
				virtualTerminalTabWidget->setSpan(mm_row,0,mb_row-mm_row,1);
				virtualTerminalTabWidget->setSpan(mm_row,1,mb_row-mm_row,1);
			}
		}
		rowSelect = rw;
	}
	else   //差异化;
	{
		TableModelDiffImpl tableSCLImpl; 
		QVector <QString> outSignalKeys;
		SignalStru signalInf;
		int rw= gsCrlBlockListWidget->selectedItems().at(0)->row();
		tableSCLImpl.GetOutSignalsByCBKey(IEDKEY, m_ctrlBlockList.at(rw), &outSignalKeys);//通过控制块获取输出信号列表;
		for(int i=0;i<outSignalKeys.count();i++)
		{
			m_iOrd++;
			tableSCLImpl.GetInfoOfOutSignal(outSignalKeys.at(i),&signalInf);
			QTableWidgetItem *item_1=new QTableWidgetItem;
			if(signalInf.Desc.state==DIFFSTATE_ADDED)
			{
				item_1->setText(signalInf. Desc.otherAttributeName);
				item_1->setIcon(QIcon("./images/zengjia.png"));
			}
			else if(signalInf.Desc.state==DIFFSTATE_EDITED)
			{
				item_1->setText(signalInf.Desc.baseAttributeName);
				item_1->setToolTip(signalInf.Desc.otherAttributeName);
				item_1->setIcon(QIcon("./images/bianjia.png"));
			}
			else if(signalInf.Desc.state==DIFFSTATE_REMOVED)
			{
				item_1->setText(signalInf.Desc.baseAttributeName);
				item_1->setIcon(QIcon("./images/shanchu.png"));
			}
			else if(signalInf.Desc.state==DIFFSTATE_NORMAL)
			{
				item_1->setText(signalInf.Desc.baseAttributeName);
			}
			virtualTerminalTabWidget->insertRow(m_row);
			virtualTerminalTabWidget->setItem(m_row, 0,new QTableWidgetItem(QString::number(m_iOrd)));
			virtualTerminalTabWidget->setItem(m_row,1,item_1);
			QVector <QString> insignaKeys;
			insignaKeys.clear();
			SignalStru sglstu;
			if(!outSignalKeys.at(i).isEmpty())
			{
				tableSCLImpl.GetInSignalsOfOutsignal(outSignalKeys.at(i),&insignaKeys);
				if(insignaKeys.count()==1)
				{     
					tableSCLImpl.GetInfoOfInSignal(insignaKeys.at(0),&sglstu);
					QTableWidgetItem *item_3=new QTableWidgetItem;
					if(sglstu.Desc.state==DIFFSTATE_ADDED)
					{
						item_3->setText(sglstu.Desc.otherAttributeName);
						item_3->setIcon(QIcon("./images/zengjia.png"));
					}
					else if(sglstu.Desc.state==DIFFSTATE_EDITED)
					{
						item_3->setText(sglstu.Desc.baseAttributeName);
						item_3->setToolTip(sglstu.Desc.otherAttributeName);
						item_3->setIcon(QIcon("./images/bianji.png"));
					}
					else if(sglstu.Desc.state==DIFFSTATE_REMOVED)
					{
						item_3->setText(sglstu.Desc.baseAttributeName);
						item_3->setIcon(QIcon("./images/shanchu.png"));
					}
					else if(sglstu.Desc.state==DIFFSTATE_NORMAL)
					{   
						item_3->setText(sglstu.Desc.baseAttributeName);
					}

					QTableWidgetItem *item=new QTableWidgetItem;
					SCLNameTranslator translator;
					QString IEDKey_1 = QString::fromStdString(translator.GetIEDKeyFromSCDDiffResult(insignaKeys.at(0).toStdString()));
					IEDStru  IEDInfo;
					tableSCLImpl.GetIEDInfo(IEDKey_1,IEDInfo);

					if(IEDInfo.IEDDesc.state==DIFFSTATE_ADDED)
					{
						QString iedDesc;
						if(IEDInfo.ID.isEmpty())
							iedDesc=IEDInfo.IEDDesc.otherAttributeName;
						else 
						{
							QStringList str=IEDInfo.ID.split(".");
							iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.otherAttributeName;

						}
						item->setText(iedDesc);
						item->setIcon(QIcon("./images/zengjia.png"));
					}
					else if(IEDInfo.IEDDesc.state==DIFFSTATE_EDITED)
					{
						QString iedDesc;
						if(IEDInfo.ID.isEmpty())
							iedDesc=IEDInfo.IEDDesc.baseAttributeName;
						else 
						{
							QStringList str=IEDInfo.ID.split(".");
							iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;

						}
						item->setText(iedDesc);
						item->setToolTip(IEDInfo.IEDDesc.otherAttributeName);
						item->setIcon(QIcon("./images/bianji.png"));

					}
					else if(IEDInfo.IEDDesc.state==DIFFSTATE_REMOVED)
					{
						QString iedDesc;
						if(IEDInfo.ID.isEmpty())
							iedDesc=IEDInfo.IEDDesc.baseAttributeName;
						else 
						{
							QStringList str=IEDInfo.ID.split(".");
							iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;

						}
						item->setText(iedDesc);
						item->setIcon(QIcon("./images/shanchu.png"));
					}
					else if(IEDInfo.IEDDesc.state==DIFFSTATE_NORMAL)
					{   
						QString iedDesc;
						if(IEDInfo.ID.isEmpty())
							iedDesc=IEDInfo.IEDDesc.baseAttributeName;
						else 
						{
							QStringList str=IEDInfo.ID.split(".");
							iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;

						}

						item->setText(iedDesc);
					}

					virtualTerminalTabWidget->setItem(m_row,2,item);
					virtualTerminalTabWidget->setItem(m_row,3,item_3);
					m_row++;
				}
				else if(insignaKeys.count()==0)
				{
					// qDebug()<<m_row;
					virtualTerminalTabWidget->setItem(m_row,2,new QTableWidgetItem);
					virtualTerminalTabWidget->setItem(m_row,3,new QTableWidgetItem);
					m_row++;
				}
				else if(insignaKeys.count()>=2)
				{
					int mm_row=m_row;
					for(int y=0;y<insignaKeys.count();y++)
					{
						if(y!=0)
							virtualTerminalTabWidget->insertRow(m_row);	 
						tableSCLImpl.GetInfoOfInSignal(insignaKeys.at(y),&sglstu);
						QTableWidgetItem *item_3=new QTableWidgetItem;
						if(sglstu.Desc.state==DIFFSTATE_ADDED)
						{
							item_3->setText(sglstu.Desc.otherAttributeName);
							item_3->setIcon(QIcon("./images/zengjia.png"));
						}
						else if(sglstu.Desc.state==DIFFSTATE_EDITED)
						{
							item_3->setText(sglstu.Desc.baseAttributeName);
							item_3->setToolTip(sglstu.Desc.otherAttributeName);
							item_3->setIcon(QIcon("./images/bianji.png"));
						}
						else if(sglstu.Desc.state==DIFFSTATE_REMOVED)
						{
							item_3->setText(sglstu.Desc.baseAttributeName);
							item_3->setIcon(QIcon("./images/shanchu.png"));
						}
						else if(sglstu.Desc.state==DIFFSTATE_NORMAL)
						{   
							item_3->setText(sglstu.Desc.baseAttributeName);
						}
						QTableWidgetItem *item=new QTableWidgetItem;
						SCLNameTranslator translator;
						QString IEDKey_1 = QString::fromStdString(translator.GetIEDKeyFromSCDDiffResult(insignaKeys.at(y).toStdString()));
						IEDStru  IEDInfo;
						tableSCLImpl.GetIEDInfo(IEDKey_1,IEDInfo);
						if(IEDInfo.IEDDesc.state==DIFFSTATE_ADDED)
						{
							QString iedDesc;
							if(IEDInfo.ID.isEmpty())
								iedDesc=IEDInfo.IEDDesc.otherAttributeName;
							else 
							{
								QStringList str=IEDInfo.ID.split(".");
								iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.otherAttributeName;

							}
							item->setText(iedDesc);
							item->setIcon(QIcon("./images/zengjia.png"));
						}
						else if(IEDInfo.IEDDesc.state==DIFFSTATE_EDITED)
						{
							QString iedDesc;
							if(IEDInfo.ID.isEmpty())
								iedDesc=IEDInfo.IEDDesc.baseAttributeName;
							else 
							{
								QStringList str=IEDInfo.ID.split(".");
								iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;

							}

							item->setText(iedDesc);
							item->setToolTip(IEDInfo.IEDDesc.otherAttributeName);
							item->setIcon(QIcon("./images/bianji.png"));

						}
						else if(IEDInfo.IEDDesc.state==DIFFSTATE_REMOVED)
						{
							QString iedDesc;
							if(IEDInfo.ID.isEmpty())
								iedDesc=IEDInfo.IEDDesc.otherAttributeName;
							else 
							{
								QStringList str=IEDInfo.ID.split(".");
								iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.otherAttributeName;

							}
							item->setText(IEDInfo.IEDDesc.baseAttributeName);
							item->setIcon(QIcon("./images/shanchu.png"));
						}
						else if(IEDInfo.IEDDesc.state==DIFFSTATE_NORMAL)
						{   
							QString iedDesc;
							if(IEDInfo.ID.isEmpty())
								iedDesc=IEDInfo.IEDDesc.baseAttributeName;
							else 
							{
								QStringList str=IEDInfo.ID.split(".");
								iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;

							}
							item->setText(iedDesc);
						}
						virtualTerminalTabWidget->setItem(m_row,2,item);
						virtualTerminalTabWidget->setItem(m_row,3,item_3);
						m_row++;
					}
					int mb_row=m_row;
					virtualTerminalTabWidget->setSpan(mm_row,0,mb_row-mm_row,1);
					virtualTerminalTabWidget->setSpan(mm_row,1,mb_row-mm_row,1);
				}
			}
		}
		rowSelect=rw;
	}
	virtualTerminalTabWidget->resizeColumnsToContents();
	virtualTerminalTabWidget->resizeRowsToContents();
}
void GooseSendWidget::clearCell()
{
	virtualTerminalTabWidget->clear();
	QStringList  virtualTerminalHead;
	virtualTerminalHead<<"内部地址"<<"外部IED"<<"外部端子描述";
	virtualTerminalTabWidget->setColumnCount(virtualTerminalHead.count());
	virtualTerminalTabWidget->setRowCount(0);
	virtualTerminalTabWidget->setHorizontalHeaderLabels(virtualTerminalHead);
}

GooseSendWidget::~GooseSendWidget()
{

}
void GooseSendWidget::clearData()
{
	virtualTerminalTabWidget->clear();
	QStringList  virtualTerminalHead;
	virtualTerminalHead<<"内部地址"<<"外部IED"<<"外部端子描述";
	virtualTerminalTabWidget->setHorizontalHeaderLabels(virtualTerminalHead);
	virtualTerminalTabWidget->setColumnCount(virtualTerminalHead.count());
	virtualTerminalTabWidget->setRowCount(0);

	gsCrlBlockListWidget->clear();
	gsCrlBlockListWidget->setHorizontalHeaderLabels(gsTabHead);
	gsCrlBlockListWidget->setColumnCount(gsTabHead.count());
	gsCrlBlockListWidget->setRowCount(0);
}
//********************************************************************************************
//sv send 
//********************************************************************************************
SvSendWidget::SvSendWidget(QWidget *parent)
	: QWidget(parent)
{

	QSplitter *splitter=new QSplitter(Qt::Vertical,this);
	QVBoxLayout *vboxLayout=new QVBoxLayout;
	svCrlBlockListWidget=new QTableWidget;
	MyitemDelegate *item=new MyitemDelegate(this);
	svCrlBlockListWidget->setItemDelegate(item);
	svCrlBlockListWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行;
	svCrlBlockListWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑;
	svCrlBlockListWidget->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色;
	connect(svCrlBlockListWidget,SIGNAL(itemSelectionChanged()),this,SLOT(fillChannelListTab()));

	gsTabHead<<"控制块名称"<<"数据集"<<"APPID"<<"MAC地址"<<"信息点数"<<"VLAN-ID";
	virtualTerminalTab=new QTabWidget;
	QTableWidget *ChannelListTabWidget=new QTableWidget; 
	ChannelListTabWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行;
	ChannelListTabWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑;
	ChannelListTabWidget->setFrameShape(QFrame::NoFrame); 
	QStringList ChannelListHead;
	ChannelListHead<<"序号"<<"类型"<<"描述";
	ChannelListTabWidget->setColumnCount(ChannelListHead.count());
	ChannelListTabWidget->setHorizontalHeaderLabels(ChannelListHead);


	virtualTerminalTabWidget=new QTableWidget;
	virtualTerminalTabWidget->setItemDelegate(item);
	virtualTerminalTabWidget->verticalHeader()->setVisible(false);
	virtualTerminalTabWidget->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色;
	virtualTerminalTabWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行;
	virtualTerminalTabWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑;
	virtualTerminalTabWidget->setFrameShape(QFrame::NoFrame); 
	//virtualTerminalTab->addTab(ChannelListTabWidget,tr("通道列表"));
	virtualTerminalTab->addTab(virtualTerminalTabWidget,tr("虚端子列表"));

	splitter->addWidget(svCrlBlockListWidget);
	splitter->addWidget(virtualTerminalTab);

	setLayout(vboxLayout);
	vboxLayout->addWidget(splitter);

}
void  SvSendWidget::GetSvCtrIndex(QVector<QString> ctrlBlockList,int type)
{
	svCrlBlockListWidget->clear();
	m_ctrlBlockList.clear();
	svCrlBlockListWidget->setColumnCount(gsTabHead.count());
	svCrlBlockListWidget->setRowCount(ctrlBlockList.size());
	svCrlBlockListWidget->setHorizontalHeaderLabels(gsTabHead);
	// m_ctrlBlockList=ctrlBlockList;
	currentType=type;
	m_ctrlBlockList=ctrlBlockList;
	if(ctrlBlockList.size()==0)
	{  
		rowSelect=0;
		clearCell();
		return;
	}
	for(int i=0;i<ctrlBlockList.size();i++)
	{
		fillSvCrlBlockListTab(ctrlBlockList.value(i),i,type);
	}

	if(ctrlBlockList.size()>=rowSelect)
	{

		svCrlBlockListWidget->selectRow(rowSelect);
	}
	else 
	{

		svCrlBlockListWidget->selectRow(0);
	}

}

void  SvSendWidget::GetDate(SVCBStru gooseinfo,QList <QString> &data)
{
	if(gooseinfo.CBName.baseAttributeName.isEmpty())
		data<<"";
	else 
		data<<gooseinfo.CBName.baseAttributeName;

	if(gooseinfo.DataSetName.baseAttributeName.isEmpty())
		data<<"";
	else 
		data<<gooseinfo.DataSetName.baseAttributeName;
	if(gooseinfo.AppID.baseAttributeName.isEmpty())
		data<<"";
	else 
		data<<gooseinfo.AppID.baseAttributeName;

	if(gooseinfo.MACAddr.baseAttributeName.isEmpty())
		data<<"";
	else 
		data<<gooseinfo.MACAddr.baseAttributeName;

	data<<QString::number(gooseinfo.DataSetCount.baseAttributeName,10);

	if(gooseinfo.VLANID.baseAttributeName.isEmpty())
		data<<"";
	else 
		data<<gooseinfo.VLANID.baseAttributeName;

}
void  SvSendWidget::GetDateDiff(SVCBStru svInfo,QList <QTableWidgetItem *> &data)
{
	QTableWidgetItem  *appIDItem=new QTableWidgetItem;
	QTableWidgetItem  *MACItem=new QTableWidgetItem;
	QTableWidgetItem  *dataSetDescItem=new QTableWidgetItem;
	QTableWidgetItem  *VlanItem=new QTableWidgetItem;
	//QTableWidgetItem  *vlnPItem=new QTableWidgetItem;
	//QTableWidgetItem  *conVerItem=new QTableWidgetItem;
	QTableWidgetItem *ctlNameItem=new QTableWidgetItem;
	QTableWidgetItem *dataSetNameItem=new QTableWidgetItem;

	if(svInfo.AppID.state==DIFFSTATE_EDITED)
	{
		appIDItem->setIcon(QIcon("./images/bianji.png"));
		appIDItem->setText(svInfo.AppID.baseAttributeName);
		appIDItem->setToolTip(svInfo.AppID.otherAttributeName);
	}
	else if(svInfo.AppID.state==DIFFSTATE_ADDED)
	{
		appIDItem->setIcon(QIcon("./images/zengjia.png"));
		appIDItem->setText(svInfo.AppID.otherAttributeName);
	}
	else if(svInfo.AppID.state==DIFFSTATE_REMOVED)
	{
		appIDItem->setIcon(QIcon("./images/shanchu.png"));
		appIDItem->setText(svInfo.AppID.baseAttributeName);
	}
	else if(svInfo.AppID.state==DIFFSTATE_NORMAL)
	{
		appIDItem->setText(svInfo.AppID.baseAttributeName);
	}


	if(svInfo.MACAddr.state==DIFFSTATE_EDITED)
	{
		MACItem->setIcon(QIcon("./images/bianji.png"));
		MACItem->setText(svInfo.MACAddr.baseAttributeName);
		MACItem->setToolTip(svInfo.MACAddr.otherAttributeName);
	}
	else if(svInfo.MACAddr.state==DIFFSTATE_ADDED)
	{
		MACItem->setIcon(QIcon("./images/zengjia.png"));
		MACItem->setText(svInfo.MACAddr.otherAttributeName);
	}
	else if(svInfo.MACAddr.state==DIFFSTATE_REMOVED)
	{
		MACItem->setIcon(QIcon("./images/shanchu.png"));
		MACItem->setText(svInfo.MACAddr.baseAttributeName);
	}
	else if(svInfo.MACAddr.state==DIFFSTATE_NORMAL)
	{
		MACItem->setText(svInfo.MACAddr.baseAttributeName);
	}

	if(svInfo.DataSetDesc.state==DIFFSTATE_EDITED)
	{
		dataSetDescItem->setIcon(QIcon("./images/bianji.png"));
		dataSetDescItem->setText(QString::number(svInfo.DataSetCount.baseAttributeName,10));
		dataSetDescItem->setToolTip(QString::number(svInfo.DataSetCount.otherAttributeName,10));
	}
	else if(svInfo.DataSetCount.state==DIFFSTATE_ADDED)
	{
		dataSetDescItem->setIcon(QIcon("./images/zengjia.png"));
		dataSetDescItem->setText(QString::number(svInfo.DataSetCount.otherAttributeName,10));
	}
	else if(svInfo.DataSetCount.state==DIFFSTATE_REMOVED)
	{
		dataSetDescItem->setIcon(QIcon("./images/shanchu.png"));
		dataSetDescItem->setText(QString::number(svInfo.DataSetCount.baseAttributeName,10));
	}
	else if(svInfo.DataSetCount.state==DIFFSTATE_NORMAL)
	{
		dataSetDescItem->setText(QString::number(svInfo.DataSetCount.baseAttributeName,10));
	}

	if(svInfo.VLANID.state==DIFFSTATE_EDITED)
	{
		VlanItem->setIcon(QIcon("./images/bianji.png"));
		VlanItem->setText(svInfo.VLANID.baseAttributeName);
		VlanItem->setToolTip(svInfo.VLANID.otherAttributeName);
	}
	else if(svInfo.VLANID.state==DIFFSTATE_ADDED)
	{
		VlanItem->setIcon(QIcon("./images/zengjia.png"));
		VlanItem->setText(svInfo.VLANID.otherAttributeName);
	}
	else if(svInfo.VLANID.state==DIFFSTATE_REMOVED)
	{
		VlanItem->setIcon(QIcon("./images/shanchu.png"));
		VlanItem->setText(svInfo.VLANID.baseAttributeName);
	}
	else if(svInfo.VLANID.state==DIFFSTATE_NORMAL)
	{
		VlanItem->setText(svInfo.VLANID.baseAttributeName);
	}

	if(svInfo.CBName.state==DIFFSTATE_EDITED)
	{
		ctlNameItem->setIcon(QIcon("./images/bianji.png"));
		ctlNameItem->setText(svInfo.CBName.baseAttributeName);
		ctlNameItem->setToolTip(svInfo.CBName.otherAttributeName);
	}
	else if(svInfo.CBName.state==DIFFSTATE_ADDED)
	{
		ctlNameItem->setIcon(QIcon("./images/zengjia.png"));
		ctlNameItem->setText(svInfo.CBName.otherAttributeName);
	}
	else if(svInfo.CBName.state==DIFFSTATE_REMOVED)
	{
		ctlNameItem->setIcon(QIcon("./images/shanchu.png"));
		ctlNameItem->setText(svInfo.CBName.baseAttributeName);
	}
	else if(svInfo.CBName.state==DIFFSTATE_NORMAL)
	{
		ctlNameItem->setText(svInfo.CBName.baseAttributeName);
	}


	if(svInfo.DataSetName.state==DIFFSTATE_EDITED)
	{
		dataSetNameItem->setIcon(QIcon("./images/bianji.png"));
		dataSetNameItem->setText(svInfo.DataSetName.baseAttributeName);
		dataSetNameItem->setToolTip(svInfo.DataSetName.otherAttributeName);
	}
	else if(svInfo.DataSetName.state==DIFFSTATE_ADDED)
	{
		dataSetNameItem->setIcon(QIcon("./images/zengjia.png"));
		dataSetNameItem->setText(svInfo.DataSetName.otherAttributeName);
	}
	else if(svInfo.DataSetName.state==DIFFSTATE_REMOVED)
	{
		dataSetNameItem->setIcon(QIcon("./images/shanchu.png"));
		dataSetNameItem->setText(svInfo.DataSetName.baseAttributeName);
	}
	else if(svInfo.DataSetName.state==DIFFSTATE_NORMAL)
	{
		dataSetNameItem->setText(svInfo.DataSetName.baseAttributeName);
	}

	data.append(ctlNameItem);
	data.append(dataSetNameItem);
	data.append(appIDItem);
	data.append(MACItem);
	data.append(dataSetDescItem);
	data.append(VlanItem);


}
void  SvSendWidget::fillSvCrlBlockListTab(QString ctrlName,int row,int type)
{

	SVCBStru svInfo;
	const int COLUMN_COUNT = 6;
	if(type==1) //非差异;
	{
		CTableSCLModelImpl tableSCLImpl;
		tableSCLImpl.GetCtrlBlockInfo(ctrlName,svInfo);
		QList <QString>data;
		GetDate(svInfo,data);
		for (uint i = 0; i != COLUMN_COUNT; i++)
		{
			svCrlBlockListWidget->setItem(row,i,new QTableWidgetItem(data.at(i)));
		}
	}
	else     //差异化;
	{
		TableModelDiffImpl diffmpl;
		QTableWidgetItem *appIDItem,*MACItem,*dateSetAddrItem,*IDItem,*vlnPItem,*conVerItem;
		diffmpl.GetCtrlBlockInfo(ctrlName,svInfo);
		ItemList.clear();
		GetDateDiff(svInfo,ItemList);
		for (uint i = 0; i != COLUMN_COUNT; i++)
		{
			svCrlBlockListWidget->setItem(row,i,ItemList.at(i));
		}
	}
	svCrlBlockListWidget->resizeColumnsToContents();
	svCrlBlockListWidget->resizeRowsToContents();
}
void SvSendWidget::fillChannelListTab()
{
	if(svCrlBlockListWidget->selectedItems().size()==0)
		return;

	virtualTerminalTabWidget->clear();
	virtualTerminalTabWidget->setRowCount(0);
	QStringList  virtualTerminalHead;
	virtualTerminalHead<<""<<"内部地址"<<"外部IED"<<"外部端子描述";
	virtualTerminalTabWidget->setColumnCount(virtualTerminalHead.count());
	virtualTerminalTabWidget->setHorizontalHeaderLabels(virtualTerminalHead);
	int m_row=0;
	int m_iOrd = 0;

	if(currentType==1)   //非差异;
	{
		CTableSCLModelImpl tableSCLImpl; 
		QVector <QString> outSignalKeys;
		SignalStru signalInf;
		int rw= svCrlBlockListWidget->selectedItems().at(0)->row();
		tableSCLImpl.GetOutSignalsByCBKey(IEDKEY, m_ctrlBlockList.at(rw), &outSignalKeys);//通过控制块获取输出信号列表;

		
		for(int i=0;i<outSignalKeys.count();i++)
		{
			m_iOrd++;
			tableSCLImpl.GetInfoOfOutSignal(outSignalKeys.at(i),&signalInf);
			virtualTerminalTabWidget->insertRow(m_row);
			virtualTerminalTabWidget->setItem(m_row, 0, new QTableWidgetItem(QString::number(m_iOrd)));
			virtualTerminalTabWidget->setItem(m_row, 1, new QTableWidgetItem(signalInf.Desc.baseAttributeName));
			
			QVector <QString> insignaKeys;
			SignalStru sglstu;
			tableSCLImpl.GetInSignalsOfOutsignal(outSignalKeys.at(i),&insignaKeys);

			if(insignaKeys.count()==1)
			{     
				tableSCLImpl.GetInfoOfInSignal(insignaKeys.at(0),&sglstu);
				SCLNameTranslator translator;
				QString IEDKey_1 = translator.GetParentKey(insignaKeys.at(0).toLocal8Bit().constData()).c_str();
				IEDStru  IEDInfo;
				tableSCLImpl.GetIEDInfo(IEDKey_1,IEDInfo);
				QString iedDesc;
				if(IEDInfo.ID.isEmpty())
					iedDesc=IEDInfo.IEDDesc.baseAttributeName;
				else 
				{
					QStringList str=IEDInfo.ID.split(".");
					iedDesc="["+str.at(3)+"]"+IEDInfo.IEDDesc.baseAttributeName;

				}
				virtualTerminalTabWidget->setItem(m_row,2,new QTableWidgetItem(iedDesc));
				virtualTerminalTabWidget->setItem(m_row,3,new QTableWidgetItem(sglstu.Desc.baseAttributeName));
				m_row++;
			}else if(insignaKeys.count()==0)
			{
				virtualTerminalTabWidget->setItem(m_row,3,new QTableWidgetItem);
				m_row++;
			}
			else
			{
				int mm_row=m_row;
				for(int y=0;y<insignaKeys.count();y++)
				{
					if(y!=0)
						virtualTerminalTabWidget->insertRow(m_row);	 
					tableSCLImpl.GetInfoOfInSignal(insignaKeys.at(y),&sglstu);
					SCLNameTranslator translator;
					QString IEDKey_1 = translator.GetParentKey(insignaKeys.at(y).toLocal8Bit().constData()).c_str();
					IEDStru  IEDInfo;
					tableSCLImpl.GetIEDInfo(IEDKey_1,IEDInfo);
					QString iedDesc;
					if(IEDInfo.ID.isEmpty())
						iedDesc=IEDInfo.IEDDesc.baseAttributeName;
					else 
					{
						QStringList str=IEDInfo.ID.split(".");
						iedDesc="["+str.at(3)+"]"+IEDInfo.IEDDesc.baseAttributeName;

					}
					virtualTerminalTabWidget->setItem(m_row,2,new QTableWidgetItem(iedDesc));
					virtualTerminalTabWidget->setItem(m_row,3,new QTableWidgetItem(sglstu.Desc.baseAttributeName));
					m_row++;
				}
				int mb_row=m_row;
				virtualTerminalTabWidget->setSpan(mm_row,0,mb_row-mm_row,1);
				virtualTerminalTabWidget->setSpan(mm_row,1,mb_row-mm_row,1);
			}
		}
		rowSelect=rw;

	}else    //  差异化;
	{
		TableModelDiffImpl tableSCLImpl; 
		QVector <QString> outSignalKeys;
		SignalStru signalInf;
		int rw= svCrlBlockListWidget->selectedItems().at(0)->row();
		tableSCLImpl.GetOutSignalsByCBKey(IEDKEY, m_ctrlBlockList.at(rw), &outSignalKeys);//通过控制块获取输出信号列表;
		int m_row=0;
		for(int i=0;i<outSignalKeys.count();i++)
		{
			m_iOrd++;
			tableSCLImpl.GetInfoOfOutSignal(outSignalKeys.at(i),&signalInf);
			virtualTerminalTabWidget->insertRow(m_row);
			QTableWidgetItem *item_1=new QTableWidgetItem;
			if(signalInf.Desc.state==DIFFSTATE_ADDED)
			{
				item_1->setText(signalInf.Desc.otherAttributeName);
				item_1->setIcon(QIcon("./images/zengjia.png"));
			}
			else if(signalInf.Desc.state==DIFFSTATE_EDITED)
			{
				item_1->setText(signalInf.Desc.baseAttributeName);
				item_1->setToolTip(signalInf.Desc.otherAttributeName);
				item_1->setIcon(QIcon("./images/bianjia.png"));

			}
			else if(signalInf.Desc.state==DIFFSTATE_REMOVED)
			{
				item_1->setText(signalInf.Desc.baseAttributeName);
				item_1->setIcon(QIcon("./images/shanchu.png"));
			}
			else if(signalInf.Desc.state==DIFFSTATE_NORMAL)
			{
				item_1->setText(signalInf.Desc.baseAttributeName);
			}
			virtualTerminalTabWidget->setItem(m_row, 0, new QTableWidgetItem(QString::number(m_iOrd)));
			virtualTerminalTabWidget->setItem(m_row, 1, item_1);

			//QStringList str=outSignalKeys.at(i).split(".");
			//virtualTerminalTabWidget->setItem(m_row,1,new QTableWidgetItem(str.at(1)));

			QVector <QString> insignaKeys;
			SignalStru sglstu;


			if(!outSignalKeys.at(i).isEmpty())
			{
				tableSCLImpl.GetInSignalsOfOutsignal(outSignalKeys.at(i),&insignaKeys);

				if(insignaKeys.count()==1)
				{     
					tableSCLImpl.GetInfoOfInSignal(insignaKeys.at(0),&sglstu);
					QTableWidgetItem *item_3=new QTableWidgetItem;
					if(sglstu.Desc.state==DIFFSTATE_ADDED)
					{
						item_3->setText(sglstu.Desc.otherAttributeName);
						item_3->setIcon(QIcon("./images/zengjia.png"));
					}
					else if(sglstu.Desc.state==DIFFSTATE_EDITED)
					{
						item_3->setText(sglstu.Desc.baseAttributeName);
						item_3->setToolTip(sglstu.Desc.otherAttributeName);
						item_3->setIcon(QIcon("./images/bianji.png"));
					}
					else if(sglstu.Desc.state==DIFFSTATE_REMOVED)
					{
						item_3->setText(sglstu.Desc.baseAttributeName);
						item_3->setIcon(QIcon("./images/shanchu.png"));
					}
					else if(sglstu.Desc.state==DIFFSTATE_NORMAL)
					{   
						item_3->setText(sglstu.Desc.baseAttributeName);
					}

					QTableWidgetItem *item=new QTableWidgetItem;
					SCLNameTranslator translator;
					QString IEDKey_1 = QString::fromStdString(translator.GetIEDKeyFromSCDDiffResult(insignaKeys.at(0).toStdString()));
					IEDStru  IEDInfo;
					tableSCLImpl.GetIEDInfo(IEDKey_1,IEDInfo);

					if(IEDInfo.IEDDesc.state==DIFFSTATE_ADDED)
					{
						QString iedDesc;
						if(IEDInfo.ID.isEmpty())
							iedDesc=IEDInfo.IEDDesc.otherAttributeName;
						else 
						{
							QStringList str=IEDInfo.ID.split(".");
							iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.otherAttributeName;
						}

						item->setText(iedDesc);
						item->setIcon(QIcon("./images/zengjia.png"));
					}
					else if(IEDInfo.IEDDesc.state==DIFFSTATE_EDITED)
					{
						QString iedDesc;
						if(IEDInfo.ID.isEmpty())
							iedDesc=IEDInfo.IEDDesc.baseAttributeName;
						else 
						{
							QStringList str=IEDInfo.ID.split(".");
							iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;
						}

						item->setText(iedDesc);
						item->setToolTip(IEDInfo.IEDDesc.otherAttributeName);
						item->setIcon(QIcon("./images/bianji.png"));

					}
					else if(IEDInfo.IEDDesc.state==DIFFSTATE_REMOVED)
					{
						QString iedDesc;
						if(IEDInfo.ID.isEmpty())
							iedDesc=IEDInfo.IEDDesc.baseAttributeName;
						else 
						{
							QStringList str=IEDInfo.ID.split(".");
							iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;

						}

						item->setText(iedDesc);
						item->setIcon(QIcon("./images/shanchu.png"));
					}
					else if(IEDInfo.IEDDesc.state==DIFFSTATE_NORMAL)
					{   
						QString iedDesc;
						if(IEDInfo.ID.isEmpty())
							iedDesc=IEDInfo.IEDDesc.baseAttributeName;
						else 
						{
							QStringList str=IEDInfo.ID.split(".");
							iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;

						}

						item->setText(iedDesc);
					}

					virtualTerminalTabWidget->setItem(m_row,2,item);
					virtualTerminalTabWidget->setItem(m_row,3,item_3);
					m_row++;
				}else if(insignaKeys.count()==0)
				{
					virtualTerminalTabWidget->setItem(m_row,3,new QTableWidgetItem);
					m_row++;
				}
				else if(insignaKeys.count()>=2)
				{
					int mm_row=m_row;
					for(int y=0;y<insignaKeys.count();y++)
					{
						if(y!=0)
							virtualTerminalTabWidget->insertRow(m_row);	 
						tableSCLImpl.GetInfoOfInSignal(insignaKeys.at(y),&sglstu);
						QTableWidgetItem *item_3=new QTableWidgetItem;
						if(sglstu.Desc.state==DIFFSTATE_ADDED)
						{
							item_3->setText(sglstu.Desc.otherAttributeName);
							item_3->setIcon(QIcon("./images/zengjia.png"));
						}
						else if(sglstu.Desc.state==DIFFSTATE_EDITED)
						{
							item_3->setText(sglstu.Desc.baseAttributeName);
							item_3->setToolTip(sglstu.Desc.otherAttributeName);
							item_3->setIcon(QIcon("./images/bianji.png"));
						}
						else if(sglstu.Desc.state==DIFFSTATE_REMOVED)
						{
							item_3->setText(sglstu.Desc.baseAttributeName);
							item_3->setIcon(QIcon("./images/shanchu.png"));
						}
						else if(sglstu.Desc.state==DIFFSTATE_NORMAL)
						{   
							item_3->setText(sglstu.Desc.baseAttributeName);
						}

						QTableWidgetItem *item=new QTableWidgetItem;
						SCLNameTranslator translator;
						QString IEDKey_1 = QString::fromStdString(translator.GetIEDKeyFromSCDDiffResult(insignaKeys.at(y).toStdString()));
						IEDStru  IEDInfo;
						tableSCLImpl.GetIEDInfo(IEDKey_1,IEDInfo);

						if(IEDInfo.IEDDesc.state==DIFFSTATE_ADDED)
						{
							QString iedDesc;
							if(IEDInfo.ID.isEmpty())
								iedDesc=IEDInfo.IEDDesc.otherAttributeName;
							else 
							{
								QStringList str=IEDInfo.ID.split(".");
								iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.otherAttributeName;

							}

							item->setText(iedDesc);
							item->setIcon(QIcon("./images/zengjia.png"));
						}
						else if(IEDInfo.IEDDesc.state==DIFFSTATE_EDITED)
						{
							QString iedDesc;
							if(IEDInfo.ID.isEmpty())
								iedDesc=IEDInfo.IEDDesc.baseAttributeName;
							else 
							{
								QStringList str=IEDInfo.ID.split(".");
								iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;

							}

							item->setText(iedDesc);
							item->setToolTip(IEDInfo.IEDDesc.otherAttributeName);
							item->setIcon(QIcon("./images/bianji.png"));

						}
						else if(IEDInfo.IEDDesc.state==DIFFSTATE_REMOVED)
						{
							QString iedDesc;
							if(IEDInfo.ID.isEmpty())
								iedDesc=IEDInfo.IEDDesc.baseAttributeName;
							else 
							{
								QStringList str=IEDInfo.ID.split(".");
								iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;

							}

							item->setText(iedDesc);
							item->setIcon(QIcon("./images/shanchu.png"));
						}
						else if(IEDInfo.IEDDesc.state==DIFFSTATE_NORMAL)
						{   
							QString iedDesc;
							if(IEDInfo.ID.isEmpty())
								iedDesc=IEDInfo.IEDDesc.baseAttributeName;
							else 
							{
								QStringList str=IEDInfo.ID.split(".");
								iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;

							}
							item->setText(iedDesc);
						}
						virtualTerminalTabWidget->setItem(m_row,2,item);
						virtualTerminalTabWidget->setItem(m_row,3,item_3);
						m_row++;
					}
					int mb_row=m_row;
					virtualTerminalTabWidget->setSpan(mm_row,0,mb_row-mm_row,1);
					virtualTerminalTabWidget->setSpan(mm_row,1,mb_row-mm_row,1);
				}
			}
		}
		rowSelect=rw;	
	}

	virtualTerminalTabWidget->resizeColumnsToContents();
	virtualTerminalTabWidget->resizeRowsToContents();

}
void SvSendWidget::setIEDKey(QString IEDkey)
{
	IEDKEY=IEDkey;
}
void SvSendWidget::clearCell()
{
	virtualTerminalTabWidget->clear();
	QStringList  virtualTerminalHead;
	virtualTerminalHead<<"内部地址"<<"外部IED"<<"外部端子描述";
	virtualTerminalTabWidget->setColumnCount(virtualTerminalHead.count());
	virtualTerminalTabWidget->setRowCount(0);
	virtualTerminalTabWidget->setHorizontalHeaderLabels(virtualTerminalHead);
}
SvSendWidget::~SvSendWidget()
{


}
void SvSendWidget::clearData()
{
	svCrlBlockListWidget->clear(); 
	svCrlBlockListWidget->setHorizontalHeaderLabels(gsTabHead);
	svCrlBlockListWidget->setColumnCount(gsTabHead.count());
	svCrlBlockListWidget->setRowCount(0);

	virtualTerminalTabWidget->clear();
	QStringList  virtualTerminalHead;
	virtualTerminalHead<<"内部地址"<<"外部IED"<<"外部端子描述";
	virtualTerminalTabWidget->setColumnCount(virtualTerminalHead.count());
	virtualTerminalTabWidget->setHorizontalHeaderLabels(virtualTerminalHead);
	virtualTerminalTabWidget->setRowCount(0);
}
//Report send 
//********************************************************************************************
ReportSendWidget::ReportSendWidget(QWidget *parent)
	: QWidget(parent)
{
	InitReportCBName();
	QSplitter *splitter=new QSplitter(Qt::Vertical,this);
	QVBoxLayout *vboxLayout=new QVBoxLayout;
	reportCrlBlockListWidget=new QTableWidget;
	MyitemDelegate *item=new MyitemDelegate(this);
	reportCrlBlockListWidget->setItemDelegate(item);
	reportCrlBlockListWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行;
	reportCrlBlockListWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑;
	reportCrlBlockListWidget->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色;
	connect(reportCrlBlockListWidget,SIGNAL(itemSelectionChanged()),this,SLOT(fillChannelListTab()));
	// gsTabHead<<"控制块名称"<<"数据集"<<"APPID"<<"MAC地址"<<"信息点数"<<"VLAN-ID";
	gsTabHead<<"逻辑设备"<<"控制块名称"<<"控制块描述"<<"数据集"<<"RptID"<<"完整性周期（intgPd）"<<"配置版本号"<<"是否缓存"<<"缓存时间"<<"信息点数";
	virtualTerminalTab=new QTabWidget;
	QTableWidget *ChannelListTabWidget=new QTableWidget; 
	ChannelListTabWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行;
	ChannelListTabWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑;
	ChannelListTabWidget->setFrameShape(QFrame::NoFrame); 
	QStringList ChannelListHead;
	ChannelListHead<<"序号"<<"类型"<<"描述";
	ChannelListTabWidget->setColumnCount(ChannelListHead.count());
	ChannelListTabWidget->setHorizontalHeaderLabels(ChannelListHead);
	virtualTerminalTabWidget=new QTableWidget;
	virtualTerminalTabWidget->setItemDelegate(item);
	virtualTerminalTabWidget->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色;
	virtualTerminalTabWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行;
	virtualTerminalTabWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑;
	virtualTerminalTabWidget->setFrameShape(QFrame::NoFrame); 

	QHBoxLayout *topHLayout = new QHBoxLayout();
	ExportButton = new QPushButton("导出", this);
	connect(ExportButton, SIGNAL(clicked()), this, SLOT(exportXLSFile()));

	QSpacerItem *hSpacer = new QSpacerItem(80,20,QSizePolicy::Expanding, QSizePolicy::Minimum);
	topHLayout->addWidget(ExportButton);
	topHLayout->addItem(hSpacer);

	virtualTerminalTab->addTab(virtualTerminalTabWidget,tr("信息条目列表"));
	splitter->addWidget(reportCrlBlockListWidget);
	splitter->addWidget(virtualTerminalTab);
	setLayout(vboxLayout);
	vboxLayout->addLayout(topHLayout);
	vboxLayout->addWidget(splitter);
	m_DataSetter = new TabInfoDataSetter(QString("信息点表"));

	m_ProcessBar = new FixedProcessBar(this);
}

void ReportSendWidget::InitReportCBName()
{
	m_ReportCBNames.insert("urcbAin", "遥测");
	m_ReportCBNames.insert("brcbDin", "遥信");
	m_ReportCBNames.insert("brcbAlarm", "故障信号");
	m_ReportCBNames.insert("brcbWarning", "告警信号");
	m_ReportCBNames.insert("brcbCommState", "通信工况");
	m_ReportCBNames.insert("brcbInterLock", "联锁");

	m_ReportCBNames.insert("brcbTripInfo", "保护事件");
	m_ReportCBNames.insert("brcbRelayEna", "保护压板");
	m_ReportCBNames.insert("brcbRelayRec", "保护录波");
	m_ReportCBNames.insert("urcbRelayAin", "保护遥测");
	m_ReportCBNames.insert("brcbRelayDin", "保护遥信");
	m_ReportCBNames.insert("brcbInterLock", "联锁");
}

void  ReportSendWidget::GetReportCtrIndex(QVector<QString> ctrlBlockList,int type)
{
	reportCrlBlockListWidget->clear();
	m_ctrlBlockList.clear();
	reportCrlBlockListWidget->setColumnCount(gsTabHead.count());
	reportCrlBlockListWidget->setRowCount(ctrlBlockList.size());
	reportCrlBlockListWidget->setHorizontalHeaderLabels(gsTabHead);
	currentType=type;
	m_ctrlBlockList=ctrlBlockList;
	if(ctrlBlockList.size()==0)
	{  
		rowSelect=0;
		clearCell();
		return;
	}
	for(int i=0;i<ctrlBlockList.size();i++)
	{
		fillReportCrlBlockListTab(ctrlBlockList.value(i),i,type);
	}
	// 默认选择第一条
	reportCrlBlockListWidget->selectRow(0);
	// InitVirtualTerminalTabWidgetHead();
	//if(ctrlBlockList.size()>=rowSelect)
	//{
	//	reportCrlBlockListWidget->selectRow(rowSelect);
	//}
	//else 
	//{
	//	reportCrlBlockListWidget->selectRow(0);
	//}
}
void  ReportSendWidget::GetDate(ReportCBStru gooseinfo,QList <QString> &data)
{
	if(gooseinfo.LDInst.isEmpty())
		data<<"";
	else 
		data<<gooseinfo.LDInst;

	if(gooseinfo.CBName.baseAttributeName.isEmpty())
		data<<"";
	else 
		data<<gooseinfo.CBName.baseAttributeName;

	QMap<QString, QString>::iterator it;
	bool isFind = false;
	for (it = m_ReportCBNames.begin(); it != m_ReportCBNames.end(); it++)
	{
		QString key = it.key();
		if (data.at(1).contains(key))
		{
			isFind = true;
			data<< it.value();
		}
	}

	if (!isFind)
	{
		data<<"";
	}

	if(gooseinfo.DataSetName.baseAttributeName.isEmpty())
		data<<"";
	else 
		data<<gooseinfo.DataSetName.baseAttributeName;
	if(gooseinfo.apAppID.baseAttributeName.isEmpty())
		data<<"";
	else 
		data<<gooseinfo.apAppID.baseAttributeName;

	data<<gooseinfo.intgPd;
	if(gooseinfo.ConfVersion.baseAttributeName.isEmpty())
		data<<"";
	else 
		data<<gooseinfo.ConfVersion.baseAttributeName;
	data<<gooseinfo.Buffered;
	data<<gooseinfo.bufTime;

	//if(gooseinfo.MACAddr.baseAttributeName.isEmpty())
	//	data<<"";
	//else 
	//	data<<gooseinfo.MACAddr.baseAttributeName;
	data<<QString::number(gooseinfo.DataSetCount.baseAttributeName,10);
	//if(gooseinfo.VLANID.baseAttributeName.isEmpty())
	//	data<<"";
	//else 
	//	data<<gooseinfo.VLANID.baseAttributeName;
}

void  ReportSendWidget::GetDateDiff(ReportCBStru svInfo,QList <QTableWidgetItem *> &data)
{
	QTableWidgetItem  *appIDItem=new QTableWidgetItem;
	QTableWidgetItem  *MACItem=new QTableWidgetItem;
	QTableWidgetItem  *dataSetDescItem=new QTableWidgetItem;
	QTableWidgetItem  *VlanItem=new QTableWidgetItem;
	QTableWidgetItem *ctlNameItem=new QTableWidgetItem;
	QTableWidgetItem *dataSetNameItem=new QTableWidgetItem;
	if(svInfo.AppID.state==DIFFSTATE_EDITED)
	{
		appIDItem->setIcon(QIcon("./images/bianji.png"));
		appIDItem->setText(svInfo.AppID.baseAttributeName);
		appIDItem->setToolTip(svInfo.AppID.otherAttributeName);
	}
	else if(svInfo.AppID.state==DIFFSTATE_ADDED)
	{
		appIDItem->setIcon(QIcon("./images/zengjia.png"));
		appIDItem->setText(svInfo.AppID.otherAttributeName);
	}
	else if(svInfo.AppID.state==DIFFSTATE_REMOVED)
	{
		appIDItem->setIcon(QIcon("./images/shanchu.png"));
		appIDItem->setText(svInfo.AppID.baseAttributeName);
	}
	else if(svInfo.AppID.state==DIFFSTATE_NORMAL)
	{
		appIDItem->setText(svInfo.AppID.baseAttributeName);
	}
	if(svInfo.MACAddr.state==DIFFSTATE_EDITED)
	{
		MACItem->setIcon(QIcon("./images/bianji.png"));
		MACItem->setText(svInfo.MACAddr.baseAttributeName);
		MACItem->setToolTip(svInfo.MACAddr.otherAttributeName);
	}
	else if(svInfo.MACAddr.state==DIFFSTATE_ADDED)
	{
		MACItem->setIcon(QIcon("./images/zengjia.png"));
		MACItem->setText(svInfo.MACAddr.otherAttributeName);
	}
	else if(svInfo.MACAddr.state==DIFFSTATE_REMOVED)
	{
		MACItem->setIcon(QIcon("./images/shanchu.png"));
		MACItem->setText(svInfo.MACAddr.baseAttributeName);
	}
	else if(svInfo.MACAddr.state==DIFFSTATE_NORMAL)
	{
		MACItem->setText(svInfo.MACAddr.baseAttributeName);
	}
	if(svInfo.DataSetDesc.state==DIFFSTATE_EDITED)
	{
		dataSetDescItem->setIcon(QIcon("./images/bianji.png"));
		dataSetDescItem->setText(QString::number(svInfo.DataSetCount.baseAttributeName,10));
		dataSetDescItem->setToolTip(QString::number(svInfo.DataSetCount.otherAttributeName,10));
	}
	else if(svInfo.DataSetCount.state==DIFFSTATE_ADDED)
	{
		dataSetDescItem->setIcon(QIcon("./images/zengjia.png"));
		dataSetDescItem->setText(QString::number(svInfo.DataSetCount.otherAttributeName,10));
	}
	else if(svInfo.DataSetCount.state==DIFFSTATE_REMOVED)
	{
		dataSetDescItem->setIcon(QIcon("./images/shanchu.png"));
		dataSetDescItem->setText(QString::number(svInfo.DataSetCount.baseAttributeName,10));
	}
	else if(svInfo.DataSetCount.state==DIFFSTATE_NORMAL)
	{
		dataSetDescItem->setText(QString::number(svInfo.DataSetCount.baseAttributeName,10));
	}
	if(svInfo.VLANID.state==DIFFSTATE_EDITED)
	{
		VlanItem->setIcon(QIcon("./images/bianji.png"));
		VlanItem->setText(svInfo.VLANID.baseAttributeName);
		VlanItem->setToolTip(svInfo.VLANID.otherAttributeName);
	}
	else if(svInfo.VLANID.state==DIFFSTATE_ADDED)
	{
		VlanItem->setIcon(QIcon("./images/zengjia.png"));
		VlanItem->setText(svInfo.VLANID.otherAttributeName);
	}
	else if(svInfo.VLANID.state==DIFFSTATE_REMOVED)
	{
		VlanItem->setIcon(QIcon("./images/shanchu.png"));
		VlanItem->setText(svInfo.VLANID.baseAttributeName);
	}
	else if(svInfo.VLANID.state==DIFFSTATE_NORMAL)
	{
		VlanItem->setText(svInfo.VLANID.baseAttributeName);
	}
	if(svInfo.CBName.state==DIFFSTATE_EDITED)
	{
		ctlNameItem->setIcon(QIcon("./images/bianji.png"));
		ctlNameItem->setText(svInfo.CBName.baseAttributeName);
		ctlNameItem->setToolTip(svInfo.CBName.otherAttributeName);
	}
	else if(svInfo.CBName.state==DIFFSTATE_ADDED)
	{
		ctlNameItem->setIcon(QIcon("./images/zengjia.png"));
		ctlNameItem->setText(svInfo.CBName.otherAttributeName);
	}
	else if(svInfo.CBName.state==DIFFSTATE_REMOVED)
	{
		ctlNameItem->setIcon(QIcon("./images/shanchu.png"));
		ctlNameItem->setText(svInfo.CBName.baseAttributeName);
	}
	else if(svInfo.CBName.state==DIFFSTATE_NORMAL)
	{
		ctlNameItem->setText(svInfo.CBName.baseAttributeName);
	}
	if(svInfo.DataSetName.state==DIFFSTATE_EDITED)
	{
		dataSetNameItem->setIcon(QIcon("./images/bianji.png"));
		dataSetNameItem->setText(svInfo.DataSetName.baseAttributeName);
		dataSetNameItem->setToolTip(svInfo.DataSetName.otherAttributeName);
	}
	else if(svInfo.DataSetName.state==DIFFSTATE_ADDED)
	{
		dataSetNameItem->setIcon(QIcon("./images/zengjia.png"));
		dataSetNameItem->setText(svInfo.DataSetName.otherAttributeName);
	}
	else if(svInfo.DataSetName.state==DIFFSTATE_REMOVED)
	{
		dataSetNameItem->setIcon(QIcon("./images/shanchu.png"));
		dataSetNameItem->setText(svInfo.DataSetName.baseAttributeName);
	}
	else if(svInfo.DataSetName.state==DIFFSTATE_NORMAL)
	{
		dataSetNameItem->setText(svInfo.DataSetName.baseAttributeName);
	}
	data.append(ctlNameItem);
	data.append(dataSetNameItem);
	data.append(appIDItem);
	data.append(MACItem);
	data.append(dataSetDescItem);
	data.append(VlanItem);
}

void  ReportSendWidget::fillReportCrlBlockListTab(QString ctrlName,int row,int type)
{
	ReportCBStru svInfo;
	const int COLUMN_COUNT = 10;
	
	if(type == 1) //非差异;
	{
		CTableSCLModelImpl tableSCLImpl;
		tableSCLImpl.GetCtrlBlockInfo(ctrlName,svInfo);
		QList <QString>data;
		GetDate(svInfo,data);
		for (uint i = 0; i != COLUMN_COUNT; i++)
		{
			reportCrlBlockListWidget->setItem(row,i,new QTableWidgetItem(data.at(i)));
		}
		//reportCrlBlockListWidget->setItem(row,9,new QTableWidgetItem(data.at(9)));
		//reportCrlBlockListWidget->setItem(row,10,new QTableWidgetItem(data.at(10)));
	}
	else     //差异化;
	{
		TableModelDiffImpl diffmpl;
		QTableWidgetItem *appIDItem,*MACItem,*dataSetDescItem,*dateSetAddrItem,*IDItem,*VlanItem,*vlnPItem,*conVerItem;
		diffmpl.GetCtrlBlockInfo(ctrlName,svInfo);
		ItemList.clear();
		GetDateDiff(svInfo,ItemList);
		for (uint i = 0; i != COLUMN_COUNT - 1; i++)
		{
			reportCrlBlockListWidget->setItem(row,i,ItemList.at(i));
		}
		//reportCrlBlockListWidget->setItem(row,9,ItemList.at(9));
		//reportCrlBlockListWidget->setItem(row,10,ItemList.at(10));
	}
	reportCrlBlockListWidget->resizeColumnsToContents();
	reportCrlBlockListWidget->resizeRowsToContents();
}

void ReportSendWidget::InitVirtualTerminalTabWidgetHead()
{
	virtualTerminalTabWidget->clear();
	virtualTerminalTabWidget->setRowCount(0);
	QStringList  virtualTerminalHead;
	virtualTerminalHead<<"内部地址"<<"信息条目描述"<<"数据类型";
	virtualTerminalTabWidget->setColumnCount(virtualTerminalHead.count());
	virtualTerminalTabWidget->setHorizontalHeaderLabels(virtualTerminalHead);
}

void ReportSendWidget::fillChannelListTab()
{
	if(reportCrlBlockListWidget->selectedItems().size()==0)
		return;
	InitVirtualTerminalTabWidgetHead();
	if(currentType==1)   //非差异;
	{
		CTableSCLModelImpl tableSCLImpl; 
		QVector <QString> outSignalKeys;
		DSSettingSignalStru dsSignaInf;
		SignalStru signalInf;
		int rw= reportCrlBlockListWidget->selectedItems().at(0)->row();
		tableSCLImpl.GetOutSignalsByCBKey(IEDKEY, m_ctrlBlockList.at(rw), &outSignalKeys);//通过控制块获取输出信号列表;
		
		int m_row=0;
		QString intAddress;
		for(int i=0;i<outSignalKeys.count();i++)
		{
			tableSCLImpl.GetInfoOfOutSignal(outSignalKeys.at(i),&signalInf);
			tableSCLImpl.GetInfoOfDSSettingOutSignal(outSignalKeys.at(i), &dsSignaInf);
			intAddress = m_NameTransLator.GetSignalIntAddressFromSignalID(signalInf.ID);
			virtualTerminalTabWidget->insertRow(m_row);
			virtualTerminalTabWidget->setItem(m_row,0,new QTableWidgetItem(intAddress));
			virtualTerminalTabWidget->setItem(m_row,1,new QTableWidgetItem(signalInf.Desc.baseAttributeName));
			
			virtualTerminalTabWidget->setItem(m_row,2,new QTableWidgetItem(dsSignaInf.cdc));
		}
		rowSelect=rw;
	}else    //  差异化;
	{
		TableModelDiffImpl tableSCLImpl; 
		QVector <QString> outSignalKeys;
		SignalStru signalInf;
		int rw= reportCrlBlockListWidget->selectedItems().at(0)->row();
		tableSCLImpl.GetOutSignalsByCBKey(IEDKEY, m_ctrlBlockList.at(rw), &outSignalKeys);//通过控制块获取输出信号列表;
		int m_row=0;
		for(int i=0;i<outSignalKeys.count();i++)
		{
			tableSCLImpl.GetInfoOfOutSignal(outSignalKeys.at(i),&signalInf);
			virtualTerminalTabWidget->insertRow(m_row);
			QTableWidgetItem *item_1=new QTableWidgetItem;
			if(signalInf.Desc.state==DIFFSTATE_ADDED)
			{
				item_1->setText(signalInf.Desc.otherAttributeName);
				item_1->setIcon(QIcon("./images/zengjia.png"));
			}
			else if(signalInf.Desc.state==DIFFSTATE_EDITED)
			{
				item_1->setText(signalInf.Desc.baseAttributeName);
				item_1->setToolTip(signalInf.Desc.otherAttributeName);
				item_1->setIcon(QIcon("./images/bianjia.png"));
			}
			else if(signalInf.Desc.state==DIFFSTATE_REMOVED)
			{
				item_1->setText(signalInf.Desc.baseAttributeName);
				item_1->setIcon(QIcon("./images/shanchu.png"));
			}
			else if(signalInf.Desc.state==DIFFSTATE_NORMAL)
			{
				item_1->setText(signalInf.Desc.baseAttributeName);
			}
			virtualTerminalTabWidget->setItem(m_row,0,item_1);
		}
		rowSelect=rw;	
	}
	virtualTerminalTabWidget->resizeColumnsToContents();
	virtualTerminalTabWidget->resizeRowsToContents();
}

void ReportSendWidget::getMainData(QMap<QString,QStringList>* mainMap)
{
	for (uint i = 0; i != m_ctrlBlockList.size(); i++)
	{
		QList<QString> tempDataList;
		ReportCBStru svInfo;
		CTableSCLModelImpl tableSCLImpl;
		tableSCLImpl.GetCtrlBlockInfo(m_ctrlBlockList.at(i), svInfo);
		GetDate(svInfo,tempDataList);
		mainMap->insert(m_ctrlBlockList.at(i), tempDataList);
	}
}

void ReportSendWidget::getSubsidiaryData(QMap<QString,QVector<QStringList>>* subsidaryMap)
{
	CTableSCLModelImpl tableSCLImpl;
	for (uint i = 0; i != m_ctrlBlockList.size(); i++)
	{
		QVector<QStringList> subsDataVec;
		QVector <QString> outSignalKeys;
		tableSCLImpl.GetOutSignalsByCBKey(IEDKEY, m_ctrlBlockList.at(i), &outSignalKeys);
		
		for (uint j = 0; j != outSignalKeys.size(); j++)
		{
			QStringList subsData;
			DSSettingSignalStru dsSignaInf;
			SignalStru signalInf;
			tableSCLImpl.GetInfoOfOutSignal(outSignalKeys.at(j),&signalInf);
			tableSCLImpl.GetInfoOfDSSettingOutSignal(outSignalKeys.at(j), &dsSignaInf);
			QString intAddress = m_NameTransLator.GetSignalIntAddressFromSignalID(signalInf.ID);
			subsData << intAddress << signalInf.Desc.baseAttributeName << dsSignaInf.cdc;
			subsDataVec.append(subsData);
		}
		subsidaryMap->insert(m_ctrlBlockList.at(i), subsDataVec);
	}
}

void ReportSendWidget::exportXLSFile()
{
	m_ProcessBar->show();
	QCoreApplication::processEvents(QEventLoop::AllEvents,1);
	
	QString nameDesc = "";
	CTableSCLModelImpl tableSCLImpl;
	IEDStru iedInfo;
	tableSCLImpl.GetIEDInfo(IEDKEY, iedInfo);
	nameDesc = QString("[") + iedInfo.IEDName + QString("]") + iedInfo.IEDDesc.baseAttributeName;
	

	m_DataSetter->setIEDNameDesc(nameDesc);
	m_DataSetter->setMainColumnNames(gsTabHead);

	QStringList subsNames;
	subsNames<<"内部地址"<<"信息条目描述"<<"数据类型";
	m_DataSetter->setSubsidiaryColumnNames(subsNames);

	QMap<QString, QStringList> tempMainData;
	QMap<QString, QVector<QStringList>> tempSubsData;
	getMainData(&tempMainData);
	getSubsidiaryData(&tempSubsData);

	m_DataSetter->setMainData(tempMainData);
	m_DataSetter->setSubsidiaryData(tempSubsData);

	m_DataSetter->ExportXLS();

	m_ProcessBar->hide();
}

void ReportSendWidget::setIEDKey(QString IEDkey)
{
	IEDKEY = IEDkey;
}

void ReportSendWidget::clearCell()
{
	virtualTerminalTabWidget->clear();
	QStringList  virtualTerminalHead;
	virtualTerminalHead<<"内部地址"<<"外部IED"<<"外部端子描述";
	virtualTerminalTabWidget->setColumnCount(virtualTerminalHead.count());
	virtualTerminalTabWidget->setRowCount(0);
	virtualTerminalTabWidget->setHorizontalHeaderLabels(virtualTerminalHead);
}

ReportSendWidget::~ReportSendWidget()
{
	delete m_DataSetter;
}

void ReportSendWidget::clearData()
{
	reportCrlBlockListWidget->clear(); 
	reportCrlBlockListWidget->setHorizontalHeaderLabels(gsTabHead);
	reportCrlBlockListWidget->setColumnCount(gsTabHead.count());
	reportCrlBlockListWidget->setRowCount(0);
	virtualTerminalTabWidget->clear();
	QStringList  virtualTerminalHead;
	virtualTerminalHead<<"内部地址"<<"外部IED"<<"外部端子描述";
	virtualTerminalTabWidget->setColumnCount(virtualTerminalHead.count());
	virtualTerminalTabWidget->setHorizontalHeaderLabels(virtualTerminalHead);
	virtualTerminalTabWidget->setRowCount(0);
}


//DsSettingWidget send 
//********************************************************************************************
DsSettingWidget::DsSettingWidget(QWidget *parent)
	: QWidget(parent)
{
	QSplitter *splitter=new QSplitter(Qt::Vertical,this);
	QVBoxLayout *vboxLayout=new QVBoxLayout;
	DsSettingListWidget=new QTableWidget;
	MyitemDelegate *item=new MyitemDelegate(this);
	DsSettingListWidget->setItemDelegate(item);
	DsSettingListWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行;
	DsSettingListWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑;
	DsSettingListWidget->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色;
	connect(DsSettingListWidget,SIGNAL(itemSelectionChanged()),this,SLOT(fillChannelListTab()));
	gsTabHead<<"访问点"<<"逻辑设备"<<"数据集"<<"数据集名称";
	virtualTerminalTab=new QTabWidget;
	QTableWidget *ChannelListTabWidget=new QTableWidget; 
	ChannelListTabWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行;
	ChannelListTabWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑;
	ChannelListTabWidget->setFrameShape(QFrame::NoFrame); 
	QStringList ChannelListHead;
	ChannelListHead<<"序号"<<"类型"<<"描述";
	ChannelListTabWidget->setColumnCount(ChannelListHead.count());
	ChannelListTabWidget->setHorizontalHeaderLabels(ChannelListHead);
	virtualTerminalTabWidget=new QTableWidget;
	virtualTerminalTabWidget->setItemDelegate(item);
	virtualTerminalTabWidget->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色;
	virtualTerminalTabWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行;
	virtualTerminalTabWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑;
	virtualTerminalTabWidget->setFrameShape(QFrame::NoFrame); 
	virtualTerminalTab->addTab(virtualTerminalTabWidget,tr("定值条目列表"));
	
	QHBoxLayout *topHLayout = new QHBoxLayout();
	ExportButton = new QPushButton("导出", this);
	connect(ExportButton, SIGNAL(clicked()), this, SLOT(exportXLSFile()));

	QSpacerItem *hSpacer = new QSpacerItem(80,20,QSizePolicy::Expanding, QSizePolicy::Minimum);
	topHLayout->addWidget(ExportButton);
	topHLayout->addItem(hSpacer);

	splitter->addWidget(DsSettingListWidget);
	splitter->addWidget(virtualTerminalTab);
	setLayout(vboxLayout);
	vboxLayout->addLayout(topHLayout);
	vboxLayout->addWidget(splitter);
	m_DataSetter = new TabInfoDataSetter(QString("定值条目"));
}

void  DsSettingWidget::GetDsSettingIndex(QVector<QDataSetInfo> dataSetList,int type)
{
	DsSettingListWidget->clear();
	m_DsSettingList.clear();
	DsSettingListWidget->setColumnCount(gsTabHead.count());
	DsSettingListWidget->setRowCount(dataSetList.size());
	DsSettingListWidget->setHorizontalHeaderLabels(gsTabHead);
	currentType=type;
	m_DsSettingList=dataSetList;
	if(dataSetList.size()==0)
	{
		rowSelect=0;
		clearCell();
		return;
	}
	for(int i=0;i<dataSetList.size();i++)
	{
		fillDsSettingListTab(dataSetList.at(i),i,type);
	}
	DsSettingListWidget->selectRow(0);
	//if(dataSetList.size()>=rowSelect)
	//{
	//	DsSettingListWidget->selectRow(rowSelect);
	//}
	//else 
	//{
	//	DsSettingListWidget->selectRow(0);
	//}
}
void  DsSettingWidget::GetDate(QDataSetInfo gooseinfo,QList <QString> &data)
{
	// TODO
}
void  DsSettingWidget::GetDateDiff(QDataSetInfo svInfo,QList <QTableWidgetItem *> &data)
{
	// TODO
}
void  DsSettingWidget::fillDsSettingListTab(QDataSetInfo dataSetName,int row,int type)
{
	if(type==1) //非差异;
	{
		//QList <QString>data;
		//GetDate(dataSetName,data);
		DsSettingListWidget->setItem(row,0,new QTableWidgetItem(dataSetName.ApName));
		DsSettingListWidget->setItem(row,1,new QTableWidgetItem(dataSetName.LDInst));
		DsSettingListWidget->setItem(row,2,new QTableWidgetItem(dataSetName.dataSetName));
		DsSettingListWidget->setItem(row,3,new QTableWidgetItem(dataSetName.dataSetDesc));//信息点数;
	}
	//else     //差异化;
	//{
	//	TableModelDiffImpl diffmpl;
	//	QTableWidgetItem *appIDItem,*MACItem,*dataSetDescItem,*dateSetAddrItem,*IDItem,*VlanItem,*vlnPItem,*conVerItem;
	//	diffmpl.GetCtrlBlockInfo(ctrlName,svInfo);
	//	ItemList.clear();
	//	GetDateDiff(svInfo,ItemList);
	//	reportCrlBlockListWidget->setItem(row,0,ItemList.at(0));
	//	reportCrlBlockListWidget->setItem(row,1,ItemList.at(1));
	//	reportCrlBlockListWidget->setItem(row,2,ItemList.at(2));//信息点数;
	//	reportCrlBlockListWidget->setItem(row,3,ItemList.at(3));
	//	reportCrlBlockListWidget->setItem(row,4,ItemList.at(4));
	//	reportCrlBlockListWidget->setItem(row,5,ItemList.at(5));
	//	reportCrlBlockListWidget->setItem(row,6,ItemList.at(6));
	//	reportCrlBlockListWidget->setItem(row,7,ItemList.at(7));//信息点数;
	//	reportCrlBlockListWidget->setItem(row,8,ItemList.at(8));
	//	reportCrlBlockListWidget->setItem(row,9,ItemList.at(9));
	//	reportCrlBlockListWidget->setItem(row,10,ItemList.at(10));
	//}
	DsSettingListWidget->resizeColumnsToContents();
	DsSettingListWidget->resizeRowsToContents();
}
void DsSettingWidget::fillChannelListTab()
{
	if(DsSettingListWidget->selectedItems().size()==0)
		return;
	virtualTerminalTabWidget->clear();
	virtualTerminalTabWidget->setRowCount(0);
	QStringList  virtualTerminalHead;
	// virtualTerminalHead<<"内部短地址"<<"定值条目描述"<<"数据类型"<<"单位"<<"最大值"<<"最小值"<<"步长";
	virtualTerminalHead<<"定值条目描述"<<"最大值"<<"最小值"<<"单位"<<"步长"<<"数据类型"<<"内部短地址";
	virtualTerminalTabWidget->setColumnCount(virtualTerminalHead.count());
	virtualTerminalTabWidget->setHorizontalHeaderLabels(virtualTerminalHead);
	if(currentType==1)   //非差异;
	{
		CTableSCLModelImpl tableSCLImpl; 
		QVector <QString> outSignalKeys;
		SignalStru signalInf;
		DSSettingSignalStru dsSignaInf;
		int rw= DsSettingListWidget->selectedItems().at(0)->row();
		tableSCLImpl.GetOutSignalsByDataSetKey(IEDKEY, m_DsSettingList.at(rw).dataSetName, &outSignalKeys);//通过控制块获取输出信号列表;
		int m_row=0;
		QString intAddress;
		for(int i=0;i<outSignalKeys.count();i++)
		{
			tableSCLImpl.GetInfoOfOutSignal(outSignalKeys.at(i),&signalInf);
			tableSCLImpl.GetInfoOfDSSettingOutSignal(outSignalKeys.at(i), &dsSignaInf);
			virtualTerminalTabWidget->insertRow(m_row);
			intAddress = m_NameTransLator.GetSignalIntAddressFromSignalID(signalInf.ID);
			//virtualTerminalTabWidget->setItem(m_row,0,new QTableWidgetItem(intAddress));
			//virtualTerminalTabWidget->setItem(m_row,1,new QTableWidgetItem(signalInf.Desc.baseAttributeName));
			//virtualTerminalTabWidget->setItem(m_row,2,new QTableWidgetItem(dsSignaInf.cdc));
			//virtualTerminalTabWidget->setItem(m_row,3,new QTableWidgetItem(dsSignaInf.unit));
			//virtualTerminalTabWidget->setItem(m_row,4,new QTableWidgetItem(dsSignaInf.maxVal));
			//virtualTerminalTabWidget->setItem(m_row,5,new QTableWidgetItem(dsSignaInf.minVal));
			//virtualTerminalTabWidget->setItem(m_row,6,new QTableWidgetItem(dsSignaInf.stepSize));

			virtualTerminalTabWidget->setItem(m_row,0,new QTableWidgetItem(signalInf.Desc.baseAttributeName));
			virtualTerminalTabWidget->setItem(m_row,1,new QTableWidgetItem(dsSignaInf.maxVal));
			virtualTerminalTabWidget->setItem(m_row,2,new QTableWidgetItem(dsSignaInf.minVal));
			virtualTerminalTabWidget->setItem(m_row,3,new QTableWidgetItem(dsSignaInf.unit));
			virtualTerminalTabWidget->setItem(m_row,4,new QTableWidgetItem(dsSignaInf.stepSize));
			virtualTerminalTabWidget->setItem(m_row,5,new QTableWidgetItem(dsSignaInf.cdc));
			virtualTerminalTabWidget->setItem(m_row,6,new QTableWidgetItem(intAddress));
		}
		rowSelect = rw;
	}
	virtualTerminalTabWidget->resizeColumnsToContents();
	virtualTerminalTabWidget->resizeRowsToContents();
}

void DsSettingWidget::getMainData(QMap<QString,QStringList>* mainMap)
{
	for (uint i = 0; i != m_DsSettingList.size(); i++)
	{
		QStringList dsData;
		dsData << m_DsSettingList.at(i).ApName << m_DsSettingList.at(i).LDInst << m_DsSettingList.at(i).dataSetName << m_DsSettingList.at(i).dataSetDesc;
		mainMap->insert(QString::number(i), dsData);
	}
}

void DsSettingWidget::getSubsidiaryData(QMap<QString,QVector<QStringList>>* subsidaryMap)
{
	for (uint i = 0; i != m_DsSettingList.size(); i++)
	{
		CTableSCLModelImpl tableSCLImpl;
		QVector<QStringList> subsDataVec;
		QVector<QString> outSignalKeys;
		tableSCLImpl.GetOutSignalsByDataSetKey(IEDKEY, m_DsSettingList.at(i).dataSetName, &outSignalKeys);//通过控制块获取输出信号列表;
		for(uint j = 0; j < outSignalKeys.count(); j++)
		{
			QStringList subsData;
			SignalStru signalInf;
			DSSettingSignalStru dsSignaInf;
			tableSCLImpl.GetInfoOfOutSignal(outSignalKeys.at(j),&signalInf);
			tableSCLImpl.GetInfoOfDSSettingOutSignal(outSignalKeys.at(j), &dsSignaInf);
			QString intAddress = m_NameTransLator.GetSignalIntAddressFromSignalID(signalInf.ID);
			subsData << intAddress << signalInf.Desc.baseAttributeName << dsSignaInf.cdc << dsSignaInf.unit << dsSignaInf.maxVal << dsSignaInf.minVal << dsSignaInf.stepSize;
			subsDataVec.append(subsData);
		}
		subsidaryMap->insert(QString::number(i), subsDataVec);
	}
}

void DsSettingWidget::exportXLSFile()
{
	QString nameDesc = "";
	CTableSCLModelImpl tableSCLImpl;
	IEDStru iedInfo;
	tableSCLImpl.GetIEDInfo(IEDKEY, iedInfo);
	nameDesc = QString("[") + iedInfo.IEDName + QString("]") + iedInfo.IEDDesc.baseAttributeName;

	m_DataSetter->setIEDNameDesc(nameDesc);

	m_DataSetter->setMainColumnNames(gsTabHead);

	QStringList subsNames;
	subsNames<<"内部短地址"<<"定值条目描述"<<"数据类型"<<"单位"<<"最大值"<<"最小值"<<"步长";
	m_DataSetter->setSubsidiaryColumnNames(subsNames);

	QMap<QString, QStringList> tempMainData;
	QMap<QString, QVector<QStringList>> tempSubsData;
	getMainData(&tempMainData);
	getSubsidiaryData(&tempSubsData);

	m_DataSetter->setMainData(tempMainData);
	m_DataSetter->setSubsidiaryData(tempSubsData);

	m_DataSetter->ExportXLS();
}

void DsSettingWidget::setIEDKey(QString IEDkey)
{
	IEDKEY = IEDkey;
}

void DsSettingWidget::clearCell()
{
	virtualTerminalTabWidget->clear();
	QStringList  virtualTerminalHead;
	virtualTerminalHead<<"内部地址"<<"外部IED"<<"外部端子描述";
	virtualTerminalTabWidget->setColumnCount(virtualTerminalHead.count());
	virtualTerminalTabWidget->setRowCount(0);
	virtualTerminalTabWidget->setHorizontalHeaderLabels(virtualTerminalHead);
}

DsSettingWidget::~DsSettingWidget()
{
	delete m_DataSetter;
}

void DsSettingWidget::clearData()
{
	DsSettingListWidget->clear(); 
	DsSettingListWidget->setHorizontalHeaderLabels(gsTabHead);
	DsSettingListWidget->setColumnCount(gsTabHead.count());
	DsSettingListWidget->setRowCount(0);
	virtualTerminalTabWidget->clear();
	QStringList  virtualTerminalHead;
	virtualTerminalHead<<"内部地址";
	virtualTerminalTabWidget->setColumnCount(virtualTerminalHead.count());
	virtualTerminalTabWidget->setHorizontalHeaderLabels(virtualTerminalHead);
	virtualTerminalTabWidget->setRowCount(0);
}