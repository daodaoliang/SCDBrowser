#include "TableSCLModelImpl.h"
#include "SCLModelSvrAdapter.h"
#include "SCLNameTranslator.h"
#include "QDebug"
#include "QTime"

CTableSCLModelImpl::CTableSCLModelImpl(void)
{
	subSetting = new QSettings("../cfg/SubStationInfo.ini", QSettings::IniFormat);

	QString oldSubstationID = subSetting->value("config/CurrentSubstationID").toString();
	SCLModel = SCLModelAdapter::GetInstance();
	QVector<QString> substationKeys;
	SCLModel->GetSubstationList("220kV", &substationKeys);
	int substationIndex = substationKeys.indexOf(oldSubstationID,0);

	if (substationIndex >= 0)
	{
		subStationKey = substationKeys.at(substationIndex);
	}
	else
	{
		if(substationKeys.size()>0)
		{
			subStationKey = substationKeys.at(0);
		}
		else
		{
			subStationKey = "";
		}
	}
	//chenxin modify 2014.10.26 <---
}

CTableSCLModelImpl::~CTableSCLModelImpl(void)
{
	delete subSetting;
}

//模型通用接口
QString CTableSCLModelImpl::GetParentKey(QString childKey) //根据子节点Key获得父节点Key
{
	if(SCLModel!=NULL)
	{
		SCLNameTranslator tranlator;
		return tranlator.GetParentKey(childKey.toLocal8Bit().constData()).c_str();
	}
	return "";
}

QString CTableSCLModelImpl::GetParentName(QString childKey) //根据子节点Key获得父节点的Name
{
	if(SCLModel!=NULL)
	{
		SCLNameTranslator tranlator;
		return tranlator.GetParentName(childKey.toLocal8Bit().constData()).c_str();
	}
	return "";
}

QString CTableSCLModelImpl::GetNameFromKey(QString key) //根据Key获得名称
{
	if(SCLModel!=NULL)
	{
		SCLNameTranslator tranlator;
		return tranlator.GetNameFromKey(key.toLocal8Bit().constData()).c_str();
	}
	return "";
}

bool CTableSCLModelImpl::GetBaseFileInfo(FileInfoStru& FileInfo)//获得基准文件信息;
{
	QSubStationInfo subInfo;
	bool isSus = SCLModel->GetSubstationInfo(subStationKey, subInfo);
	FileInfo.basefileName = subInfo.Name;
	FileInfo.reVersion.baseAttributeName = subInfo.reversion;
	FileInfo.version.baseAttributeName = subInfo.version;
	return isSus;
}

bool CTableSCLModelImpl::GetOtherFileInfo(FileInfoStru& FileInfo)//获得被比较文件信息;
{
	return GetBaseFileInfo(FileInfo);
}

bool CTableSCLModelImpl::GetIEDList(QString panelKey, QVector<QString>* IEDKeys)//获得所有装置ID;
{
	return SCLModel->GetIEDList(panelKey, IEDKeys);
}

bool CTableSCLModelImpl::GetIEDInfo(QString IEDKey, IEDStru& IEDInfo)//获得装置信息;
{
	QIEDInfo iedInfo;
	bool isSus = SCLModel->GetIEDInfo(IEDKey, iedInfo);
	IEDInfo.ID = IEDKey;
	IEDInfo.IEDName = iedInfo.name;
	IEDInfo.IEDDesc.baseAttributeName = iedInfo.desc;
	IEDInfo.IEDManu.baseAttributeName = iedInfo.manu;
	IEDInfo.IEDType.baseAttributeName = iedInfo.type;
	return isSus;
}

bool CTableSCLModelImpl::GetInfoOfInSignal(QString signalKey,SignalStru *signalInfo)//获得输入信号的信息;
{
	QSignalInfo sigInfo;
	bool isSus = SCLModel->GetInfoOfInSignal(signalKey, &sigInfo);
	signalInfo->ID = signalKey;
	(signalInfo->Ord).baseAttributeName = sigInfo.iOrd;
	(signalInfo->Desc).baseAttributeName = sigInfo.desc;
	(signalInfo->LNDesc).baseAttributeName = sigInfo.LnDesc;
	(signalInfo->FC).baseAttributeName = sigInfo.fc;
	(signalInfo->BDAType).baseAttributeName = sigInfo.BDAType;
	(signalInfo->DODesc).baseAttributeName = sigInfo.DOIDesc;
	
	(signalInfo->PortIndex).baseAttributeName = m_NameTranslator.GetSignalPortQStringFromList(sigInfo.portIndexList);
	(signalInfo->TripName).baseAttributeName = sigInfo.softTripKey;
	return isSus;
}

bool CTableSCLModelImpl::GetInfoOfOutSignal(QString signalKey,SignalStru *signalInfo)//获得输出信号的信息;
{
	QSignalInfo sigInfo;
	bool isSus = SCLModel->GetInfoOfOutSignal(signalKey, &sigInfo);
	signalInfo->ID = signalKey;
	(signalInfo->Ord).baseAttributeName = sigInfo.iOrd;
	(signalInfo->Desc).baseAttributeName = sigInfo.desc;
	(signalInfo->LNDesc).baseAttributeName = sigInfo.LnDesc;
	(signalInfo->FC).baseAttributeName = sigInfo.fc;
	(signalInfo->BDAType).baseAttributeName = sigInfo.BDAType;
	(signalInfo->DODesc).baseAttributeName = sigInfo.DOIDesc;
	(signalInfo->DOIdU).baseAttributeName = sigInfo.dUVal;
	(signalInfo->PortIndex).baseAttributeName = m_NameTranslator.GetSignalPortQStringFromList(sigInfo.portIndexList);
	(signalInfo->TripName).baseAttributeName = sigInfo.softTripKey;
	return isSus;
}

bool CTableSCLModelImpl::GetInfoOfDSSettingOutSignal(QString signalKey,DSSettingSignalStru *signalInfo)//获得输出信号的信息;
{
	QDSSettingSignalInfo sigInfo;
	bool isSus = SCLModel->GetInfoOfDSSettingOutSignal(signalKey, &sigInfo);
	signalInfo->cdc = sigInfo.cdc;
	signalInfo->unit = sigInfo.unit;
	signalInfo->maxVal = sigInfo.maxVal;
	signalInfo->minVal = sigInfo.minVal;
	signalInfo->stepSize = sigInfo.stepSize;
	return isSus;
}


bool CTableSCLModelImpl::GetGooseCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList)//获取当前IED的GOOSE控制块列表;
{
	QVector<QString> cbList;
	bool isSus = SCLModel->GetCtrlBlockListByIED(IEDKey, &cbList);
	for (int i = 0; i != cbList.size(); i++)
	{
		QCtrlBlockInfo cbInfo;
		SCLModel->GetCtrlBlockInfo(cbList.at(i), cbInfo);
		if (cbInfo.type == "GSEControl")
		{
			ctrlBlockList->append(cbList.at(i));
		}
	}
	return isSus;
}
bool CTableSCLModelImpl::GetReportCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList)//获取当前IED的GOOSE控制块列表;
{
	QVector<QString> cbList;
	bool isSus = SCLModel->GetCtrlBlockListByIED(IEDKey, &cbList);
	for (int i = 0; i != cbList.size(); i++)
	{
		QCtrlBlockInfo cbInfo;
		SCLModel->GetCtrlBlockInfo(cbList.at(i), cbInfo);
		if (cbInfo.type == "ReportControl")
		{
			ctrlBlockList->append(cbList.at(i));
		}
	}
	return isSus;
}
bool CTableSCLModelImpl::GetSVCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList)//获取当前IED的SV控制块列表;
{
	QVector<QString> cbList;
	bool isSus = SCLModel->GetCtrlBlockListByIED(IEDKey, &cbList);
	for (int i = 0; i != cbList.size(); i++)
	{
		QCtrlBlockInfo cbInfo;
		SCLModel->GetCtrlBlockInfo(cbList.at(i), cbInfo);
		if (cbInfo.type == "SampledValueControl")
		{
			ctrlBlockList->append(cbList.at(i));
		}
	}
	return isSus;
}

bool CTableSCLModelImpl::GetDsSettingListByIED(QString IEDKey, QVector<QDataSetInfo>* dsSettingList)//获取当前IED的SV控制块列表;
{
	QVector<QDataSetInfo> cbList;
	bool isSus = SCLModel->GetDsSettingListByIED(IEDKey, &cbList);
	for (int i = 0; i != cbList.size(); i++)
	{
		dsSettingList->append(cbList.at(i));
	}
	return isSus;
}


int CTableSCLModelImpl::GetSigCountByIED(QString IEDKey, bool IsInSig, QSignalTypeEnum sigType)
{
	QIEDInfo iedInfo;
	SCLModel->GetIEDInfo(IEDKey, iedInfo);
	if (IsInSig)
	{
		if (sigType == typeGoose_)
		{
			return iedInfo.GooseInSigCount;
		} 
		else if(sigType == typeSV_)
		{
			return iedInfo.SVInSigCount;
		}
	}
	else
	{
		if (sigType == typeGoose_)
		{
			return iedInfo.GooseOutSigCount;
		} 
		else if (sigType == typeSV_)
		{
			return iedInfo.SVOutSigCount;
		}
	}
	return 0;
}

bool CTableSCLModelImpl::GetCtrlBlockInfo(QString ctrlBlockKey, SVCBStru& cbInfo)//获取控制块信息;
{
	QCtrlBlockInfo ctrlBlockInfo;
	bool isSus = SCLModel->GetCtrlBlockInfo(ctrlBlockKey, ctrlBlockInfo);
	cbInfo.ID = ctrlBlockKey;
	cbInfo.apAppID.baseAttributeName = ctrlBlockInfo.apAPPID;
	cbInfo.AppID.baseAttributeName = ctrlBlockInfo.APPID;
	cbInfo.ASDUCount.baseAttributeName = ctrlBlockInfo.ASDU;
	cbInfo.CBName.baseAttributeName = ctrlBlockInfo.name;
	cbInfo.CBDesc.baseAttributeName = ctrlBlockInfo.name;
	cbInfo.ConfVersion.baseAttributeName = ctrlBlockInfo.confRev;
	cbInfo.DataSetAddr.baseAttributeName = ctrlBlockInfo.dataSetAddr;
	cbInfo.DataSetName.baseAttributeName = ctrlBlockInfo.dataSetName;
	cbInfo.MACAddr.baseAttributeName = ctrlBlockInfo.apMAC;
	cbInfo.smpRate.baseAttributeName = ctrlBlockInfo.smpRate;
	cbInfo.VLANID.baseAttributeName = ctrlBlockInfo.apVLAN_ID;
	cbInfo.VLANPriority.baseAttributeName = ctrlBlockInfo.apVLAN_PRIORITY;
	//cbInfo.DataSetCount.baseAttributeName = 0;
	QVector<QString> cbSigs;
	SCLModel->GetOutSignalsByCBKey("", ctrlBlockKey, &cbSigs);
	cbInfo.DataSetCount.baseAttributeName = cbSigs.size();
	return isSus;
}

bool CTableSCLModelImpl::GetCtrlBlockInfo(QString ctrlBlockKey, GOOSECBStru& cbInfo)//获取Goose控制块信息;
{
	QCtrlBlockInfo ctrlBlockInfo;
	bool isSus = SCLModel->GetCtrlBlockInfo(ctrlBlockKey, ctrlBlockInfo);
	cbInfo.ID = ctrlBlockKey;
	cbInfo.apAppID.baseAttributeName = ctrlBlockInfo.apAPPID;
	cbInfo.AppID.baseAttributeName = ctrlBlockInfo.APPID;
	cbInfo.CBName.baseAttributeName = ctrlBlockInfo.name;
	cbInfo.CBDesc.baseAttributeName = ctrlBlockInfo.name;
	cbInfo.MaxTime.baseAttributeName = ctrlBlockInfo.maxTime;
	cbInfo.MinTime.baseAttributeName = ctrlBlockInfo.minTime;
	cbInfo.GocbRef.baseAttributeName = ctrlBlockInfo.gocbRef;
	cbInfo.ConfVersion.baseAttributeName = ctrlBlockInfo.confRev;
	cbInfo.DataSetAddr.baseAttributeName = ctrlBlockInfo.dataSetAddr;
	cbInfo.DataSetName.baseAttributeName = ctrlBlockInfo.dataSetName;
	cbInfo.MACAddr.baseAttributeName = ctrlBlockInfo.apMAC;
	cbInfo.VLANID.baseAttributeName = ctrlBlockInfo.apVLAN_ID;
	cbInfo.VLANPriority.baseAttributeName = ctrlBlockInfo.apVLAN_PRIORITY;
	QVector<QString> cbSigs;
	SCLModel->GetOutSignalsByCBKey("", ctrlBlockKey, &cbSigs);
	cbInfo.DataSetCount.baseAttributeName = cbSigs.size();
	return isSus;
}
bool CTableSCLModelImpl::GetCtrlBlockInfo(QString ctrlBlockKey, ReportCBStru& cbInfo)//获取Goose控制块信息;
{
	QCtrlBlockInfo ctrlBlockInfo;
	bool isSus = SCLModel->GetCtrlBlockInfo(ctrlBlockKey, ctrlBlockInfo);
	cbInfo.ID = ctrlBlockKey;
	cbInfo.apAppID.baseAttributeName = ctrlBlockInfo.apAPPID;
	cbInfo.AppID.baseAttributeName = ctrlBlockInfo.APPID;
	cbInfo.CBName.baseAttributeName = ctrlBlockInfo.name;
	cbInfo.CBDesc.baseAttributeName = ctrlBlockInfo.name;
	cbInfo.ConfVersion.baseAttributeName = ctrlBlockInfo.confRev;
	cbInfo.DataSetAddr.baseAttributeName = ctrlBlockInfo.dataSetAddr;
	cbInfo.DataSetName.baseAttributeName = ctrlBlockInfo.dataSetName;
	cbInfo.MACAddr.baseAttributeName = ctrlBlockInfo.apMAC;
	cbInfo.VLANID.baseAttributeName = ctrlBlockInfo.apVLAN_ID;
	cbInfo.VLANPriority.baseAttributeName = ctrlBlockInfo.apVLAN_PRIORITY;

	cbInfo.LDInst = ctrlBlockInfo.LDInst;
	cbInfo.desc = ctrlBlockInfo.desc;
	cbInfo.intgPd = ctrlBlockInfo.intgPd;
	cbInfo.Buffered = ctrlBlockInfo.Buffered;
	cbInfo.bufTime = ctrlBlockInfo.bufTime;

	QVector<QString> cbSigs;
	SCLModel->GetOutSignalsByCBKey("", ctrlBlockKey, &cbSigs);
	cbInfo.DataSetCount.baseAttributeName = cbSigs.size();

	return isSus;
}

bool CTableSCLModelImpl::GetOutSignalsByCBKey(QString IEDKey, QString cbKey, QVector<QString>* outSignalKeys)//通过控制块获取输出信号列表;
{
	return SCLModel->GetOutSignalsByCBKey(IEDKey, cbKey, outSignalKeys);
}

bool CTableSCLModelImpl::GetOutSignalsByDataSetKey(QString IEDKey, QString DataSetKey, QVector<QString>* outSignalKeys)//通过控制块获取输出信号列表;
{
	return SCLModel->GetOutSignalsByDataSetKey(IEDKey, DataSetKey, outSignalKeys);
}

bool CTableSCLModelImpl::GetInSignalsByIEDKey(QString IEDKey, QVector<QString>* inSignalKeys)//通过控制块获取输出信号列表;
{
	return SCLModel->GetInSignalsListOfIED(IEDKey, typeAll_, inSignalKeys);
}

bool CTableSCLModelImpl::GetInputsOfIED(QString IEDKey, QVector<ExtRefStru> &ExtRefs) // 取得IED的Inputs信息
{
	QVector<QVirtualLink> vLinks;
	SCLModel->GetVritualLinksOfIED(IEDKey, typeAll_,&vLinks);
	for (int i = 0; i != vLinks.size(); i++)
	{
		QVirtualLink tempVLink = vLinks.at(i);
		QString tempIEDKey = SCLModel->GetParentKey(tempVLink.destSignal);
		if (tempIEDKey == IEDKey)
		{
			QSignalInfo inSigInfo, outSigInfo;
			SCLModel->GetInfoOfInSignal(tempVLink.destSignal, &inSigInfo);
			SCLModel->GetInfoOfOutSignal(tempVLink.srcSignal, &outSigInfo);
			ExtRefStru extRefs;
			extRefs.state = DIFFSTATE_NORMAL;
			extRefs.ID = tempVLink.srcSignal;
			extRefs.Ord.baseAttributeName = inSigInfo.iOrd;
			extRefs.desc.baseAttributeName = inSigInfo.desc;
			extRefs.OutAddr.baseAttributeName = outSigInfo.path;
			extRefs.IntAddr.baseAttributeName = inSigInfo.path;
			extRefs.srcSignalID = tempVLink.srcSignal;
			extRefs.destSignalID = tempVLink.destSignal;
			extRefs.PortName.baseAttributeName = m_NameTranslator.GetSignalPortQStringFromList(inSigInfo.portIndexList);
			extRefs.type = inSigInfo.type;
			SignalStru tempInSigStru;
			GetInfoOfInSignal(tempVLink.destSignal, &tempInSigStru);
			extRefs.InSigInfo = tempInSigStru;
			ExtRefs.append(extRefs);
		}
	}

	return true;
}

bool CTableSCLModelImpl::GetCtrlBlockOfOutIEDByInsignal(QString signalKey, SVCBStru* CBKey)		//通过输入信号获取对端IED的控制块;
{
	//找到相关控制块;
	QString assoCBKey = GetAssoCBKeyBySignal(signalKey);

	//填充数据结构;
	return GetCtrlBlockInfo(assoCBKey, *CBKey);
}

bool CTableSCLModelImpl::GetCtrlBlockOfOutIEDByInsignal(QString signalKey, GOOSECBStru* CBKey)	//通过输入信号获取对端IED的控制块;
{
	//找到相关控制块;
	QString assoCBKey = GetAssoCBKeyBySignal(signalKey);
	//填充数据结构;
	bool isSus = GetCtrlBlockInfo(assoCBKey, *CBKey);
	return isSus;
}

bool CTableSCLModelImpl::GetCtrlBlockOfOutIEDByOutsignal(QString signalKey, SVCBStru* CBKey)
{
	QSignalInfo sigInfo;
	bool getInfoSus = SCLModel->GetInfoOfOutSignal(signalKey, &sigInfo);
	if (getInfoSus)
	{
		bool getCBSus = GetCtrlBlockInfo(sigInfo.ctrlBlockID, *CBKey);
		return getCBSus;
	}
	return false;
}

bool CTableSCLModelImpl::GetCtrlBlockOfOutIEDByOutsignal(QString signalKey, GOOSECBStru* CBKey)
{
	QSignalInfo sigInfo;
	bool getInfoSus = SCLModel->GetInfoOfOutSignal(signalKey, &sigInfo);
	if (getInfoSus)
	{
		bool getCBSus = GetCtrlBlockInfo(sigInfo.ctrlBlockID, *CBKey);
		return getCBSus;
	}
	return false;
}

bool CTableSCLModelImpl::GetInSignalsOfOutsignal(QString outSignalKey, QVector<QString>* inSignalKeys)//通过输出信号获取对端的输入信号列表;
{
	return SCLModel->GetInSignalsListOfOutSignal(outSignalKey, inSignalKeys);
}

//chenxin add 2010.10.20 --->
bool CTableSCLModelImpl::GetOutSignalsOfInsignal(QString outSignalKey, QVector<QString>* inSignalKeys)//通过输出信号获取对端的输入信号列表;
{
	return SCLModel->GetOutSignalsListOfInSignal(outSignalKey, inSignalKeys);
}
//chenxin add 2010.10.20 <---

QString CTableSCLModelImpl::GetAssoCBKeyBySignal(const QString sigKey)
{
	QString outSigKey;

	QVector<QString> tempOutKey;
	SCLModel->GetOutSignalsListOfInSignal(sigKey, &tempOutKey);
	outSigKey = tempOutKey.at(0);

	QSignalInfo outSigInfo;
	SCLModel->GetInfoOfOutSignal(outSigKey, &outSigInfo);
	QString assoCBKey = outSigInfo.ctrlBlockID;

	return assoCBKey;
}

bool CTableSCLModelImpl::GetIEDListBySubnetKey(QString subnetKey, QVector<QString> &subIEDs)		//通过子网Key获取子网内IED;
{
	return SCLModel->GetIEDListBySubNet(subnetKey, subIEDs);
}

bool CTableSCLModelImpl::GetApInfo(QString subnetKey, QString IEDKey, QString APName, QAPInfo &apInfo)			//通过子网Key以及IEDKey获取接入点信息;
{
	return SCLModel->GetAPInfo(subnetKey, IEDKey, APName, apInfo);
}

bool CTableSCLModelImpl::GetAPListOfIEDFromSubnet(const QString& subNetKey, const QString IEDName, QVector<QString> &APList)// 取得子网下IED的AP列表
{
	return SCLModel->GetAPListOfIEDFromSubnet(subNetKey, IEDName,APList);
}

bool CTableSCLModelImpl::GetPortListOfIEDAPFromSubNet(const QString& subNetKey, const QString IEDName, const QString APName, QVector<QString> &PortList)// 取得子网下IED某个AP包含的物理端口列表
{
	return SCLModel->GetPortListOfIEDAPFromSubNet(subNetKey,IEDName,APName,PortList);
}

bool CTableSCLModelImpl::GetAPListFromIED(const QString& IEDKey, QVector<QString> &APList)// 取得IED下AP的列表
{
	return SCLModel->GetAPListFromIED(IEDKey, APList);
}

bool CTableSCLModelImpl::GetAPPortListFormIED(const QString& IEDKey, const QString &APName, QVector<QString> &PortList)// 取得IED下某个AP对应的物理端口列表
{
	return SCLModel->GetAPPortListFormIED(IEDKey, APName, PortList);
}
