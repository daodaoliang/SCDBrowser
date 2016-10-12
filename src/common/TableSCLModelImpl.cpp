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

//ģ��ͨ�ýӿ�
QString CTableSCLModelImpl::GetParentKey(QString childKey) //�����ӽڵ�Key��ø��ڵ�Key
{
	if(SCLModel!=NULL)
	{
		SCLNameTranslator tranlator;
		return tranlator.GetParentKey(childKey.toLocal8Bit().constData()).c_str();
	}
	return "";
}

QString CTableSCLModelImpl::GetParentName(QString childKey) //�����ӽڵ�Key��ø��ڵ��Name
{
	if(SCLModel!=NULL)
	{
		SCLNameTranslator tranlator;
		return tranlator.GetParentName(childKey.toLocal8Bit().constData()).c_str();
	}
	return "";
}

QString CTableSCLModelImpl::GetNameFromKey(QString key) //����Key�������
{
	if(SCLModel!=NULL)
	{
		SCLNameTranslator tranlator;
		return tranlator.GetNameFromKey(key.toLocal8Bit().constData()).c_str();
	}
	return "";
}

bool CTableSCLModelImpl::GetBaseFileInfo(FileInfoStru& FileInfo)//��û�׼�ļ���Ϣ;
{
	QSubStationInfo subInfo;
	bool isSus = SCLModel->GetSubstationInfo(subStationKey, subInfo);
	FileInfo.basefileName = subInfo.Name;
	FileInfo.reVersion.baseAttributeName = subInfo.reversion;
	FileInfo.version.baseAttributeName = subInfo.version;
	return isSus;
}

bool CTableSCLModelImpl::GetOtherFileInfo(FileInfoStru& FileInfo)//��ñ��Ƚ��ļ���Ϣ;
{
	return GetBaseFileInfo(FileInfo);
}

bool CTableSCLModelImpl::GetIEDList(QString panelKey, QVector<QString>* IEDKeys)//�������װ��ID;
{
	return SCLModel->GetIEDList(panelKey, IEDKeys);
}

bool CTableSCLModelImpl::GetIEDInfo(QString IEDKey, IEDStru& IEDInfo)//���װ����Ϣ;
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

bool CTableSCLModelImpl::GetInfoOfInSignal(QString signalKey,SignalStru *signalInfo)//��������źŵ���Ϣ;
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

bool CTableSCLModelImpl::GetInfoOfOutSignal(QString signalKey,SignalStru *signalInfo)//�������źŵ���Ϣ;
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

bool CTableSCLModelImpl::GetInfoOfDSSettingOutSignal(QString signalKey,DSSettingSignalStru *signalInfo)//�������źŵ���Ϣ;
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


bool CTableSCLModelImpl::GetGooseCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList)//��ȡ��ǰIED��GOOSE���ƿ��б�;
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
bool CTableSCLModelImpl::GetReportCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList)//��ȡ��ǰIED��GOOSE���ƿ��б�;
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
bool CTableSCLModelImpl::GetSVCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList)//��ȡ��ǰIED��SV���ƿ��б�;
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

bool CTableSCLModelImpl::GetDsSettingListByIED(QString IEDKey, QVector<QDataSetInfo>* dsSettingList)//��ȡ��ǰIED��SV���ƿ��б�;
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

bool CTableSCLModelImpl::GetCtrlBlockInfo(QString ctrlBlockKey, SVCBStru& cbInfo)//��ȡ���ƿ���Ϣ;
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

bool CTableSCLModelImpl::GetCtrlBlockInfo(QString ctrlBlockKey, GOOSECBStru& cbInfo)//��ȡGoose���ƿ���Ϣ;
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
bool CTableSCLModelImpl::GetCtrlBlockInfo(QString ctrlBlockKey, ReportCBStru& cbInfo)//��ȡGoose���ƿ���Ϣ;
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

bool CTableSCLModelImpl::GetOutSignalsByCBKey(QString IEDKey, QString cbKey, QVector<QString>* outSignalKeys)//ͨ�����ƿ��ȡ����ź��б�;
{
	return SCLModel->GetOutSignalsByCBKey(IEDKey, cbKey, outSignalKeys);
}

bool CTableSCLModelImpl::GetOutSignalsByDataSetKey(QString IEDKey, QString DataSetKey, QVector<QString>* outSignalKeys)//ͨ�����ƿ��ȡ����ź��б�;
{
	return SCLModel->GetOutSignalsByDataSetKey(IEDKey, DataSetKey, outSignalKeys);
}

bool CTableSCLModelImpl::GetInSignalsByIEDKey(QString IEDKey, QVector<QString>* inSignalKeys)//ͨ�����ƿ��ȡ����ź��б�;
{
	return SCLModel->GetInSignalsListOfIED(IEDKey, typeAll_, inSignalKeys);
}

bool CTableSCLModelImpl::GetInputsOfIED(QString IEDKey, QVector<ExtRefStru> &ExtRefs) // ȡ��IED��Inputs��Ϣ
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

bool CTableSCLModelImpl::GetCtrlBlockOfOutIEDByInsignal(QString signalKey, SVCBStru* CBKey)		//ͨ�������źŻ�ȡ�Զ�IED�Ŀ��ƿ�;
{
	//�ҵ���ؿ��ƿ�;
	QString assoCBKey = GetAssoCBKeyBySignal(signalKey);

	//������ݽṹ;
	return GetCtrlBlockInfo(assoCBKey, *CBKey);
}

bool CTableSCLModelImpl::GetCtrlBlockOfOutIEDByInsignal(QString signalKey, GOOSECBStru* CBKey)	//ͨ�������źŻ�ȡ�Զ�IED�Ŀ��ƿ�;
{
	//�ҵ���ؿ��ƿ�;
	QString assoCBKey = GetAssoCBKeyBySignal(signalKey);
	//������ݽṹ;
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

bool CTableSCLModelImpl::GetInSignalsOfOutsignal(QString outSignalKey, QVector<QString>* inSignalKeys)//ͨ������źŻ�ȡ�Զ˵������ź��б�;
{
	return SCLModel->GetInSignalsListOfOutSignal(outSignalKey, inSignalKeys);
}

//chenxin add 2010.10.20 --->
bool CTableSCLModelImpl::GetOutSignalsOfInsignal(QString outSignalKey, QVector<QString>* inSignalKeys)//ͨ������źŻ�ȡ�Զ˵������ź��б�;
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

bool CTableSCLModelImpl::GetIEDListBySubnetKey(QString subnetKey, QVector<QString> &subIEDs)		//ͨ������Key��ȡ������IED;
{
	return SCLModel->GetIEDListBySubNet(subnetKey, subIEDs);
}

bool CTableSCLModelImpl::GetApInfo(QString subnetKey, QString IEDKey, QString APName, QAPInfo &apInfo)			//ͨ������Key�Լ�IEDKey��ȡ�������Ϣ;
{
	return SCLModel->GetAPInfo(subnetKey, IEDKey, APName, apInfo);
}

bool CTableSCLModelImpl::GetAPListOfIEDFromSubnet(const QString& subNetKey, const QString IEDName, QVector<QString> &APList)// ȡ��������IED��AP�б�
{
	return SCLModel->GetAPListOfIEDFromSubnet(subNetKey, IEDName,APList);
}

bool CTableSCLModelImpl::GetPortListOfIEDAPFromSubNet(const QString& subNetKey, const QString IEDName, const QString APName, QVector<QString> &PortList)// ȡ��������IEDĳ��AP����������˿��б�
{
	return SCLModel->GetPortListOfIEDAPFromSubNet(subNetKey,IEDName,APName,PortList);
}

bool CTableSCLModelImpl::GetAPListFromIED(const QString& IEDKey, QVector<QString> &APList)// ȡ��IED��AP���б�
{
	return SCLModel->GetAPListFromIED(IEDKey, APList);
}

bool CTableSCLModelImpl::GetAPPortListFormIED(const QString& IEDKey, const QString &APName, QVector<QString> &PortList)// ȡ��IED��ĳ��AP��Ӧ������˿��б�
{
	return SCLModel->GetAPPortListFormIED(IEDKey, APName, PortList);
}
