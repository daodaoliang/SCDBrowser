#include "Receivewidget.h"
#include <QSplitter>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <QHeaderView>
#include <QDateTime>
#include <QMessageBox>
#include "myitemdelegate.h"

//goose receive
GooseReceiveWidget::GooseReceiveWidget(QWidget *parent)
	: QWidget(parent)
{
	setMouseTracking(true);
	QSplitter *spliterMain = new QSplitter(Qt::Vertical,this);
	gooseRecvListWidget = new QTableWidget(spliterMain);
	//My							//	 设置委托;
	MyitemDelegate *item =new MyitemDelegate(this);
	gooseRecvListWidget->setItemDelegate(item);

	gooseRecvHead<<"内部短地址"<<"内部信号描述"<<"物理端口"<<"外部IED"<<"外部信号描述";
	gooseRecvListWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行;
	gooseRecvListWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置不可编辑;
	gooseRecvListWidget->setStyleSheet("selection-background-color:lightblue;"); 
	gooseRecvListWidget->setMouseTracking(true);

	connect(gooseRecvListWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(setSelecttion(QTableWidgetItem*)));

	QSplitter *splitercl=new QSplitter(Qt::Horizontal,spliterMain);
	QVBoxLayout *vbox=new QVBoxLayout(this);

	gooseExternalCtllListWidget=new QTableWidget(splitercl);
	connect(gooseExternalCtllListWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(setReverse(QTableWidgetItem*)));
	gooseExternalCtllListWidget->setItemDelegate(item);
	gooseExternalCtllListWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
	gooseExternalCtllListWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑
	gooseExternalCtlHead<<"APPID"<<"所在IED"<<"控制块名称"<<"数据集";
	gooseExternalCtllListWidget->setColumnCount(gooseExternalCtlHead.count());
	gooseExternalCtllListWidget->setHorizontalHeaderLabels(gooseExternalCtlHead);
	gooseExternalCtllListWidget->setStyleSheet("selection-background-color:lightblue;"); 
	vbox->addWidget(spliterMain);	
}
void GooseReceiveWidget::clearData()
{
	gooseExternalCtllListWidget->clear();
	gooseExternalCtllListWidget->setColumnCount(gooseExternalCtlHead.count());
	gooseExternalCtllListWidget->setHorizontalHeaderLabels(gooseExternalCtlHead);
	gooseExternalCtllListWidget->setRowCount(0);

	gooseRecvListWidget->clear();
	gooseRecvListWidget->setColumnCount(gooseRecvHead.count());
	gooseRecvListWidget->setHorizontalHeaderLabels(gooseRecvHead);
	gooseRecvListWidget->setRowCount(0);
}
void GooseReceiveWidget::GetExtRefStruInfo(QVector<ExtRefStru> ExtRefs,int y,int index) 
{
	gooseRecvListWidget->clear();
	ctrList.clear();
	CtrIndex.clear();
	SiganlToCtrl.clear();
	mCtrlList.clear();
	crlAppIed.clear();

	gooseRecvListWidget->setColumnCount(gooseRecvHead.count());
	gooseRecvListWidget->setHorizontalHeaderLabels(gooseRecvHead);
	gooseRecvListWidget->setRowCount(0);

	gooseExternalCtllListWidget->setColumnCount(gooseExternalCtlHead.count());
	gooseExternalCtllListWidget->setRowCount(0);
	gooseExternalCtllListWidget->setHorizontalHeaderLabels(gooseExternalCtlHead);
	int row=0;
	int Excellrow=0;
	int m_count=0;
	if(y==0)   //非差异化;
	{
		CTableSCLModelImpl modeImpl;
		GOOSECBStru CBKey;
		GOOSECBStru CBKey_1;
		for(int i=0;i<ExtRefs.count();i++)
		{
			if(ExtRefs.at(i).type==typeGoose_)
			{ 
				m_count++;
				emit enbleTanFouce(true);
				gooseRecvListWidget->insertRow(row);

				QTableWidgetItem *item=new QTableWidgetItem;
				item->setText(ExtRefs.at(i).IntAddr.baseAttributeName);
				gooseRecvListWidget->setItem(row,0,item);

				QTableWidgetItem *item_1=new QTableWidgetItem;
				item_1->setText(ExtRefs.at(i).desc.baseAttributeName);
				gooseRecvListWidget->setItem(row,1,item_1);

				QTableWidgetItem *ptItem = new QTableWidgetItem;
				ptItem->setText(ExtRefs.at(i).PortName.baseAttributeName);
				gooseRecvListWidget->setItem(row,2,ptItem);

				QTableWidgetItem *item_2=new QTableWidgetItem; 
				SCLNameTranslator translator;
				QString IEDKey_1 = translator.GetParentKey(ExtRefs.at(i).srcSignalID.toLocal8Bit().constData()).c_str();
				IEDStru  IEDInfo;
				modeImpl.GetIEDInfo(IEDKey_1,IEDInfo);
				QString iedDesc;
				QStringList str=QStringList()<<"NULL"<<"NULL"<<"NULL"<<"NULL";
				if(IEDInfo.ID.isEmpty())
					iedDesc=IEDInfo.IEDDesc.baseAttributeName;
				else 
				{
					str=IEDInfo.ID.split(".");
					iedDesc="["+str.at(3)+"]"+IEDInfo.IEDDesc.baseAttributeName;
				}
				item_2->setText(iedDesc);
				gooseRecvListWidget->setItem(row,3,item_2);

				QTableWidgetItem *item_3=new QTableWidgetItem;   //外部信号描述
				SignalStru signalInfo;
				modeImpl.GetInfoOfOutSignal(ExtRefs.at(i).srcSignalID,&signalInfo);
				item_3->setText(signalInfo.Desc.baseAttributeName);
				gooseRecvListWidget->setItem(row,4,item_3);
				modeImpl.GetCtrlBlockOfOutIEDByInsignal(ExtRefs.at(i).InSigInfo.ID, &CBKey_1);

				QHash <QString,QString> m_CtlIed;
				m_CtlIed.insert(CBKey_1.AppID.baseAttributeName,str.at(3));
				mCtrlList.insert(row,m_CtlIed); 
				
				if(!crlAppIed.contains(CBKey_1.AppID.baseAttributeName))
				{
					gooseExternalCtllListWidget->insertRow(Excellrow);
					fillGooseExternalCtllListWidget_1(CBKey_1,Excellrow,ExtRefs,i);
					crlAppIed.insert(CBKey_1.AppID.baseAttributeName,str.at(3));
					Excellrow++;
				}else 
				{
					QList <QString> ctlkey=crlAppIed.keys();
					for(int y=0;y<ctlkey.count();y++)
					{
						if(ctlkey.at(y)==CBKey_1.AppID.baseAttributeName)
						{
							if(str.at(3)!=crlAppIed.value(ctlkey.at(y)))
							{
								gooseExternalCtllListWidget->insertRow(Excellrow);
								fillGooseExternalCtllListWidget_1(CBKey_1,Excellrow,ExtRefs,i);
								crlAppIed.insert(CBKey_1.AppID.baseAttributeName,str.at(3));
								Excellrow++;
								break;
							}
						}
					}	
				}
				row++;
			}	   
		}
		if(m_count==0)
		{
			emit enbleTanFouce(false);
		}
	}
	else     //差异化;
	{
		TableModelDiffImpl diifimpl;
		for(int i=0;i<ExtRefs.count();i++)
		{ 
			GOOSECBStru CBKey;
			GOOSECBStru CBKey_1;
			if(ExtRefs.at(i).type==typeGoose_)
			{
				m_count++;
				emit enbleTanFouce(true);
				gooseRecvListWidget->insertRow(row);

				QTableWidgetItem *ptItem = new QTableWidgetItem;
				if (ExtRefs.at(i).PortName.state == DIFFSTATE_EDITED)
				{
					ptItem->setText(ExtRefs.at(i).PortName.baseAttributeName);
					ptItem->setToolTip(ExtRefs.at(i).PortName.otherAttributeName);
					ptItem->setIcon(QIcon("./images/bianji.png"));
					gooseRecvListWidget->setItem(row,2,ptItem);
				}
				else if (ExtRefs.at(i).PortName.state == DIFFSTATE_ADDED)
				{
					ptItem->setText(ExtRefs.at(i).PortName.otherAttributeName);
					ptItem->setIcon(QIcon("./images/zengjia.png"));
					gooseRecvListWidget->setItem(row,2,ptItem);
				}
				else if (ExtRefs.at(i).PortName.state == DIFFSTATE_REMOVED)
				{
					ptItem->setText(ExtRefs.at(i).PortName.baseAttributeName);
					ptItem->setIcon(QIcon("./images/shanchu.png"));
					gooseRecvListWidget->setItem(row,2,ptItem);
				}
				else if (ExtRefs.at(i).PortName.state == DIFFSTATE_NORMAL)
				{
					ptItem->setText(ExtRefs.at(i).PortName.baseAttributeName);
					gooseRecvListWidget->setItem(row,2,ptItem);
				}

				QTableWidgetItem *item=new QTableWidgetItem;
				if(ExtRefs.at(i).IntAddr.state==DIFFSTATE_EDITED)
				{  
					item->setText(ExtRefs.at(i).IntAddr.baseAttributeName);                      // 4 代表编辑 ;
					item->setToolTip(ExtRefs.at(i).IntAddr.otherAttributeName);
					item->setIcon(QIcon("./images/bianji.png"));
					gooseRecvListWidget->setItem(row,0,item);
				}else if(ExtRefs.at(i).IntAddr.state==DIFFSTATE_ADDED)
				{
					item->setText(ExtRefs.at(i).IntAddr.otherAttributeName);                          // 4 代表编辑;
					item->setIcon(QIcon("./images/zengjia.png"));
					gooseRecvListWidget->setItem(row,0,item);
				}
				else if(ExtRefs.at(i).IntAddr.state==DIFFSTATE_REMOVED)
				{
					item->setText(ExtRefs.at(i).IntAddr.baseAttributeName);                       // 4 代表编辑;
					item->setIcon(QIcon("./images/shanchu.png"));
					gooseRecvListWidget->setItem(row,0,item);
				}
				else if(ExtRefs.at(i).IntAddr.state==DIFFSTATE_NORMAL)
				{
					item->setText(ExtRefs.at(i).IntAddr.baseAttributeName);   
					//item->setText(ExtRefs.at(i).IntAddr.baseAttributeName);
					gooseRecvListWidget->setItem(row,0,item);
				} 
				
				QTableWidgetItem *item_1=new QTableWidgetItem;
				if(ExtRefs.at(i).desc.state==DIFFSTATE_EDITED)
				{
					item_1->setText(ExtRefs.at(i).desc.baseAttributeName);
					item_1->setToolTip(ExtRefs.at(i).desc.otherAttributeName);
					item_1->setIcon(QIcon("./images/bianji.png"));
					gooseRecvListWidget->setItem(row,1,item_1);

				}else if(ExtRefs.at(i).desc.state==DIFFSTATE_ADDED)
				{
					item_1->setText(ExtRefs.at(i).desc.otherAttributeName);
					item_1->setIcon(QIcon("./images/zengjia.png"));
					gooseRecvListWidget->setItem(row,1,item_1);		
				}
				else if(ExtRefs.at(i).desc.state==DIFFSTATE_REMOVED)
				{
					item_1->setText(ExtRefs.at(i).desc.baseAttributeName);                                   // 4 代表编辑;
					item_1->setIcon(QIcon("./images/shanchu.png"));
					gooseRecvListWidget->setItem(row,1,item_1);
				}
				else if(ExtRefs.at(i).desc.state==DIFFSTATE_NORMAL)
				{
					item_1->setText(ExtRefs.at(i).desc.baseAttributeName);
					gooseRecvListWidget->setItem(row,1,item_1);
				} 

				QTableWidgetItem *item_2=new QTableWidgetItem;   // 外部ied;
				SCLNameTranslator translator;
				QString IEDKey_1 = QString::fromStdString(translator.GetIEDKeyFromSCDDiffResult(ExtRefs.at(i).srcSignalID.toStdString()));
				IEDStru  IEDInfo;
				diifimpl.GetIEDInfo(IEDKey_1,IEDInfo);
				QStringList str=QStringList()<<"NULL"<<"NULL"<<"NULL"<<"NULL";
				if(IEDInfo.IEDDesc.state==DIFFSTATE_EDITED)
				{
					QString iedDesc;
					if(IEDInfo.ID.isEmpty())
						iedDesc=IEDInfo.IEDDesc.baseAttributeName;
					else 
					{
						str=IEDInfo.ID.split(".");
						iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;

					}
					item_2->setText(iedDesc);
					item_2->setToolTip(IEDInfo.IEDDesc.otherAttributeName);
					item_2->setIcon(QIcon("./images/bianji.png"));
					gooseRecvListWidget->setItem(row,3,item_2);

				}else if(IEDInfo.IEDDesc.state==DIFFSTATE_ADDED)
				{
					QString iedDesc;
					if(IEDInfo.ID.isEmpty())
						iedDesc=IEDInfo.IEDDesc.otherAttributeName;
					else 
					{
						str=IEDInfo.ID.split(".");
						iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.otherAttributeName;

					}
					item_2->setText(iedDesc);
					item_2->setIcon(QIcon("./images/zengjia.png"));
					gooseRecvListWidget->setItem(row,3,item_2);		
				}
				else if(IEDInfo.IEDDesc.state==DIFFSTATE_REMOVED) 
				{
					QString iedDesc;
					if(IEDInfo.ID.isEmpty())
						iedDesc=IEDInfo.IEDDesc.baseAttributeName;
					else 
					{
						str=IEDInfo.ID.split(".");
						iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;

					}
					item_2->setText(iedDesc);
					item_2->setIcon(QIcon("./images/shanchu.png"));
					gooseRecvListWidget->setItem(row,3,item_2);
				}
				else if(IEDInfo.IEDDesc.state==DIFFSTATE_NORMAL)
				{
					QString iedDesc;
					if(IEDInfo.ID.isEmpty())
						iedDesc=IEDInfo.IEDDesc.baseAttributeName;
					else 
					{
						str=IEDInfo.ID.split(".");
						iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;

					}
					item_2->setText(iedDesc);
					gooseRecvListWidget->setItem(row,3,item_2);
				} 


				QTableWidgetItem *item_3=new QTableWidgetItem;   //外部信号描述;
				SignalStru signalInfo;
				diifimpl.GetInfoOfOutSignal(ExtRefs.at(i).srcSignalID,&signalInfo);

				if(signalInfo.Desc.state==DIFFSTATE_EDITED)
				{
					item_3->setText(signalInfo.Desc.baseAttributeName);
					item_3->setToolTip(signalInfo.Desc.otherAttributeName);
					item_3->setIcon(QIcon("./images/bianji.png"));
					gooseRecvListWidget->setItem(row,4,item_3);

				}else if(signalInfo.Desc.state==DIFFSTATE_ADDED)
				{

					item_3->setText(signalInfo.Desc.otherAttributeName);
					item_3->setIcon(QIcon("./images/zengjia.png"));
					gooseRecvListWidget->setItem(row,4,item_3);		
				}
				else if(signalInfo.Desc.state==DIFFSTATE_REMOVED) 
				{
					item_3->setText(signalInfo.Desc.baseAttributeName);
					item_3->setIcon(QIcon("./images/shanchu.png"));
					gooseRecvListWidget->setItem(row,4,item_3);
				}
				else if(signalInfo.Desc.state==DIFFSTATE_NORMAL)
				{
					item_3->setText(signalInfo.Desc.baseAttributeName);
					gooseRecvListWidget->setItem(row,4,item_3);

				} 

				diifimpl.GetCtrlBlockOfOutIEDByOutsignal(ExtRefs.at(i).srcSignalID, &CBKey_1);

				QString ctrName="";

				if(CBKey_1.AppID.state==DIFFSTATE_EDITED)   // 虚端子列表和gs列表数组;
					ctrName=CBKey_1.AppID.baseAttributeName;
				else if(CBKey_1.AppID.state==DIFFSTATE_ADDED)
					ctrName=CBKey_1.AppID.otherAttributeName;		
				else if(CBKey_1.AppID.state==DIFFSTATE_REMOVED)
					ctrName=CBKey_1.AppID.baseAttributeName;	
				else if(CBKey_1.AppID.state==DIFFSTATE_NORMAL)
					ctrName=CBKey_1.AppID.baseAttributeName;

				QHash <QString,QString> m_CtlIed;
				m_CtlIed.insert(ctrName,str.at(1));
				mCtrlList.insert(row,m_CtlIed); 
				qDebug()<<i<<ctrName;
				if(!ctrName.isEmpty())    //fill 
				{
					if(!crlAppIed.contains(ctrName))
					{
						gooseExternalCtllListWidget->insertRow(Excellrow);
						fillGooseExternalCtllListWidget(CBKey_1,Excellrow,ExtRefs,i);
						crlAppIed.insert(ctrName,str.at(1));
						Excellrow++;
					}else 
					{
						QList <QString> ctlkey=crlAppIed.keys();
						for(int y=0;y<ctlkey.count();y++)
						{
							if(ctlkey.at(y)==ctrName)
							{
								if(str.at(1)!=crlAppIed.value(ctlkey.at(y)))
								{
									gooseExternalCtllListWidget->insertRow(Excellrow);
									fillGooseExternalCtllListWidget(CBKey_1,Excellrow,ExtRefs,i);
									crlAppIed.insert(ctrName,str.at(1));
									Excellrow++;
									break;
								}
							}
						}	
					}

				}
				row++;
			}	   
		}
		if(m_count==0)
			emit enbleTanFouce(false);
	}

	gooseRecvListWidget->resizeColumnsToContents();
	gooseRecvListWidget->resizeRowsToContents();

	gooseExternalCtllListWidget->resizeColumnsToContents();
	gooseExternalCtllListWidget->resizeRowsToContents();

}

void GooseReceiveWidget::GetSignalList(QVector<QString> inSignalKey,int i)
{
	gooseRecvListWidget->clear();
	gooseRecvListWidget->setColumnCount(gooseRecvHead.count());
	gooseRecvListWidget->setHorizontalHeaderLabels(gooseRecvHead);
	gooseRecvListWidget->setRowCount(0);
	if(inSignalKey.size()==0)
		return;
	if(i==1)
	{  
		//f//illGooseExternalCtllListWidget(inSignalKey,i);
	}
	else
	{
		//  fiilReceiveTerminal(inSignalKey);                          
		//  fillGooseExternalCtllListWidget(inSignalKey,i);  // 外部控制块列表
	}

}
void GooseReceiveWidget::fiilReceiveTerminal(QVector<QString> inSignalKey)
{
	CTableSCLModelImpl diifimpl;
	int row=0;
	for(int i=0;i<inSignalKey.count();i++)
	{
		SignalStru signalInfo;
		diifimpl.GetInfoOfInSignal(inSignalKey.at(i),&signalInfo);
		if(signalInfo.Desc.baseAttributeName!="")
		{
			gooseRecvListWidget->insertRow(row);
			gooseRecvListWidget->setItem(row,1,new QTableWidgetItem(signalInfo.Desc.baseAttributeName));
			row++;
		}

	}

}
void GooseReceiveWidget::fillGooseExternalCtllListWidget_1(GOOSECBStru goose,int Excellrow,QVector<ExtRefStru> ExtRefs,int i)
{
	CTableSCLModelImpl modeImpl;
	QTableWidgetItem *externalItem=new QTableWidgetItem;
	QTableWidgetItem *externalItem_1=new QTableWidgetItem;
	externalItem->setText(goose.AppID.baseAttributeName);
	gooseExternalCtllListWidget->setItem(Excellrow,0,externalItem);
	QVector<QString> inSignalKeys;

	//chenxin modify 2014.10.20 --->
	//modeImpl.GetInSignalsOfOutsignal(ExtRefs.at(i).InSigInfo.ID,&inSignalKeys);
	modeImpl.GetOutSignalsOfInsignal(ExtRefs.at(i).InSigInfo.ID,&inSignalKeys);
	if(inSignalKeys.size()==0)
	{
		return ;
	}
	//chenxin modify 2014.10.20 <---
	SCLNameTranslator translator;
	QString IEDKey_1 = translator.GetParentKey(inSignalKeys.at(0).toLocal8Bit().constData()).c_str();

	IEDStru  IEDInfo;
	modeImpl.GetIEDInfo(IEDKey_1,IEDInfo);
	QString iedDesc;
	QStringList str;
	if(IEDInfo.ID.isEmpty())
		iedDesc=IEDInfo.IEDDesc.baseAttributeName;
	else 
	{
		str=IEDInfo.ID.split(".");
		iedDesc="["+str.at(3)+"]"+IEDInfo.IEDDesc.baseAttributeName;
	}

	externalItem_1->setText(iedDesc);
	externalItem_1->setData(100,str.at(3));
	gooseExternalCtllListWidget->setItem(Excellrow,1,externalItem_1);
	gooseExternalCtllListWidget->setItem(Excellrow,2,new QTableWidgetItem(goose.CBName.baseAttributeName));
	gooseExternalCtllListWidget->setItem(Excellrow,3,new QTableWidgetItem(goose.DataSetName.baseAttributeName));
}

void GooseReceiveWidget::fillGooseExternalCtllListWidget(GOOSECBStru goose,int Excellrow,QVector<ExtRefStru> ExtRefs,int i)
{
	TableModelDiffImpl diifimpl;
	QTableWidgetItem *externalItem=new QTableWidgetItem;
	QTableWidgetItem *externalItem_1=new QTableWidgetItem;
	QTableWidgetItem *externalCtlNameItem=new QTableWidgetItem;
	QTableWidgetItem *externalDataItem=new QTableWidgetItem;
	if(goose.AppID.state==DIFFSTATE_EDITED)
	{
		externalItem->setText(goose.AppID.baseAttributeName);
		externalItem->setIcon(QIcon("./images/bianji.png"));
		gooseExternalCtllListWidget->setItem(Excellrow,0,externalItem);
		externalItem->setToolTip(goose.AppID.otherAttributeName);

	}
	else if(goose.AppID.state==DIFFSTATE_ADDED)
	{
		externalItem->setIcon(QIcon("./images/zengjia.png"));
		gooseExternalCtllListWidget->setItem(Excellrow,0,externalItem);
		externalItem->setText(goose.AppID.otherAttributeName);

	}
	else if(goose.AppID.state==DIFFSTATE_REMOVED)
	{
		externalItem->setText(goose.AppID.baseAttributeName);
		externalItem->setIcon(QIcon("./images/shanchu.png"));
		gooseExternalCtllListWidget->setItem(Excellrow,0,externalItem);	
	}
	else if(goose.AppID.state==DIFFSTATE_NORMAL)
	{
		externalItem->setText(goose.AppID.baseAttributeName);
		gooseExternalCtllListWidget->setItem(Excellrow,0,externalItem);

	} 

	SCLNameTranslator translator;
	QString IEDKey_1 = QString::fromStdString(translator.GetIEDKeyFromSCDDiffResult(ExtRefs.at(i).srcSignalID.toStdString()));
	IEDStru  IEDInfo;
	diifimpl.GetIEDInfo(IEDKey_1,IEDInfo);
	QStringList str;
	if(IEDInfo.IEDDesc.state==DIFFSTATE_ADDED)
	{
		QString iedDesc;
		if(IEDInfo.ID.isEmpty())
			iedDesc=IEDInfo.IEDDesc.otherAttributeName;
		else 
		{
			str=IEDInfo.ID.split(".");
			iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.otherAttributeName;

		}
		externalItem_1->setIcon(QIcon("./images/zengjia.png"));
		gooseExternalCtllListWidget->setItem(Excellrow,1,externalItem_1);
		externalItem_1->setText(iedDesc);
		externalItem_1->setData(100,str.at(1));
	}
	else if(IEDInfo.IEDDesc.state==DIFFSTATE_EDITED)
	{
		QString iedDesc;
		if(IEDInfo.ID.isEmpty())
			iedDesc=IEDInfo.IEDDesc.baseAttributeName;
		else 
		{
			str=IEDInfo.ID.split(".");
			iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;

		}
		externalItem_1->setText(iedDesc);
		externalItem_1->setIcon(QIcon("./images/bianji.png"));
		gooseExternalCtllListWidget->setItem(Excellrow,1,externalItem_1);
		externalItem_1->setToolTip(IEDInfo.IEDDesc.otherAttributeName);
		externalItem_1->setData(100,str.at(1));
	}
	else if(IEDInfo.IEDDesc.state==DIFFSTATE_REMOVED)
	{
		QString iedDesc;
		if(IEDInfo.ID.isEmpty())
			iedDesc=IEDInfo.IEDDesc.baseAttributeName;
		else 
		{
			str=IEDInfo.ID.split(".");
			iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;

		}
		externalItem_1->setText(iedDesc);
		externalItem_1->setIcon(QIcon("./images/shanchu.png"));
		externalItem_1->setData(100,str.at(1));
		gooseExternalCtllListWidget->setItem(Excellrow,1,externalItem_1);	
	}
	else if(IEDInfo.IEDDesc.state==DIFFSTATE_NORMAL)
	{
		QString iedDesc;
		if(IEDInfo.ID.isEmpty())
			iedDesc=IEDInfo.IEDDesc.baseAttributeName;
		else 
		{
			str=IEDInfo.ID.split(".");
			iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;
		}
		externalItem_1->setData(100,str.at(1));
		externalItem_1->setText(iedDesc);
		gooseExternalCtllListWidget->setItem(Excellrow,1,externalItem_1);
	}

	if(goose.CBName.state==DIFFSTATE_EDITED)
	{
		externalCtlNameItem->setText(goose.CBName.baseAttributeName);
		externalCtlNameItem->setIcon(QIcon("./images/bianji.png"));
		externalCtlNameItem->setToolTip(goose.CBName.otherAttributeName);
		gooseExternalCtllListWidget->setItem(Excellrow,2,externalCtlNameItem);

	}else if(goose.CBName.state==DIFFSTATE_ADDED)
	{
		externalCtlNameItem->setIcon(QIcon("./images/zengjia.png"));
		gooseExternalCtllListWidget->setItem(Excellrow,2,externalCtlNameItem);
		externalCtlNameItem->setText(goose.CBName.otherAttributeName);

	}
	else if(goose.CBName.state==DIFFSTATE_REMOVED)
	{
		externalCtlNameItem->setText(goose.CBName.baseAttributeName);
		externalCtlNameItem->setIcon(QIcon("./images/shanchu.png"));
		gooseExternalCtllListWidget->setItem(Excellrow,2,externalCtlNameItem);	
	}
	else if(goose.CBName.state==DIFFSTATE_NORMAL)
	{
		externalCtlNameItem->setText(goose.CBName.baseAttributeName);
		gooseExternalCtllListWidget->setItem(Excellrow,2,externalCtlNameItem);

	} 

	if(goose.DataSetName.state==DIFFSTATE_EDITED)
	{
		externalDataItem->setText(goose.DataSetName.baseAttributeName);
		externalDataItem->setIcon(QIcon("./images/bianji.png"));
		gooseExternalCtllListWidget->setItem(Excellrow,3,externalDataItem);
		externalDataItem->setToolTip(goose.DataSetName.otherAttributeName);

	}else if(goose.DataSetName.state==DIFFSTATE_ADDED)
	{
		externalDataItem->setIcon(QIcon("./images/zengjia.png"));
		gooseExternalCtllListWidget->setItem(Excellrow,3,externalDataItem);
		externalDataItem->setText(goose.DataSetName.otherAttributeName);

	}
	else if(goose.DataSetName.state==DIFFSTATE_REMOVED)
	{
		externalDataItem->setText(goose.DataSetName.baseAttributeName);
		externalDataItem->setIcon(QIcon("./images/shanchu.png"));
		gooseExternalCtllListWidget->setItem(Excellrow,3,externalDataItem);	
	}
	else if(goose.DataSetName.state==DIFFSTATE_NORMAL)
	{
		externalDataItem->setText(goose.DataSetName.baseAttributeName);
		gooseExternalCtllListWidget->setItem(Excellrow,3,externalDataItem);

	} 

}

GooseReceiveWidget::~GooseReceiveWidget()
{

}

void GooseReceiveWidget::setReverse(QTableWidgetItem* a)
{
	QItemSelectionModel *sel=gooseRecvListWidget->selectionModel();
	sel->clear();
	QString ctrlAppId = gooseExternalCtllListWidget->item(a->row(),0)->text();
	QString IEDAppId  = gooseExternalCtllListWidget->item(a->row(),1)->data(100).toString();
	QList <int> mm_row;
	QList <int> IndexKey=mCtrlList.keys();
	for(int gIndex=0;gIndex<IndexKey.count();gIndex++)
	{ 
		QHash <QString,QString> mList = mCtrlList.value(IndexKey.at(gIndex));
		if(mList.keys().at(0)==ctrlAppId&&mList.values().at(0)==IEDAppId)
			mm_row.append(gIndex);
	}
	recvListSelection(mm_row);

	if(mm_row.count()==1)
	{
		QModelIndex index=gooseRecvListWidget->model()->index(mm_row.at(0),0,gooseRecvListWidget->rootIndex());
		QModelIndex index_1=gooseRecvListWidget->model()->index(mm_row.at(0),3,gooseRecvListWidget->rootIndex());
		QItemSelectionModel *sel=gooseRecvListWidget->selectionModel();
		QItemSelection seltion(index,index_1);
		sel->select(seltion,QItemSelectionModel::SelectCurrent| QItemSelectionModel::Rows);
	}
}

void  GooseReceiveWidget::recvListSelection(QList <int> mm_row)
{
	QItemSelectionModel *sel=gooseRecvListWidget->selectionModel();
	int mRowStart=0;
	int mRowEnd=0;
	for(int mRowCount=0;mRowCount<mm_row.count();mRowCount++)  // 顶层想关联的行变色
	{
		if(mm_row.count()==1)
		{
			return;
		}
		else if(mm_row.count()==2)
		{
			if(mm_row.at(1)-mm_row.at(0)!=1)
			{
				mRowStart=mm_row.at(0); 
				mRowEnd=mm_row.at(1);
				QModelIndex index=gooseRecvListWidget->model()->index(mRowStart,0,gooseRecvListWidget->rootIndex());
				QModelIndex index_1=gooseRecvListWidget->model()->index(mRowStart,3,gooseRecvListWidget->rootIndex());

				QModelIndex index_2=gooseRecvListWidget->model()->index(mRowEnd,0,gooseRecvListWidget->rootIndex());
				QModelIndex index_3=gooseRecvListWidget->model()->index(mRowEnd,3,gooseRecvListWidget->rootIndex());
				QItemSelection seltion(index,index_1);
				QItemSelection seltion_1(index_2,index_3);
				sel->select(seltion,QItemSelectionModel::Select | QItemSelectionModel::Rows); 
				sel->select(seltion_1,QItemSelectionModel::Select | QItemSelectionModel::Rows); 
			}else
			{
				mRowStart=mm_row.at(0);
				mRowEnd=mm_row.at(1);
				QModelIndex index=gooseRecvListWidget->model()->index(mRowStart,0,gooseRecvListWidget->rootIndex());
				QModelIndex index_1=gooseRecvListWidget->model()->index(mRowEnd,3,gooseRecvListWidget->rootIndex());
				QItemSelection seltion(index,index_1);
				sel->select(seltion,QItemSelectionModel::Select | QItemSelectionModel::Rows); 
			}
			return;
		}
		else {
			if(mRowCount==0)
			{
				mRowStart=mm_row.at(mRowCount);
			}else
			{
				if(mm_row.at(mRowCount)-mm_row.at(mRowCount-1)!=1)
				{
					mRowEnd=mm_row.at(mRowCount-1);
					QModelIndex index=gooseRecvListWidget->model()->index(mRowStart,0,gooseRecvListWidget->rootIndex());
					QModelIndex index_1=gooseRecvListWidget->model()->index(mRowEnd,3,gooseRecvListWidget->rootIndex());
					QItemSelection seltion(index,index_1);
					sel->select(seltion,QItemSelectionModel::Select | QItemSelectionModel::Rows); 
					if(mRowCount==mm_row.count()-1)  // 如果生下的一行为单行 并且是最后一行
					{
						QModelIndex index=gooseRecvListWidget->model()->index(mm_row.at(mRowCount),0,gooseRecvListWidget->rootIndex());
						QModelIndex index_1=gooseRecvListWidget->model()->index(mm_row.at(mRowCount),3,gooseRecvListWidget->rootIndex());
						QItemSelection seltion(index,index_1);
						sel->select(seltion,QItemSelectionModel::Select | QItemSelectionModel::Rows);
					}
					mRowStart=mm_row.at(mRowCount);

				}else
				{
					if(mRowCount==mm_row.count()-1)
					{
						mRowEnd=mm_row.at(mRowCount);
						QModelIndex index=gooseRecvListWidget->model()->index(mRowStart,0,gooseRecvListWidget->rootIndex());
						QModelIndex index_1=gooseRecvListWidget->model()->index(mRowEnd,3,gooseRecvListWidget->rootIndex());
						QItemSelection seltion(index,index_1);
						sel->select(seltion,QItemSelectionModel::Select | QItemSelectionModel::Rows); 
						mRowStart=mm_row.at(mRowCount);
					}

				}
			}
		}
	}
}
void GooseReceiveWidget::setSelecttion(QTableWidgetItem *a)
{
	QString indexCtlStr=mCtrlList.value(a->row()).keys().at(0);      //gooseExternal 列表对应的ctl行选中状态
	QString indexIedStr=mCtrlList.value(a->row()).value(indexCtlStr);
	for(int i=0;i<gooseExternalCtllListWidget->rowCount();i++)  //gooseExternalCtllListWidget 关联
	{
		QTableWidgetItem *item=gooseExternalCtllListWidget->item(i,0);

		if(item->text()==indexCtlStr&&indexIedStr==gooseExternalCtllListWidget->item(i,1)->data(100))
		{
			QModelIndex index=gooseExternalCtllListWidget->model()->index(i,0,gooseExternalCtllListWidget->rootIndex());
			QModelIndex index_1=gooseExternalCtllListWidget->model()->index(i,3,gooseExternalCtllListWidget->rootIndex());
			QItemSelectionModel *sel=gooseExternalCtllListWidget->selectionModel();
			QItemSelection seltion(index,index_1);
			sel->select(seltion,QItemSelectionModel::SelectCurrent);
		}
	}

	QList<QString>CtrlKey=SiganlToCtrl.keys(); //得到信号对应的控制块列表

	QString ctrlAppId=mCtrlList.value(a->row()).keys().at(0);
	QString IEDAppId=mCtrlList.value(a->row()).values().at(0);
	QList <int> mm_row;
	QList <int> IndexKey=mCtrlList.keys();
	for(int gIndex=0;gIndex<IndexKey.count();gIndex++)
	{ 
		QHash <QString,QString> mList = mCtrlList.value(IndexKey.at(gIndex));
		if(mList.keys().at(0)==ctrlAppId&&mList.values().at(0)==IEDAppId)
			mm_row.append(gIndex);
	}

	recvListSelection(mm_row);
}
//********************************************************************************************
//sv receive 
//********************************************************************************************

SvReceiveWidget::SvReceiveWidget(QWidget *parent)
	: QWidget(parent)
{
	QSplitter *spliterMain=new QSplitter(Qt::Vertical,this);

	svRecvListWidget=new QTableWidget(spliterMain);
	MyitemDelegate *item =new MyitemDelegate(this);
	svRecvListWidget->setItemDelegate(item);
	svRecvHead<<"内部短地址"<<"内部信号描述"<<"物理端口"<<"外部IED"<<"外部信号描述";
	svRecvListWidget->setColumnCount(svRecvHead.count());
	svRecvListWidget->setHorizontalHeaderLabels(svRecvHead);
	svRecvListWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行;
	svRecvListWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); 
	svRecvListWidget->setStyleSheet("selection-background-color:lightblue;"); 

	QSplitter *splitercl=new QSplitter(Qt::Horizontal,spliterMain);
	QVBoxLayout *vbox=new QVBoxLayout(this);
	connect(svRecvListWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(setSelection(QTableWidgetItem*)));
	svExternalCtllListWidget=new QTableWidget(splitercl);
	svExternalCtllListWidget->setItemDelegate(item);
	svExternalCtlHead<<"APPID"<<"所在IED"<<"控制块名称"<<"数据集";
	svExternalCtllListWidget->setColumnCount(svExternalCtlHead.count());
	svExternalCtllListWidget->setHorizontalHeaderLabels(svExternalCtlHead);
	svExternalCtllListWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行;
	svExternalCtllListWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); 
	svExternalCtllListWidget->setStyleSheet("selection-background-color:lightblue;");  
	connect(svExternalCtllListWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(setReverse(QTableWidgetItem*)));
	vbox->addWidget(spliterMain);
}
SvReceiveWidget::~SvReceiveWidget()
{


}
void SvReceiveWidget::GetExtRefStruInfo(QVector<ExtRefStru> ExtRefs,int type)
{
	svRecvListWidget->clear();
	svRecvListWidget->setColumnCount(svRecvHead.count());
	svRecvListWidget->setHorizontalHeaderLabels(svRecvHead);
	svRecvListWidget->setRowCount(0);
	mCtrlList.clear();
	crlAppIed.clear();

	svExternalCtllListWidget->clear();
	ctrList.clear();
	svExternalCtllListWidget->setColumnCount(svExternalCtlHead.count());
	svExternalCtllListWidget->setRowCount(0);
	svExternalCtllListWidget->setHorizontalHeaderLabels(svExternalCtlHead);

	int m_count=0;
	if(type==0)
	{
		CTableSCLModelImpl modeImpl;
		SVCBStru CBKey;
		SVCBStru CBKey_1;
		int row=0;
		int Excellrow=0;
		for(int i=0;i<ExtRefs.count();i++)
		{ 

			if(ExtRefs.at(i).type==typeSV_)
			{
				m_count++;
				emit enbleTanFouce(true);
				svRecvListWidget->insertRow(row);

				QTableWidgetItem *item=new QTableWidgetItem;
				item->setText(ExtRefs.at(i).IntAddr.baseAttributeName);                      // 4 代表编辑;
				svRecvListWidget->setItem(row,0,item);

				QTableWidgetItem *item_1=new QTableWidgetItem;
				item_1->setText(ExtRefs.at(i).desc.baseAttributeName);
				svRecvListWidget->setItem(row,1,item_1);

				QTableWidgetItem *ptItem = new QTableWidgetItem;
				ptItem->setText(ExtRefs.at(i).PortName.baseAttributeName);
				svRecvListWidget->setItem(row,2,ptItem);

				/*modeImpl.GetCtrlBlockOfOutIEDByInsignal(ExtRefs.at(i).InSigInfo.ID, &CBKey);
				if(!CBKey.ID.isEmpty())
				{
				QStringList str=CBKey.ID.split(".");
				item_2->setText(str.at(3));
				}*/
				QTableWidgetItem *item_2=new QTableWidgetItem; 
				SCLNameTranslator translator;
				QString IEDKey_1 = translator.GetParentKey(ExtRefs.at(i).srcSignalID.toLocal8Bit().constData()).c_str();
				IEDStru  IEDInfo;
				modeImpl.GetIEDInfo(IEDKey_1,IEDInfo);
				QString iedDesc;
				QStringList str=QStringList()<<"NULL"<<"NULL"<<"NULL"<<"NULL";
				if(IEDInfo.ID.isEmpty())
					iedDesc=IEDInfo.IEDDesc.baseAttributeName;
				else 
				{
					str=IEDInfo.ID.split(".");
					iedDesc="["+str.at(3)+"]"+IEDInfo.IEDDesc.baseAttributeName;

				}
				item_2->setText(iedDesc);
				svRecvListWidget->setItem(row,3,item_2);

				QTableWidgetItem *item_3=new QTableWidgetItem;
				SignalStru signalInfo;
				modeImpl.GetInfoOfOutSignal(ExtRefs.at(i).srcSignalID,&signalInfo);
				item_3->setText(signalInfo.Desc.baseAttributeName);
				svRecvListWidget->setItem(row,4,item_3);

				modeImpl.GetCtrlBlockOfOutIEDByInsignal(ExtRefs.at(i).InSigInfo.ID, &CBKey_1);
				QHash <QString,QString> m_CtlIed;
				m_CtlIed.insert(CBKey_1.AppID.baseAttributeName,str.at(3));
				mCtrlList.insert(row,m_CtlIed); 

				CrtIndex.insert(row,CBKey_1.AppID.baseAttributeName);

				if(!crlAppIed.contains(CBKey_1.AppID.baseAttributeName))
				{
					svExternalCtllListWidget->insertRow(Excellrow);
					fillGooseExternalCtllListWidget_1(CBKey_1,Excellrow,ExtRefs,i);
					crlAppIed.insert(CBKey_1.AppID.baseAttributeName,str.at(3));
					Excellrow++;
				}else 
				{
					QList <QString> ctlkey=crlAppIed.keys();
					for(int y=0;y<ctlkey.count();y++)
					{
						if(ctlkey.at(y)==CBKey_1.AppID.baseAttributeName)
						{
							if(str.at(3)!=crlAppIed.value(ctlkey.at(y)))
							{
								svExternalCtllListWidget->insertRow(Excellrow);
								fillGooseExternalCtllListWidget_1(CBKey_1,Excellrow,ExtRefs,i);
								crlAppIed.insert(CBKey_1.AppID.baseAttributeName,str.at(3));
								Excellrow++;
								break;
							}
						}
					}	
				}
				row++;
			}	   
		}
		if(m_count==0)
			emit enbleTanFouce(false);
	}
	else
	{
		TableModelDiffImpl diifimpl;
		int row=0;
		int Excellrow=0;
		for(int i=0;i<ExtRefs.count();i++)
		{ 
			SVCBStru CBKey;
			SVCBStru CBKey_1;
			if(ExtRefs.at(i).type==typeSV_)
			{
				m_count++;
				emit enbleTanFouce(true);
				svRecvListWidget->insertRow(row);                          //

				QTableWidgetItem *ptItem = new QTableWidgetItem;
				if (ExtRefs.at(i).PortName.state == DIFFSTATE_EDITED)
				{
					ptItem->setText(ExtRefs.at(i).PortName.baseAttributeName);
					ptItem->setToolTip(ExtRefs.at(i).PortName.otherAttributeName);
					ptItem->setIcon(QIcon("./images/bianji.png"));
					svRecvListWidget->setItem(row,2,ptItem);
				}
				else if (ExtRefs.at(i).PortName.state == DIFFSTATE_ADDED)
				{
					ptItem->setText(ExtRefs.at(i).PortName.otherAttributeName);
					ptItem->setIcon(QIcon("./images/zengjia.png"));
					svRecvListWidget->setItem(row,2,ptItem);
				}
				else if (ExtRefs.at(i).PortName.state == DIFFSTATE_REMOVED)
				{
					ptItem->setText(ExtRefs.at(i).PortName.baseAttributeName);
					ptItem->setIcon(QIcon("./images/shanchu.png"));
					svRecvListWidget->setItem(row,2,ptItem);
				}
				else if (ExtRefs.at(i).PortName.state == DIFFSTATE_NORMAL)
				{
					ptItem->setText(ExtRefs.at(i).PortName.baseAttributeName);
					svRecvListWidget->setItem(row,2,ptItem);
				}

				QTableWidgetItem *item=new QTableWidgetItem;
				if(ExtRefs.at(i).IntAddr.state==DIFFSTATE_EDITED)
				{  
					item->setText(ExtRefs.at(i).IntAddr.baseAttributeName);                      // 4 代表编辑 
					item->setToolTip(ExtRefs.at(i).IntAddr.otherAttributeName);
					item->setIcon(QIcon("./images/bianji.png"));
					svRecvListWidget->setItem(row,0,item);
				}else if(ExtRefs.at(i).IntAddr.state==DIFFSTATE_ADDED)
				{
					item->setText(ExtRefs.at(i).IntAddr.otherAttributeName);                          // 4 代表编辑
					item->setIcon(QIcon("./images/zengjia.png"));
					svRecvListWidget->setItem(row,0,item);

				}
				else if(ExtRefs.at(i).IntAddr.state==DIFFSTATE_REMOVED)
				{
					item->setText(ExtRefs.at(i).IntAddr.baseAttributeName);                       // 4 代表编辑
					item->setIcon(QIcon("./images/shanchu.png"));
					svRecvListWidget->setItem(row,0,item);

				}
				else if(ExtRefs.at(i).IntAddr.state==DIFFSTATE_NORMAL)
				{
					item->setText(ExtRefs.at(i).IntAddr.baseAttributeName);   
					//item->setText(ExtRefs.at(i).IntAddr.baseAttributeName);
					svRecvListWidget->setItem(row,0,item);
				} 


				QTableWidgetItem *item_1=new QTableWidgetItem;
				if(ExtRefs.at(i).desc.state==DIFFSTATE_EDITED)
				{
					item_1->setText(ExtRefs.at(i).desc.baseAttributeName);
					item_1->setToolTip(ExtRefs.at(i).desc.otherAttributeName);
					item_1->setIcon(QIcon("./images/bianji.png"));
					svRecvListWidget->setItem(row,1,item_1);

				}else if(ExtRefs.at(i).desc.state==DIFFSTATE_ADDED)
				{
					item_1->setText(ExtRefs.at(i).desc.otherAttributeName);
					item_1->setIcon(QIcon("./images/zengjia.png"));
					svRecvListWidget->setItem(row,1,item_1);		
				}
				else if(ExtRefs.at(i).desc.state==DIFFSTATE_REMOVED)
				{
					item_1->setText(ExtRefs.at(i).desc.baseAttributeName);                                   // 4 代表编辑
					item_1->setIcon(QIcon("./images/shanchu.png"));
					svRecvListWidget->setItem(row,1,item_1);
				}
				else if(ExtRefs.at(i).desc.state==DIFFSTATE_NORMAL)
				{
					item_1->setText(ExtRefs.at(i).desc.baseAttributeName);
					svRecvListWidget->setItem(row,1,item_1);
				} 

				QTableWidgetItem *item_2=new QTableWidgetItem;
				SCLNameTranslator translator;
				QString IEDKey_1 = QString::fromStdString(translator.GetIEDKeyFromSCDDiffResult(ExtRefs.at(i).srcSignalID.toStdString()));
				IEDStru  IEDInfo;
				diifimpl.GetIEDInfo(IEDKey_1,IEDInfo);
				QStringList str=QStringList()<<"NULL"<<"NULL"<<"NULL"<<"NULL";
				if(IEDInfo.IEDDesc.state==DIFFSTATE_EDITED)
				{
					QString iedDesc;
					if(IEDInfo.ID.isEmpty())
						iedDesc=IEDInfo.IEDDesc.baseAttributeName;
					else 
					{
						str=IEDInfo.ID.split(".");
						iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;

					}
					item_2->setText(iedDesc);
					item_2->setToolTip(IEDInfo.IEDDesc.otherAttributeName);
					item_2->setIcon(QIcon("./images/bianji.png"));
					svRecvListWidget->setItem(row,3,item_2);

				}else if(IEDInfo.IEDDesc.state==DIFFSTATE_ADDED)
				{
					QString iedDesc;
					if(IEDInfo.ID.isEmpty())
						iedDesc=IEDInfo.IEDDesc.otherAttributeName;
					else 
					{
						str=IEDInfo.ID.split(".");
						iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.otherAttributeName;

					}
					item_2->setText(iedDesc);
					item_2->setIcon(QIcon("./images/zengjia.png"));
					svRecvListWidget->setItem(row,3,item_2);		
				}
				else if(IEDInfo.IEDDesc.state==DIFFSTATE_REMOVED) 
				{
					QString iedDesc;
					if(IEDInfo.ID.isEmpty())
						iedDesc=IEDInfo.IEDDesc.baseAttributeName;
					else 
					{
						str=IEDInfo.ID.split(".");
						iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;

					}
					item_2->setText(iedDesc);
					item_2->setIcon(QIcon("./images/shanchu.png"));
					svRecvListWidget->setItem(row,3,item_2);
				}
				else if(IEDInfo.IEDDesc.state==DIFFSTATE_NORMAL)
				{
					QString iedDesc;
					if(IEDInfo.ID.isEmpty())
						iedDesc=IEDInfo.IEDDesc.baseAttributeName;
					else 
					{
						str=IEDInfo.ID.split(".");
						iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;

					}
					item_2->setText(iedDesc);
					svRecvListWidget->setItem(row,3,item_2);
				}	


				QTableWidgetItem *item_3=new QTableWidgetItem;
				SignalStru signalInfo;
				diifimpl.GetInfoOfOutSignal(ExtRefs.at(i).srcSignalID,&signalInfo);

				if(signalInfo.Desc.state==DIFFSTATE_EDITED)
				{
					item_3->setText(signalInfo.Desc.baseAttributeName);
					item_3->setToolTip(signalInfo.Desc.otherAttributeName);
					item_3->setIcon(QIcon("./images/bianji.png"));
					svRecvListWidget->setItem(row,4,item_3);

				}else if(signalInfo.Desc.state==DIFFSTATE_ADDED)
				{

					item_3->setText(signalInfo.Desc.otherAttributeName);
					item_3->setIcon(QIcon("./images/zengjia.png"));
					svRecvListWidget->setItem(row,4,item_3);		
				}
				else if(signalInfo.Desc.state==DIFFSTATE_REMOVED)
				{
					item_3->setText(signalInfo.Desc.baseAttributeName);
					item_3->setIcon(QIcon("./images/shanchu.png"));
					svRecvListWidget->setItem(row,4,item_3);
				}
				else if(signalInfo.Desc.state==DIFFSTATE_NORMAL)
				{
					item_3->setText(signalInfo.Desc.baseAttributeName);
					svRecvListWidget->setItem(row,4,item_3);
				} 

				diifimpl.GetCtrlBlockOfOutIEDByOutsignal(ExtRefs.at(i).srcSignalID, &CBKey_1);
				QString ctrName="";

				if(CBKey_1.AppID.state==DIFFSTATE_EDITED)   // 虚端子列表和gs列表数组;
					ctrName=CBKey_1.AppID.baseAttributeName;
				else if(CBKey_1.AppID.state==DIFFSTATE_ADDED)
					ctrName=CBKey_1.AppID.otherAttributeName;		
				else if(CBKey_1.AppID.state==DIFFSTATE_REMOVED)
					ctrName=CBKey_1.AppID.baseAttributeName;	
				else if(CBKey_1.AppID.state==DIFFSTATE_NORMAL)
					ctrName=CBKey_1.AppID.baseAttributeName;

				CrtIndex.insert(row,ctrName);
				QHash <QString,QString> m_CtlIed;
				m_CtlIed.insert(ctrName,str.at(1));
				mCtrlList.insert(row,m_CtlIed); 


				if(!ctrName.isEmpty())    //fill 
				{
					if(!crlAppIed.contains(ctrName))
					{
						svExternalCtllListWidget->insertRow(Excellrow);
						fillGooseExternalCtllListWidget(CBKey_1,Excellrow,ExtRefs,i);
						crlAppIed.insert(ctrName,str.at(1));
						Excellrow++;
					}else 
					{
						QList <QString> ctlkey=crlAppIed.keys();
						for(int y=0;y<ctlkey.count();y++)
						{
							if(ctlkey.at(y)==ctrName)
							{
								if(str.at(1)!=crlAppIed.value(ctlkey.at(y)))
								{
									svExternalCtllListWidget->insertRow(Excellrow);
									fillGooseExternalCtllListWidget(CBKey_1,Excellrow,ExtRefs,i);
									crlAppIed.insert(ctrName,str.at(1));
									Excellrow++;
									break;
								}
							}
						}	
					}

				}
				row++;
			}	   
		}
		if(m_count==0)
			emit enbleTanFouce(false);
	}
	svExternalCtllListWidget->resizeColumnsToContents();
	svExternalCtllListWidget->resizeRowsToContents();

	svRecvListWidget->resizeColumnsToContents();
	svRecvListWidget->resizeRowsToContents();
}
void SvReceiveWidget::fillGooseExternalCtllListWidget_1(SVCBStru goose,int Excellrow,QVector<ExtRefStru> ExtRefs,int i)
{
	QTableWidgetItem *externalItem=new QTableWidgetItem;
	QTableWidgetItem *externalItem_1=new QTableWidgetItem;
	CTableSCLModelImpl modeImpl;
	QVector<QString> inSignalKeys;
	//chenxin modify 2014.10.21 --->
	//modeImpl.GetInSignalsOfOutsignal(ExtRefs.at(i).InSigInfo.ID,&inSignalKeys);
	modeImpl.GetOutSignalsOfInsignal(ExtRefs.at(i).InSigInfo.ID,&inSignalKeys);
	//chenxin modify 2014.10.21 <---

	if(inSignalKeys.size()==0)
	{
		return ;
	}
	SCLNameTranslator translator;
	QString IEDKey_1 = translator.GetParentKey(inSignalKeys.at(0).toLocal8Bit().constData()).c_str();
	IEDStru  IEDInfo;
	QStringList str;
	modeImpl.GetIEDInfo(IEDKey_1,IEDInfo);
	QString iedDesc;
	if(IEDInfo.ID.isEmpty())
		iedDesc=IEDInfo.IEDDesc.baseAttributeName;
	else 
	{
		str=IEDInfo.ID.split(".");
		iedDesc="["+str.at(3)+"]"+IEDInfo.IEDDesc.baseAttributeName;

	}
	externalItem_1->setText(iedDesc);
	externalItem_1->setData(100,str.at(3));
	externalItem->setText(goose.AppID.baseAttributeName);
	svExternalCtllListWidget->setItem(Excellrow,0,externalItem);
	svExternalCtllListWidget->setItem(Excellrow,1,externalItem_1);
	svExternalCtllListWidget->setItem(Excellrow,2,new QTableWidgetItem(goose.CBName.baseAttributeName));
	svExternalCtllListWidget->setItem(Excellrow,3,new QTableWidgetItem(goose.DataSetName.baseAttributeName));
}
void SvReceiveWidget::fillGooseExternalCtllListWidget(SVCBStru goose,int Excellrow,QVector<ExtRefStru> ExtRefs,int i)
{
	TableModelDiffImpl diifimpl;
	QTableWidgetItem *externalItem=new QTableWidgetItem;
	QTableWidgetItem *externalItem_1=new QTableWidgetItem;
	QTableWidgetItem *externalCtlNameItem=new QTableWidgetItem;
	QTableWidgetItem *externalDataItem=new QTableWidgetItem;

	if(goose.AppID.state==DIFFSTATE_EDITED)
	{
		externalItem->setText(goose.AppID.baseAttributeName);
		externalItem->setIcon(QIcon("./images/bianji.png"));
		svExternalCtllListWidget->setItem(Excellrow,0,externalItem);
		externalItem->setToolTip(goose.AppID.otherAttributeName);

	}else if(goose.AppID.state==DIFFSTATE_ADDED)
	{
		externalItem->setIcon(QIcon("./images/zengjia.png"));
		svExternalCtllListWidget->setItem(Excellrow,0,externalItem);
		externalItem->setText(goose.AppID.otherAttributeName);

	}
	else if(goose.AppID.state==DIFFSTATE_REMOVED)
	{
		externalItem->setText(goose.AppID.baseAttributeName);
		externalItem->setIcon(QIcon("./images/shanchu.png"));
		svExternalCtllListWidget->setItem(Excellrow,0,externalItem);	
	}
	else if(goose.AppID.state==DIFFSTATE_NORMAL)
	{
		externalItem->setText(goose.AppID.baseAttributeName);
		svExternalCtllListWidget->setItem(Excellrow,0,externalItem);
	} 

	SCLNameTranslator translator;
	QString IEDKey_1 = QString::fromStdString(translator.GetIEDKeyFromSCDDiffResult(ExtRefs.at(i).srcSignalID.toStdString()));
	IEDStru  IEDInfo;
	diifimpl.GetIEDInfo(IEDKey_1,IEDInfo);
	QStringList str;
	if(IEDInfo.IEDDesc.state==DIFFSTATE_ADDED)
	{
		QString iedDesc;
		if(IEDInfo.ID.isEmpty())
		{
			iedDesc=IEDInfo.IEDDesc.otherAttributeName;
		}
		else 
		{
			str=IEDInfo.ID.split(".");
			iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.otherAttributeName;

		}
		externalItem_1->setIcon(QIcon("./images/zengjia.png"));
		svExternalCtllListWidget->setItem(Excellrow,1,externalItem_1);
		externalItem_1->setText(iedDesc);
		externalItem_1->setData(100,str.at(1));
	}
	else if(IEDInfo.IEDDesc.state==DIFFSTATE_EDITED)
	{
		QString iedDesc;
		if(IEDInfo.ID.isEmpty())
			iedDesc=IEDInfo.IEDDesc.baseAttributeName;
		else 
		{
			str=IEDInfo.ID.split(".");
			iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;

		}
		externalItem_1->setText(iedDesc);
		externalItem_1->setData(100,str.at(1));
		externalItem_1->setIcon(QIcon("./images/bianji.png"));
		svExternalCtllListWidget->setItem(Excellrow,1,externalItem_1);
		externalItem_1->setToolTip(IEDInfo.IEDDesc.otherAttributeName);
	}
	else if(IEDInfo.IEDDesc.state==DIFFSTATE_REMOVED)
	{
		QString iedDesc;
		if(IEDInfo.ID.isEmpty())
			iedDesc=IEDInfo.IEDDesc.baseAttributeName;
		else 
		{
			str=IEDInfo.ID.split(".");
			iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;

		}
		externalItem_1->setText(iedDesc);
		externalItem_1->setData(100,str.at(1));
		externalItem_1->setIcon(QIcon("./images/shanchu.png"));
		svExternalCtllListWidget->setItem(Excellrow,1,externalItem_1);	
	}
	else if(IEDInfo.IEDDesc.state==DIFFSTATE_NORMAL)
	{
		QString iedDesc;
		if(IEDInfo.ID.isEmpty())
			iedDesc=IEDInfo.IEDDesc.baseAttributeName;
		else 
		{
			str=IEDInfo.ID.split(".");
			iedDesc="["+str.at(1)+"]"+IEDInfo.IEDDesc.baseAttributeName;

		}
		externalItem_1->setText(iedDesc);
		externalItem_1->setData(100,str.at(1));
		svExternalCtllListWidget->setItem(Excellrow,1,externalItem_1);
	}

	if(goose.CBName.state==DIFFSTATE_EDITED)
	{
		externalCtlNameItem->setText(goose.CBName.baseAttributeName);
		externalCtlNameItem->setIcon(QIcon("./images/bianji.png"));
		externalCtlNameItem->setToolTip(goose.CBName.otherAttributeName);
		svExternalCtllListWidget->setItem(Excellrow,2,externalCtlNameItem);

	}
	else if(goose.CBName.state==DIFFSTATE_ADDED)
	{
		externalCtlNameItem->setIcon(QIcon("./images/zengjia.png"));
		svExternalCtllListWidget->setItem(Excellrow,2,externalCtlNameItem);
		externalCtlNameItem->setText(goose.CBName.otherAttributeName);

	}
	else if(goose.CBName.state==DIFFSTATE_REMOVED)
	{
		externalCtlNameItem->setText(goose.CBName.baseAttributeName);
		externalCtlNameItem->setIcon(QIcon("./images/shanchu.png"));
		svExternalCtllListWidget->setItem(Excellrow,2,externalCtlNameItem);	
	}
	else if(goose.CBName.state==DIFFSTATE_NORMAL)
	{
		externalCtlNameItem->setText(goose.CBName.baseAttributeName);
		svExternalCtllListWidget->setItem(Excellrow,2,externalCtlNameItem);

	} 

	if(goose.DataSetName.state==DIFFSTATE_EDITED)
	{
		externalDataItem->setText(goose.DataSetName.baseAttributeName);
		externalDataItem->setIcon(QIcon("./images/bianji.png"));
		svExternalCtllListWidget->setItem(Excellrow,3,externalDataItem);
		externalDataItem->setToolTip(goose.DataSetName.otherAttributeName);

	}else if(goose.DataSetName.state==DIFFSTATE_ADDED)
	{
		externalDataItem->setIcon(QIcon("./images/zengjia.png"));
		svExternalCtllListWidget->setItem(Excellrow,3,externalDataItem);
		externalDataItem->setText(goose.DataSetName.otherAttributeName);

	}
	else if(goose.DataSetName.state==DIFFSTATE_REMOVED)
	{
		externalDataItem->setText(goose.DataSetName.baseAttributeName);
		externalDataItem->setIcon(QIcon("./images/shanchu.png"));
		svExternalCtllListWidget->setItem(Excellrow,3,externalDataItem);	
	}
	else if(goose.DataSetName.state==DIFFSTATE_NORMAL)
	{
		externalDataItem->setText(goose.DataSetName.baseAttributeName);
		svExternalCtllListWidget->setItem(Excellrow,3,externalDataItem);
	} 
}
void SvReceiveWidget::clearData()
{

	svExternalCtllListWidget->clear();
	svExternalCtllListWidget->setColumnCount(svRecvHead.count());
	svExternalCtllListWidget->setHorizontalHeaderLabels(svRecvHead);
	svExternalCtllListWidget->setRowCount(0);

	svRecvListWidget->clear();
	svRecvListWidget->setColumnCount(svExternalCtlHead.count());
	svRecvListWidget->setHorizontalHeaderLabels(svExternalCtlHead);
	svRecvListWidget->setRowCount(0);
}

void SvReceiveWidget::setSelection(QTableWidgetItem* a)
{
	QString indexStr=mCtrlList.value(a->row()).keys().at(0);
	QString indexStrIed=mCtrlList.value(a->row()).value(indexStr);
	for(int i=0;i<svExternalCtllListWidget->rowCount();i++)
	{
		QTableWidgetItem *item=svExternalCtllListWidget->item(i,0);
		if(item->text()==indexStr&&svExternalCtllListWidget->item(i,1)->data(100)==indexStrIed)
		{
			QModelIndex index=svExternalCtllListWidget->model()->index(i,0,svExternalCtllListWidget->rootIndex());
			QModelIndex index_1=svExternalCtllListWidget->model()->index(i,3,svExternalCtllListWidget->rootIndex());
			QItemSelectionModel *sel=svExternalCtllListWidget->selectionModel();
			QItemSelection seltion(index,index_1);
			sel->select(seltion,QItemSelectionModel::SelectCurrent);
		}
	}

	QString ctrlAppId=mCtrlList.value(a->row()).keys().at(0);
	QString IEDAppId=mCtrlList.value(a->row()).values().at(0);
	QList <int> mm_row;
	QList <int> IndexKey=mCtrlList.keys();
	for(int gIndex=0;gIndex<IndexKey.count();gIndex++)
	{ 
		QHash <QString,QString> mList = mCtrlList.value(IndexKey.at(gIndex));

		if(mList.keys().at(0)==ctrlAppId&&mList.values().at(0)==IEDAppId)
			mm_row.append(gIndex);
	}

	recvListSelection(mm_row);
}

void SvReceiveWidget::setReverse(QTableWidgetItem* a)
{
	QItemSelectionModel *sel=svRecvListWidget->selectionModel();
	sel->clear();
	QString ctrlAppId = svExternalCtllListWidget->item(a->row(),0)->text();
	QString IEDAppId  = svExternalCtllListWidget->item(a->row(),1)->data(100).toString();
	QList <int> mm_row;
	QList <int> IndexKey=mCtrlList.keys();
	for(int gIndex=0;gIndex<IndexKey.count();gIndex++)
	{ 
		QHash <QString,QString> mList = mCtrlList.value(IndexKey.at(gIndex));
		if(mList.keys().at(0)==ctrlAppId&&mList.values().at(0)==IEDAppId)
			mm_row.append(gIndex);
	}

	if(mm_row.count()==1)
	{
		QModelIndex index=svRecvListWidget->model()->index(mm_row.at(0),0,svRecvListWidget->rootIndex());
		QModelIndex index_1=svRecvListWidget->model()->index(mm_row.at(0),3,svRecvListWidget->rootIndex());
		QItemSelectionModel *sel=svRecvListWidget->selectionModel();
		QItemSelection seltion(index,index_1);
		sel->select(seltion,QItemSelectionModel::SelectCurrent| QItemSelectionModel::Rows);

	}

	recvListSelection(mm_row);

}
void SvReceiveWidget::recvListSelection(QList <int> mm_row)
{
	QItemSelectionModel *sel=svRecvListWidget->selectionModel();
	int mRowStart=0;
	int mRowEnd=0;
	for(int mRowCount=0;mRowCount<mm_row.count();mRowCount++)
	{
		if(mm_row.count()==1)
		{
			return;
		}
		else if(mm_row.count()==2)
		{
			if(mm_row.at(1)-mm_row.at(0)!=1)
			{
				mRowStart=mm_row.at(0); 
				mRowEnd=mm_row.at(1);
				QModelIndex index=svRecvListWidget->model()->index(mRowStart,0,svRecvListWidget->rootIndex());
				QModelIndex index_1=svRecvListWidget->model()->index(mRowStart,3,svRecvListWidget->rootIndex());

				QModelIndex index_2=svRecvListWidget->model()->index(mRowEnd,0,svRecvListWidget->rootIndex());
				QModelIndex index_3=svRecvListWidget->model()->index(mRowEnd,3,svRecvListWidget->rootIndex());
				QItemSelection seltion(index,index_1);
				QItemSelection seltion_1(index_2,index_3);
				sel->select(seltion,QItemSelectionModel::Select | QItemSelectionModel::Rows); 
				sel->select(seltion_1,QItemSelectionModel::Select | QItemSelectionModel::Rows); 
			}else
			{
				mRowStart=mm_row.at(0);
				mRowEnd=mm_row.at(1);
				QModelIndex index=svRecvListWidget->model()->index(mRowStart,0,svRecvListWidget->rootIndex());
				QModelIndex index_1=svRecvListWidget->model()->index(mRowEnd,3,svRecvListWidget->rootIndex());
				QItemSelection seltion(index,index_1);
				sel->select(seltion,QItemSelectionModel::Select | QItemSelectionModel::Rows); 
			}
			return;
		}
		else 
		{
			if(mRowCount==0)    //不集中的行 处理方法;
			{
				mRowStart=mm_row.at(mRowCount);
			}
			else
			{
				if(mm_row.at(mRowCount)-mm_row.at(mRowCount-1)!=1)
				{
					mRowEnd=mm_row.at(mRowCount-1);
					QModelIndex index=svRecvListWidget->model()->index(mRowStart,0,svRecvListWidget->rootIndex());
					QModelIndex index_1=svRecvListWidget->model()->index(mRowEnd,3,svRecvListWidget->rootIndex());
					QItemSelection seltion(index,index_1);
					sel->select(seltion,QItemSelectionModel::Select | QItemSelectionModel::Rows); 

					if(mRowCount==mm_row.count()-1)  // 如果生下的一行为单行 并且是最后一行;
					{
						QModelIndex index=svRecvListWidget->model()->index(mm_row.at(mRowCount),0,svRecvListWidget->rootIndex());
						QModelIndex index_1=svRecvListWidget->model()->index(mm_row.at(mRowCount),3,svRecvListWidget->rootIndex());
						QItemSelection seltion(index,index_1);
						sel->select(seltion,QItemSelectionModel::Select | QItemSelectionModel::Rows);

					}
					mRowStart=mm_row.at(mRowCount);

				}
				else
				{
					if(mRowCount==mm_row.count()-1)
					{
						mRowEnd=mm_row.at(mRowCount);
						QModelIndex index=svRecvListWidget->model()->index(mRowStart,0,svRecvListWidget->rootIndex());
						QModelIndex index_1=svRecvListWidget->model()->index(mRowEnd,3,svRecvListWidget->rootIndex());
						QItemSelection seltion(index,index_1);
						sel->select(seltion,QItemSelectionModel::Select | QItemSelectionModel::Rows); 
						mRowStart=mm_row.at(mRowCount);
					}

				}
			}
		}
	}
}
