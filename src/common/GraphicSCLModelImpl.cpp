#include "GraphicSCLModelImpl.h"
#include "SCLModelSvrAdapter.h"

CGraphicSCLModelImpl::CGraphicSCLModelImpl(void)
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
}

CGraphicSCLModelImpl::~CGraphicSCLModelImpl(void)
{
	delete subSetting;
}

bool CGraphicSCLModelImpl::GetBaseFileInfo(QSubStationInfo& FileInfo)
{
	return SCLModel->GetSubstationInfo(subStationKey, FileInfo);
}
bool CGraphicSCLModelImpl::GetOtherFileInfo(QSubStationInfo& FileInfo)
{
	return SCLModel->GetSubstationInfo(subStationKey, FileInfo);
}

bool CGraphicSCLModelImpl::GetIEDList(QString panelKey, QVector<QString>* IEDKeys)
{
	return SCLModel->GetIEDList(panelKey, IEDKeys);
}

bool CGraphicSCLModelImpl::GetIEDListBySubNetKey(QString subNetKey, QVector<QString> *subIEDs)
{
	return SCLModel->GetIEDListBySubNet(subNetKey, *subIEDs);
}

bool CGraphicSCLModelImpl::GetSubNetKeysByIEDKey(QString IEDKey, QVector<QString> *subNetKeys)
{
	QString panelKey = SCLModel->GetParentKey(IEDKey);
	QString ssKey = SCLModel->GetParentKey(panelKey);
	QVector<QString> allSubKeys;
	SCLModel->GetSubNetList(ssKey, allSubKeys);
	for (int i = 0; i != allSubKeys.size(); i++)
	{
		QVector<QString> subIEDs;
		SCLModel->GetIEDListBySubNet(allSubKeys.at(i), subIEDs);
		if (subIEDs.contains(IEDKey))
		{
			subNetKeys->append(allSubKeys.at(i));
		}
	}
	return true;
}

bool CGraphicSCLModelImpl::GetIEDInfo(QString IEDKey, QIEDInfo& IEDInfo)
{
	IEDInfo.DiffState = 1;
	IEDInfo.diffDesc = "";
	return SCLModel->GetIEDInfo(IEDKey, IEDInfo);
}

bool CGraphicSCLModelImpl::GetInfoOfInSignal(QString signalKey,QSignalInfo *signalInfo)
{
	signalInfo->DiffState = 1;
	signalInfo->diffDesc = "";
	return SCLModel->GetInfoOfInSignal(signalKey, signalInfo);
}

bool CGraphicSCLModelImpl::GetInfoOfOutSignal(QString signalKey,QSignalInfo *signalInfo)
{
	signalInfo->DiffState = 1;
	signalInfo->diffDesc = "";
	return SCLModel->GetInfoOfOutSignal(signalKey, signalInfo);
}

//获得masterIEDKey为关键字IED的CBkey控制块发送给slaveIEDKey为关键字IED的虚链接列表;
bool CGraphicSCLModelImpl::GetSignalInfosByIEDKeyAndCBKey(const QString masterIEDKey, const QString slaveIEDKey, const QString CBkey, QVector<QVirtualLink> *VirtualLinkList)
{
	//获取装置间所有虚链接;
	QVector<QVirtualLink> vLinks;
	SCLModel->GetVirtualSignalsAssociatedIED(masterIEDKey, slaveIEDKey, typeAll_, &vLinks);

	//根据控制块过滤;
	QVector<QString> cbSigs;
	SCLModel->GetOutSignalsByCBKey(masterIEDKey, CBkey, &cbSigs);
	for (int i = 0; i != vLinks.size(); i++)
	{
		bool isInclude = false;
		for (int j = 0; j != cbSigs.size(); j++)
		{
			QString sigKey = cbSigs.at(j);
			if (vLinks.at(i).srcSignal == sigKey)
			{
				isInclude = true;
				break;
			}
		}
		if (isInclude)
		{
			VirtualLinkList->append(vLinks.at(i));
		}
	}
	return true;
}

VirLinkDiff CGraphicSCLModelImpl::GetVirLinkStateByIEDKeyAndCBKey(const QString masterIEDKey, const QString slaveIEDKey, const QString CBkey)
{
	VirLinkDiff virLinkDiff;
	virLinkDiff.state = DIFFSTATE_NORMAL;
	virLinkDiff.iCount = 0;
	return virLinkDiff;
}

bool CGraphicSCLModelImpl::GetSignalInfosByIEDKey(const QString IEDKey, QVector<QVirtualLink> *VirtualLinkList)
{
	return SCLModel->GetVritualLinksOfIED(IEDKey,typeAll_,VirtualLinkList);
}

int	CGraphicSCLModelImpl::GetSignalCountByIEDKey(const QString IEDKey, QSignalTypeEnum sigType)
{
	QIEDInfo iedInfo;
	SCLModel->GetIEDInfo(IEDKey, iedInfo);
	if (sigType == typeGoose_)
	{
		return iedInfo.GooseInSigCount + iedInfo.GooseOutSigCount;
	}
	else if (sigType == typeSV_)
	{
		return iedInfo.SVInSigCount + iedInfo.SVOutSigCount;
	}
	return 0;
}

//-----------------------------------旧有形式---------------------------------------;
bool CGraphicSCLModelImpl::GetSignalInfosByIEDKeyAndCBKey(const QString masterIEDKey, const QString slaveIEDKey, const QString CBkey, QList<QSignalInfo> *mSigInfos, QList<QSignalInfo> *sSigInfos)
{
	return SCLModel->GetSignalInfosByIEDKeyAndCBKey(masterIEDKey, slaveIEDKey, CBkey, mSigInfos, sSigInfos);
}

bool CGraphicSCLModelImpl::GetSignalInfosByIEDKey(const QString IEDKey, QList<QSignalInfo> *mSigInfos, QList<QSignalInfo> *sSigInfos)
{
	return SCLModel->GetSignalInfosByIEDKey(IEDKey, mSigInfos, sSigInfos);
}
//-----------------------------------旧有形式---------------------------------------;

bool CGraphicSCLModelImpl::GetCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList)
{
	return SCLModel->GetCtrlBlockListByIED(IEDKey, ctrlBlockList);
}

bool CGraphicSCLModelImpl::GetCtrlBlockInfo(QString ctrlBlockKey, QCtrlBlockInfo& cbInfo)
{
	cbInfo.DiffState = 1;
	cbInfo.diffDesc = "";
	return SCLModel->GetCtrlBlockInfo(ctrlBlockKey, cbInfo);
}

bool CGraphicSCLModelImpl::GetSlaveCtrlBlockByIEDKeys(QString masterKey, QString slaveKey, QVector<QString>* ctrlBlockList)
{
	return SCLModel->GetSlaveCtrlBlockByIEDKeys(masterKey, slaveKey, ctrlBlockList);
}

bool CGraphicSCLModelImpl::GetOutSignalsByCBKey(QString masterIEDKey, QString cbKey, QVector<QString>* outSignals)
{
	return SCLModel->GetOutSignalsByCBKey(masterIEDKey, cbKey, outSignals);
}

bool CGraphicSCLModelImpl::GetPhyPortInfo(QString pPortKey, QPhyPortInfo& pPortInfo)
{
	pPortInfo.DiffState = 1;
	pPortInfo.diffDesc = "";
	return SCLModel->GetPhyPortInfo(pPortKey, pPortInfo);
}

bool CGraphicSCLModelImpl::GetPhyPortListByIED(QString IEDKey, QVector<QString>* pPortList)
{
	return SCLModel->GetPhyPortListByIED(IEDKey, pPortList);
}

bool CGraphicSCLModelImpl::GetPhyPortListByInSignalID(QString signalKey, QVector<QString> &pPortList)
{
	return SCLModel->GetPhyPortListByInSignalID(signalKey, pPortList);
}

bool CGraphicSCLModelImpl::GetPhyPortKeyByOutSignalID(QString signalKey, QString* pPortKey)
{
	return SCLModel->GetPhyPortKeyByOutSignalID(signalKey, pPortKey);
}

//IED之间虚回路的差异放在控制块中实现;
bool CGraphicSCLModelImpl::GetIEDListByLogicalRealitionWithCurrentIED(QString IEDKey,QVector<QString>* associatedIEDKeys)
{
	return SCLModel->GetIEDListByLogicalRealitionWithCurrentIED(IEDKey, associatedIEDKeys);
}