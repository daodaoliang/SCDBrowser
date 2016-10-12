#include "SCLModelSvrAdapter.h"
#include "QMessageBox"
#include <QDebug>
#include <windows.h>
#include "SCLNameTranslator.h"



SCLModelAdapter *SCLModelAdapter::m_pInstance = NULL;

QString SCLModelAdapter::GetSubstationImportTime(QString subStationKey)
{
	if(subStationKey.isEmpty())
		return "";
	return SCLModelSvrAdapter->GetSubstationImportTime(subStationKey.toLocal8Bit().constData()).c_str();//需要修改
}

//模型通用接口
QString SCLModelAdapter::GetParentKey(QString childKey) //根据子节点Key获得父节点Key
{
	if(SCLModelSvrAdapter!=NULL)
	{
		SCLNameTranslator tranlator;
		return tranlator.GetParentKey(childKey.toLocal8Bit().constData()).c_str();
	}
	return "";
}

QString SCLModelAdapter::GetParentName(QString childKey) //根据子节点Key获得父节点的Name
{
	if(SCLModelSvrAdapter!=NULL)
	{
		SCLNameTranslator tranlator;
		return tranlator.GetParentName(childKey.toLocal8Bit().constData()).c_str();
	}
	return "";
}

QString SCLModelAdapter::GetNameFromKey(QString key) //根据Key获得名称
{
	if(SCLModelSvrAdapter!=NULL)
	{
		SCLNameTranslator tranlator;
		return tranlator.GetNameFromKey(key.toLocal8Bit().constData()).c_str();
	}
	return "";
}

QString SCLModelAdapter::GetChildKeyFromParentKeyAndChildName(QString parentKey,QString childName)//根据父节点的Key和子节点Name获得子节点的Key 
{
	if(SCLModelSvrAdapter!=NULL)
	{
		SCLNameTranslator tranlator;
		return tranlator.GetChildKeyFromParentKeyAndChildName(parentKey.toLocal8Bit().constData(),childName.toLocal8Bit().constData()).c_str();
	}
	return "";
}


//模型基本访问接口
bool   SCLModelAdapter::GetVoltageLevelList(QVector<QString>* nameList)//获得电压等级名列表（同Key）
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> voltNameList;
		SCLModelSvrAdapter->GetVoltageLevelList(voltNameList);
		for(uint i=0;i<voltNameList.size();i++)
		{
			nameList->append(voltNameList[i].c_str());
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetSubstationList(QString voltageLevelKey,QVector<QString>* subStationKeyList)           //获得变电站Key列表
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempSubStationNameList;
		SCLModelSvrAdapter->GetSubstationList(voltageLevelKey.toLocal8Bit().constData(),tempSubStationNameList);
		for(uint i=0;i<tempSubStationNameList.size();i++)
		{
			subStationKeyList->append(tempSubStationNameList[i].c_str());
		}
		return true;
	}
	return false;
}

bool SCLModelAdapter::GetSubstationInfo(QString subStationKey, QSubStationInfo& subStationInfo)//获得变电站（特定SCD版本）信息	
{
	if(SCLModelSvrAdapter!=NULL)
	{
		SubStationInfo tempInfo;
		if(SCLModelSvrAdapter->GetSubstationInfo(subStationKey.toLocal8Bit().constData()
			,tempInfo))
		{
			subStationInfo.Name = tempInfo.Name.c_str();
			subStationInfo.desc = tempInfo.desc.c_str();
			subStationInfo.version = tempInfo.version.c_str();
			subStationInfo.reversion = tempInfo.reversion.c_str();
			subStationInfo.fileName = tempInfo.fileName.c_str();
			subStationInfo.filePath = tempInfo.filePath.c_str();
			subStationInfo.crc = tempInfo.crc.c_str();
			subStationInfo.toolID = tempInfo.toolID.c_str();
			return true;
		}
	}
	return false;
}

QString SCLModelAdapter::GetSubstationPrivateXMLInfo(const QString& subStationKey)//获得变电站private标签XML原文
{
	if (SCLModelSvrAdapter != NULL)
	{
		std::string privateStr = SCLModelSvrAdapter->GetSubstationPrivateXMLInfo(subStationKey.toLocal8Bit().constData());
		return QString::fromUtf8(privateStr.c_str());
	}
	return "";
}

QString SCLModelAdapter::GetSubstationHeaderXMLInfo(const QString& subStationKey)//获得变电站Header标签XML原文
{
	if (SCLModelSvrAdapter != NULL)
	{
		std::string headerStr = SCLModelSvrAdapter->GetSubstationHeaderXMLInfo(subStationKey.toLocal8Bit().constData());
		return QString::fromUtf8(headerStr.c_str());
	}
	return "";
}

bool SCLModelAdapter::GetSCDHItem(QString subStationKey, QVector<QHItemInfo>& hItemList)//获得SCD历史修订记录
{
	if(SCLModelSvrAdapter!=NULL)
	{

		try
		{
			vector<HItemInfo> tempHItemInfos;
			SCLModelSvrAdapter->GetSCDHItem(subStationKey.toLocal8Bit().constData(),tempHItemInfos);

			for(uint i=0;i<tempHItemInfos.size();i++)
			{
				QHItemInfo item;
				item.version  = tempHItemInfos[i].version.c_str();
				item.reversion = tempHItemInfos[i].reversion.c_str();
				item.substationName  = tempHItemInfos[i].substationName.c_str();
				item.when = tempHItemInfos[i].when.c_str();
				item.who = tempHItemInfos[i].who.c_str();
				item.what = tempHItemInfos[i].what.c_str();
				item.why = tempHItemInfos[i].why.c_str();
				hItemList.append(item);
			}
			return true;		
		}
		catch (exception *e)
		{
			e->what();
		}

	}
	return false;
}
bool SCLModelAdapter::GetSubNetList(QString subStationKey, QVector<QString>& subNetKeyList)//获得子网Key列表
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempSubNetKeyList;
		SCLModelSvrAdapter->GetSubNetList(subStationKey.toLocal8Bit().constData(),tempSubNetKeyList);
		for(uint i=0;i<tempSubNetKeyList.size();i++)
		{
			subNetKeyList.append(tempSubNetKeyList[i].c_str());
		}
		return true;
	}
	return false;
}
bool SCLModelAdapter::GetSubNetInfo(QString subNetKey, QSubNetInfo& subNetInfo)//获得子网信息
{
	if(SCLModelSvrAdapter!=NULL)
	{
		SubNetInfo tempInfo;
		if(SCLModelSvrAdapter->GetSubNetInfo(subNetKey.toLocal8Bit().constData()
			,tempInfo))
		{
			subNetInfo.name = tempInfo.name.c_str();
			subNetInfo.desc = tempInfo.desc.c_str();
			subNetInfo.type = tempInfo.type.c_str();
			return true;
		}
	}
	return false;
}

QString SCLModelAdapter::GetSubNetXMLInfo(const QString& subNetKey)
{
	if (SCLModelSvrAdapter != NULL)
	{
		std::string subNetXML = SCLModelSvrAdapter->GetSubNetXMLInfo(subNetKey.toLocal8Bit().constData());
		return QString::fromUtf8(subNetXML.c_str());
	}
	return "";
}

bool   SCLModelAdapter::GetAPInfo(QString subNetKey, QString IEDName, QString APName, QAPInfo& apInfo)//获得访问点信息
{
	if(SCLModelSvrAdapter!=NULL)
	{
		QString substationID = GetParentKey(subNetKey);
		QString subNetName = GetNameFromKey(subNetKey);
		APInfo tempInfo;
		if(SCLModelSvrAdapter->GetApInfo(substationID.toLocal8Bit().constData(), subNetName.toLocal8Bit().constData(), IEDName.toLocal8Bit().constData(), APName.toLocal8Bit().constData(), tempInfo))////需要修改
		{
			apInfo.name = tempInfo.name.c_str();
			apInfo.desc = tempInfo.desc.c_str();
			apInfo.subNetname = tempInfo.subNetname.c_str();
			apInfo.ipAddr = tempInfo.ipAddr.c_str();
			apInfo.macAddr = tempInfo.macAddr.c_str();
		}
	}
	return false;
}

bool   SCLModelAdapter::GetPanelList(QString subStationKey,QVector<QString>* subPanelKeyList)                    //获得屏柜Key列表
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempPanelKeyList;
		SCLModelSvrAdapter->GetPanelList(subStationKey.toLocal8Bit().constData(),tempPanelKeyList);
		for(uint i=0;i<tempPanelKeyList.size();i++)
		{
			subPanelKeyList->append(tempPanelKeyList[i].c_str());
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetIEDList(QString panelKey,QVector<QString>* subIEDKeyList)                             //获得装置Key列表
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempIEDKeyList;
		SCLModelSvrAdapter->GetIEDList(panelKey.toLocal8Bit().constData(),tempIEDKeyList);
		for(uint i=0;i<tempIEDKeyList.size();i++)
		{
			subIEDKeyList->append(tempIEDKeyList[i].c_str());
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetIEDListBySubNet(QString subNetKey, QVector<QString>& subIEDKeyList)//获得装置Key列表(根据子网)
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempIEDKeyList;
		SCLModelSvrAdapter->GetIEDListBySubNet(subNetKey.toLocal8Bit().constData(),tempIEDKeyList);
		for(uint i=0;i<tempIEDKeyList.size();i++)
		{
			subIEDKeyList.append(tempIEDKeyList[i].c_str());
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetIEDInfo(QString IEDKey, QIEDInfo& info)//获得装置信息	
{
	if(SCLModelSvrAdapter!=NULL)
	{
		DEVInfo tempInfo;
		if(SCLModelSvrAdapter->GetIEDInfo(IEDKey.toLocal8Bit().constData()
			,tempInfo))
		{
			info.name = tempInfo.name.c_str();
			info.crc = tempInfo.crc.c_str();
			info.desc = tempInfo.desc.c_str();
			info.manu = tempInfo.manu.c_str();
			info.type = tempInfo.type.c_str();
			info.configVer = tempInfo.configVer.c_str();
			info.funVer = tempInfo.funVer.c_str();
			info.GooseInSigCount = tempInfo.GooseInSigCount;
			info.GooseOutSigCount = tempInfo.GooseOutSigCount;
			info.SVInSigCount = tempInfo.SVInSigCount;
			info.SVOutSigCount = tempInfo.SVOutSigCount;
			info.ReportCBCount = tempInfo.ReportCBCount;
			info.DsSettingCount = tempInfo.DsSettingCount;
			return true;
		}
	}
	return false;
}

QString SCLModelAdapter::GetIEDXMLInfo(QString &IEDKey)//获得IED的XML信息
{
    if(SCLModelSvrAdapter!=NULL)
    {
        std::string xmlInfo = SCLModelSvrAdapter->GetIEDXMLInfo(IEDKey.toLocal8Bit().constData());
        return QString::fromUtf8(xmlInfo.c_str());
    }
    return false;
}

QString SCLModelAdapter::GetSCDVersion(QString subStationKey)                                                    //获得当前模型的SCD文件版本号
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->GetSCDVersion(subStationKey.toLocal8Bit().constData()).c_str();
	}
	return false;
}

bool   SCLModelAdapter::IsVoltageLevelExist(QString voltageLevelName)                                            //是否存在电压等级
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->IsVoltageLevelExist(voltageLevelName.toLocal8Bit().constData());
	}
	return false;
}

bool   SCLModelAdapter::IsSubstationExist(QString substationKey)                                                 //是否存在变电站	
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->IsSubstationExist(substationKey.toLocal8Bit().constData());
	}
	return false;
}

bool   SCLModelAdapter::GetPortsList(QString IEDKey,QVector<QString>* portKeys)                                  //获得装置的物理端口列表
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempPortKeys;
		SCLModelSvrAdapter->GetPortsList(IEDKey.toLocal8Bit().constData(),tempPortKeys);
		for(uint i=0;i<tempPortKeys.size();i++)
		{
			portKeys->append(tempPortKeys[i].c_str());
		}
		return true;
	}
	return false;
}

QString SCLModelAdapter::GetPortDescByPortKey(QString portKey)                                                   //获得物理端口的描述
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->GetPortDescByPortKey(portKey.toLocal8Bit().constData()).c_str();
	}
	return "";
}

bool   SCLModelAdapter::GetLinksListOfIED(QString IEDKey,QVector<QPhysLink>* links)                              //获得装置的物理链接列表 
{
	if(SCLModelSvrAdapter!=NULL)
	{

		try
		{
			vector<PhysLinkInfo> tempPortKeys;
			SignalInfo info;
			SCLModelSvrAdapter->GetLinksListOfIED(IEDKey.toLocal8Bit().constData(),tempPortKeys);

			for(uint i=0;i<tempPortKeys.size();i++)
			{
				QPhysLink link;
				link.srcPort  = tempPortKeys[i].srcPort.c_str();
				link.destPort = tempPortKeys[i].destPort.c_str();
				links->append(link);
			}
			return true;		
		}
		catch (exception *e)
		{
			e->what();
		}

	}
	return false;
}

bool SCLModelAdapter::GetIEDType(QString IEDKey,int* type)                 //获得装置类型
{
	if(SCLModelSvrAdapter!=NULL)
	{
		if (IEDKey.length() < 1)
			return false;
		return SCLModelSvrAdapter->GetIEDType(IEDKey.toLocal8Bit().constData(),*type);
	}
	return false;

}

bool   SCLModelAdapter::GetInSignalsListOfIED(QString IEDKey,QSignalTypeEnum signalType,QVector<QString>* inSignalKeys)                     //获得装置的输入信号列表
{
	if(SCLModelSvrAdapter!=NULL)
	{
		if (IEDKey.length() < 1)
			return false;
		vector<string> tempinSignalKeys;
		SCLModelSvrAdapter->GetInSignalsListOfIED(IEDKey.toLocal8Bit().constData(),(SignalTypeEnum)signalType,tempinSignalKeys);
		for(uint i=0;i<tempinSignalKeys.size();i++)
		{
			inSignalKeys->append(tempinSignalKeys[i].c_str());
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetOutSignalsListOfIED(QString IEDKey,QSignalTypeEnum signalType,QVector<QString>* outSignalKeys)                   //获得装置的输出信号列表 
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempOutSignalKeys;
		SCLModelSvrAdapter->GetOutSignalsListOfIED(IEDKey.toLocal8Bit().constData(),(SignalTypeEnum)signalType,tempOutSignalKeys);
		for(uint i=0;i<tempOutSignalKeys.size();i++)
		{
			outSignalKeys->append(tempOutSignalKeys[i].c_str());
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetVritualLinksOfIED(QString IEDKey,QSignalTypeEnum signalType,QVector<QVirtualLink>* links)                        //获得装置的虚拟链接列表（装置和装置之间）
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<VirtualLinkInfo> tempLinks;
		SCLModelSvrAdapter->GetVritualLinksOfIED(IEDKey.toLocal8Bit().constData(),(SignalTypeEnum)signalType,tempLinks);
		for(uint i=0;i<tempLinks.size();i++)
		{
			QVirtualLink link;
			link.iOrd = tempLinks[i].iOrd;
			link.srcSignal  = tempLinks[i].srcSignal.c_str();
			link.destSignal = tempLinks[i].destSignal.c_str();
			link.inSigInfo = tempLinks[i].inSigDesc.c_str();
			links->append(link);
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetInternalVritualLinksOfIED(QString IEDKey,QSignalTypeEnum signalType,QVector<QVirtualLink>* links)                //获得装置内部的虚拟链接列表（装置内部）
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<VirtualLinkInfo> tempLinks;
		SCLModelSvrAdapter->GetInternalVritualLinksOfIED(IEDKey.toLocal8Bit().constData(),(SignalTypeEnum)signalType,tempLinks);
		for(uint i=0;i<tempLinks.size();i++)
		{
			QVirtualLink link;
			link.srcSignal  = tempLinks[i].srcSignal.c_str();
			link.destSignal = tempLinks[i].destSignal.c_str();
			links->append(link);
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetFunctionsList(QString IEDKey,QVector<QString>* protectFuncKeys)                       //获得装置的功能列表
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempFuncKeys;
		SCLModelSvrAdapter->GetFunctionsList(IEDKey.toLocal8Bit().constData(),tempFuncKeys);
		for(uint i=0;i<tempFuncKeys.size();i++)
		{
			protectFuncKeys->append(tempFuncKeys[i].c_str());
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetVirtualLinksListOfPhysicsLink(QPhysLink phyLink,QSignalTypeEnum signalType,QVector<QVirtualLink>* links)     //获得物理链接关联的虚拟链接列表
{
	if(SCLModelSvrAdapter!=NULL)
	{
		PhysLinkInfo tempPhyLink;
		tempPhyLink.srcPort = phyLink.srcPort.toLocal8Bit().constData();
		tempPhyLink.destPort = phyLink.destPort.toLocal8Bit().constData();

		vector<VirtualLinkInfo> tempLinks;
		SCLModelSvrAdapter->GetVirtualLinksListOfPhysicsLink(tempPhyLink,(SignalTypeEnum)signalType,tempLinks);
		for(uint i=0;i<tempLinks.size();i++)
		{
			QVirtualLink tempVirLink;
			tempVirLink.srcSignal = tempLinks[i].srcSignal.c_str();
			tempVirLink.destSignal = tempLinks[i].destSignal.c_str();
			links->append(tempVirLink);
		}
		return true;
	}
	return false;
}

bool SCLModelAdapter::GetTripsList(QString IEDKey,QVector<QString>* tripKeys)                                  //获得装置的压板列表
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempTripKeys;
		SCLModelSvrAdapter->GetTripsList(IEDKey.toLocal8Bit().constData(),tempTripKeys);
		for(uint i=0;i<tempTripKeys.size();i++)
		{
			tripKeys->append(tempTripKeys[i].c_str());
		}
		return true;
	}
	return false;
}

bool  SCLModelAdapter::GetInfoOfInSignal(QString signalKey,QSignalInfo *signalInfo)            //获得输入信号的信息  
{
	if(SCLModelSvrAdapter!=NULL)
	{
		SignalInfo info;

		//chenxin add 2014.10.23 --->
		if(inSignalInfosMap.contains(signalKey))
		{
			QMap<QString,QSignalInfo>::iterator it = inSignalInfosMap.find(signalKey);
			*signalInfo = it.value();
			return true;
		}
		//chenxin add 2014.10.23 <---

		if(SCLModelSvrAdapter->GetInfoOfInSignal(signalKey.toLocal8Bit().constData()
			,info))
		{
			signalInfo->iOrd = info.iOrd;
			signalInfo->iOutSigOrd = info.iOutSigOrd;
			signalInfo->ID = signalKey;
			signalInfo->name = info.signalName.c_str();
			signalInfo->path = info.signalPath.c_str();
			signalInfo->desc = info.signalDesc.c_str();
			if (info.signalType == typeGoose)
			{
				signalInfo->type = typeGoose_;
			}
			else if(info.signalType == typeSV)
			{
				signalInfo->type = typeSV_;
			}
			signalInfo->iedName = info.iedName.c_str();
			signalInfo->dataSet = info.dataSet.c_str();
			signalInfo->fc = info.fc.c_str();
			signalInfo->BDAType = info.BDAType.c_str();
			signalInfo->DOIDesc = info.DOIDesc.c_str();
			signalInfo->dUVal = info.dUVal.c_str();
			signalInfo->LnName = info.LnName.c_str();
			signalInfo->LnDesc = info.LnDesc.c_str();
			signalInfo->plugType = info.plugType.c_str();

			signalInfo->softTripKey = info.softTripKey.c_str();
			signalInfo->state = info.state;

			for (uint i = 0; i != info.portNames.size(); i++)
			{
				signalInfo->portIndexList.append(info.portNames.at(i).c_str());
			}

			//chenxin add 2014.10.23 --->
			if(!inSignalInfosMap.contains(signalKey))
			{
			    inSignalInfosMap.insert(signalKey,*signalInfo);
			}
			//chenxin add 2014.10.23 <---

			return true;
		}
	}
	return false;
}

bool  SCLModelAdapter::GetInfoOfOutSignal(QString signalKey,QSignalInfo *signalInfo)           //获得输出信号的信息 
{
	if(SCLModelSvrAdapter!=NULL)
	{
		//chenxin add 2014.10.23 --->
		if(outSignalInfosMap.contains(signalKey))
		{
			QMap<QString,QSignalInfo>::iterator it = outSignalInfosMap.find(signalKey);
			*signalInfo = it.value();
			return true;
		}
		//chenxin add 2014.10.23 <---


		SignalInfo info;
		if(SCLModelSvrAdapter->GetInfoOfOutSignal(signalKey.toLocal8Bit().constData()
			,info))
		{
			signalInfo->iOrd = info.iOrd;
			signalInfo->iOutSigOrd = info.iOutSigOrd;
			signalInfo->ID = signalKey;
			signalInfo->name = info.signalName.c_str();
			signalInfo->path = info.signalPath.c_str();
			signalInfo->desc = info.signalDesc.c_str();
			if (info.signalType == typeGoose)
			{
				signalInfo->type = typeGoose_;
			}
			else if(info.signalType == typeSV)
			{
				signalInfo->type = typeSV_;
			}
			signalInfo->iedName = info.iedName.c_str();
			signalInfo->dataSet = info.dataSet.c_str();
			signalInfo->fc = info.fc.c_str();
			signalInfo->BDAType = info.BDAType.c_str();
			signalInfo->DOIDesc = info.DOIDesc.c_str();
			signalInfo->dUVal = info.dUVal.c_str();
			signalInfo->LnName = info.LnName.c_str();
			signalInfo->LnDesc = info.LnDesc.c_str();
			signalInfo->plugType = info.plugType.c_str();
			// edit by zxc 2014.12.10
			for (uint i = 0; i < info.portNames.size(); i++)
			{
				signalInfo->portIndexList.push_back(info.portNames[i].c_str());
			}

			signalInfo->softTripKey = info.softTripKey.c_str();
			signalInfo->ctrlBlockID = info.ctrlBlockID.c_str();
			signalInfo->state = info.state;


			//chenxin add 2014.10.23 --->
			if(!outSignalInfosMap.contains(signalKey))
			{
				outSignalInfosMap.insert(signalKey,*signalInfo);
			}
			//chenxin add 2014.10.23 <---

			return true;
		}
	}
	return false;

}

bool  SCLModelAdapter::GetInfoOfDSSettingOutSignal(QString signalKey,QDSSettingSignalInfo *signalInfo)
	//获得输出信号的信息 
{
	if(SCLModelSvrAdapter!=NULL)
	{
		DSSettingSignalInfo info;
		if(SCLModelSvrAdapter->GetInfoOfDSSettingOutSignal(signalKey.toLocal8Bit().constData()
			,info))
		{
			QString cdc = info.cdc.c_str();
			signalInfo->cdc = GetCDCDesc(cdc);
			signalInfo->minVal = info.minVal.c_str();
			signalInfo->maxVal = info.maxVal.c_str();
			signalInfo->unit = info.unit.c_str();
			signalInfo->stepSize = info.stepSize.c_str();
			return true;
		}
	}
	return false;

}

/** 
  * @brief    GetCDCDesc
  * 
  * 根据CDC取得对应的中文描述
  * 
  * @class    SCLModelAdapter
  * @param    QString cdc :
  * @return   QString
  * @author   zhang-xch
  * @date     2015/1/23
  * @remarks  
  */ 
QString SCLModelAdapter::GetCDCDesc(QString cdc)
{
	QMap<QString, QString>::iterator it = m_CDCDescMap.find(cdc);
	if (it != m_CDCDescMap.end())
	{
		return it.value();
	}
	return cdc;
}


//高级访问接口
bool   SCLModelAdapter::GetIEDListByLogicalRealitionWithCurrentIED(QString IEDKey,QVector<QString>* associatedIEDKeys)    //获得IED的关联IED列表（逻辑）
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempAssociatedIEDKeys;
		SCLModelSvrAdapter->GetIEDListByLogicalRealitionWithCurrentIED(IEDKey.toLocal8Bit().constData(),tempAssociatedIEDKeys);
		for(uint i=0;i<tempAssociatedIEDKeys.size();i++)
		{
			associatedIEDKeys->append(tempAssociatedIEDKeys[i].c_str());
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetRealitionTypeOfSrcIEDAndDestIED(QString srcIEDKey,QString destIEDKey,int *type)     //获得IED之间的关联类型（0：源IED为输入）
																																			//1: 源IED为输出
																																			//2: 源IED既是输入又是输出
																																			//-1:源IED和目的IED没有关系）
{
	if(SCLModelSvrAdapter!=NULL)
	{
		SCLModelSvrAdapter->GetRealitionTypeOfSrcIEDAndDestIED(srcIEDKey.toLocal8Bit().constData(),destIEDKey.toLocal8Bit().constData(),*type);
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetIEDListByPhysicalRealitionWithCurrentIED(QString IEDKey,QVector<QString>* associatedIEDKeys)   //获得IED的关联IED列表（物理） 
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempassociatedIEDKeys;
		SCLModelSvrAdapter->GetIEDListByPhysicalRealitionWithCurrentIED(IEDKey.toLocal8Bit().constData(),tempassociatedIEDKeys);
		for(uint i=0;i<tempassociatedIEDKeys.size();i++)
		{
			associatedIEDKeys->append(tempassociatedIEDKeys[i].c_str());
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetPhysicsLinkAssociatedIED(QString srcIEDKey,QString destIEDKey,QVector<QPhysLink>* links)       //获得IED与IED之间物理链接（多个链接表示多个物理通道，0个链接表示无直接连接）
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<PhysLinkInfo> tempLinks;
		SCLModelSvrAdapter->GetPhysicsLinkAssociatedIED(srcIEDKey.toLocal8Bit().constData()
			,destIEDKey.toLocal8Bit().constData()
			,tempLinks);
		for(uint i=0;i<tempLinks.size();i++)
		{
			QPhysLink tempPhyLink;
			tempPhyLink.srcPort = tempLinks[i].srcPort.c_str();
			tempPhyLink.destPort = tempLinks[i].destPort.c_str();
			links->append(tempPhyLink);
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetLinksBetweenPort(QString srcPortKey,QString destPortKey,QVector<QPhysLink>* links)             //获得原port与目的port之间的物理链接（多个链接表示之间有多个设备）
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<PhysLinkInfo> tempLinks;
		SCLModelSvrAdapter->GetLinksBetweenPort(srcPortKey.toLocal8Bit().constData()
			,destPortKey.toLocal8Bit().constData()
			,tempLinks);
		for(uint i=0;i<tempLinks.size();i++)
		{
			QPhysLink tempPhyLink;
			tempPhyLink.srcPort = tempLinks[i].srcPort.c_str();
			tempPhyLink.destPort = tempLinks[i].destPort.c_str();
			links->append(tempPhyLink);
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetVirtualSignalsAssociatedIED(QString srcIEDKey,QString destIEDKey,QSignalTypeEnum signalType,QVector<QVirtualLink>* links)         //获得IED与IED之间虚信号 
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<VirtualLinkInfo> tempLinks;
		SCLModelSvrAdapter->GetVirtualSignalsAssociatedIED(srcIEDKey.toLocal8Bit().constData()
			,destIEDKey.toLocal8Bit().constData()
			,(SignalTypeEnum)signalType
			,tempLinks);
		for(uint i=0;i<tempLinks.size();i++)
		{
			QVirtualLink tempVirLink;
			tempVirLink.iOrd = tempLinks[i].iOrd;
			tempVirLink.DiffState = 1;
			tempVirLink.srcSignal = tempLinks[i].srcSignal.c_str();
			tempVirLink.destSignal = tempLinks[i].destSignal.c_str();
			tempVirLink.inSigInfo = tempLinks[i].inSigDesc.c_str();
			links->append(tempVirLink);
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetPortsListOfPort(QString portKey,QVector<QString>* portKeys)                                    //获得与物理端口关联的其它物理端口
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempKeys;
		SCLModelSvrAdapter->GetPortsListOfPort(portKey.toLocal8Bit().constData(),tempKeys);
		for(uint i=0;i<tempKeys.size();i++)
		{
			portKeys->append(tempKeys[i].c_str());
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetPhysicsLinkeByPort(QString srcPortKey,QString destPortKey,QPhysLink* link)                      //根据两个端口获得物理链接
{
	if(SCLModelSvrAdapter!=NULL)
	{
		PhysLinkInfo tempLink;
		SCLModelSvrAdapter->GetPhysicsLinkeByPort(srcPortKey.toLocal8Bit().constData()
			,destPortKey.toLocal8Bit().constData()
			,tempLink);

		link->srcPort = tempLink.srcPort.c_str();
		link->destPort = tempLink.destPort.c_str();
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetInSignalsListOfOutSignal(QString outSignalKey,QVector<QString>* inSignalKeys)                  //获得输出信号关联的输入信号列表（两个装置之间）
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempKeys;
		SCLModelSvrAdapter->GetInSignalsListOfOutSignal(outSignalKey.toLocal8Bit().constData(),tempKeys);
		for(uint i=0;i<tempKeys.size();i++)
		{
			inSignalKeys->append(tempKeys[i].c_str());
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetOutSignalsListOfInSignal(QString inSignalKey,QVector<QString>* outSignalKeys)                  //获得输入信号关联的输出信号列表（两个装置之间） 
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempKeys;
		SCLModelSvrAdapter->GetOutSignalsListOfInSignal(inSignalKey.toLocal8Bit().constData(),tempKeys);
		for(uint i=0;i<tempKeys.size();i++)
		{
			outSignalKeys->append(tempKeys[i].c_str());
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetInternalOutSignalsListOfInSignal(QString inSignalKey,QVector<QString>* outSignalKeys)          //获得输入信号关联的输出信号列表（装置内部）
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempKeys;
		SCLModelSvrAdapter->GetInternalOutSignalsListOfInSignal(inSignalKey.toLocal8Bit().constData(),tempKeys);
		for(uint i=0;i<tempKeys.size();i++)
		{
			outSignalKeys->append(tempKeys[i].c_str());
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetInternalInSignalsListOfOutSignal(QString outSignalKey,QVector<QString>* inSignalKeys)          //获得输出信号关联的输入信号列表（装置内部）
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempKeys;
		SCLModelSvrAdapter->GetInternalInSignalsListOfOutSignal(outSignalKey.toLocal8Bit().constData(),tempKeys);
		for(uint i=0;i<tempKeys.size();i++)
		{
			inSignalKeys->append(tempKeys[i].c_str());
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetVirtualLinkBySignal(QString srcSignalKey,QString destSignalKey,QVirtualLink* link)              //根据两个信号获得虚拟链接   
{
	if(SCLModelSvrAdapter!=NULL)
	{
		VirtualLinkInfo tempLink;
		SCLModelSvrAdapter->GetVirtualLinkBySignal(srcSignalKey.toLocal8Bit().constData()
			,destSignalKey.toLocal8Bit().constData()
			,tempLink);

		link->srcSignal = tempLink.srcSignal.c_str();
		link->destSignal = tempLink.destSignal.c_str();
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetFunctionListOfInSignal(QString inSignalKey,QVector<QString>* funcKeys)                         //获得输入信号关联的功能列表（本装置功能）
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempKeys;
		SCLModelSvrAdapter->GetFunctionListOfInSignal(inSignalKey.toLocal8Bit().constData(),tempKeys);
		for(uint i=0;i<tempKeys.size();i++)
		{
			funcKeys->append(tempKeys[i].c_str());
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetFunctionsListOfOutSignal(QString outSignalKey,QVector<QString>* funcKeys)                      //获得输出信号关联的功能列表 （其它装置功能）
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempKeys;
		SCLModelSvrAdapter->GetFunctionsListOfOutSignal(outSignalKey.toLocal8Bit().constData(),tempKeys);
		for(uint i=0;i<tempKeys.size();i++)
		{
			funcKeys->append(tempKeys[i].c_str());
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetFunctionsListOfOutSignalWithDestIED(QString outSignalKey,QString destIEDKey,QVector<QString>* funcKeys) //获得输出信号相对于某装置的功能列表（某装置功能）  
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempKeys;
		SCLModelSvrAdapter->GetFunctionsListOfOutSignalWithDestIED(outSignalKey.toLocal8Bit().constData()
			,destIEDKey.toLocal8Bit().constData()
			,tempKeys);
		for(uint i=0;i<tempKeys.size();i++)
		{
			funcKeys->append(tempKeys[i].c_str());
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetFunctionsListOfPort(QString portKey,QVector<QString>* FuncKeys)                                //获得物理端口功能列表 
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempKeys;
		SCLModelSvrAdapter->GetFunctionsListOfPort(portKey.toLocal8Bit().constData(),tempKeys);
		for(uint i=0;i<tempKeys.size();i++)
		{
			FuncKeys->append(tempKeys[i].c_str());
		}
		return true;
	}
	return false;
}

bool   SCLModelAdapter::GetFunctionsListOfTrip(QString tripKey,QVector<QString>* FuncKeys)          //获得压板对应的功能列表     
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempKeys;
		SCLModelSvrAdapter->GetFunctionsListOfTrip(tripKey.toLocal8Bit().constData(),tempKeys);
		for(uint i=0;i<tempKeys.size();i++)
		{
			FuncKeys->append(tempKeys[i].c_str());
		}
		return true;
	}
	return false;
}

QString  SCLModelAdapter::GetFunctionDesc(QString functionKey)                                      //获得功能的功能描述
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->GetFunctionDesc(functionKey.toLocal8Bit().constData()).c_str();
	}
	return "";
}

QString  SCLModelAdapter::GetFunctionTripKey(QString functionKey)                                          //获得功能的功能压板（可能不存在）
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->GetFunctionTripKey(functionKey.toLocal8Bit().constData()).c_str();
	}
	return "";
} 

QString SCLModelAdapter::GetIEDTripKey(QString IEDKey)          //获得设备的压板（表示断电一类的操作） 
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->GetIEDTripKey(IEDKey.toLocal8Bit().constData()).c_str();
	}
	return "";

}

QString SCLModelAdapter::GetPortTripKey(QString portKey)             //获得端口的压板（表示拨光纤一类的操作） 
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->GetPortTripKey(portKey.toLocal8Bit().constData()).c_str();
	}
	return "";

}

QString SCLModelAdapter::GetSignalTripKey(QString signalKey)          //获得信号的压板（软压板）   
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->GetSignalTripKey(signalKey.toLocal8Bit().constData()).c_str();
	}
	return "";

}

bool SCLModelAdapter::GetSignalInfosByIEDKeyAndCBKey(const QString masterIEDKey, const QString slaveIEDKey, const QString CBkey, QList<QSignalInfo> *mSigInfos, QList<QSignalInfo> *sSigInfos)
{
	//获取装置间所有虚链接;
	QVector<QVirtualLink> vLinks, fVLinks;
	GetVirtualSignalsAssociatedIED(masterIEDKey, slaveIEDKey, typeAll_, &vLinks);
	
	//根据控制块过滤;
	QVector<QString> cbSigs;
	GetOutSignalsByCBKey(masterIEDKey, CBkey, &cbSigs);
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
			fVLinks.append(vLinks.at(i));
		}
	}

	//填充List;
	for (QVector<QVirtualLink>::Iterator j = fVLinks.begin();j != fVLinks.end(); ++j)
	{
		QVirtualLink a_vlink = *j;
		QString strSrcSignal = a_vlink.srcSignal;
		QString strDesSignal = a_vlink.destSignal;

		QSignalInfo srcSigInfo, desSigInfo;
		GetInfoOfOutSignal(strSrcSignal,&srcSigInfo);
		GetInfoOfInSignal(strDesSignal,&desSigInfo);
		mSigInfos->append(srcSigInfo);
		sSigInfos->append(desSigInfo);
	}

	//排序;
	int count = mSigInfos->size();
	if (count < 1)
	{
		return false;
	}
	int index = 0;
	for (int i = 0; i < count - 1; i++)
	{
		index = i;
		int sOrd = mSigInfos->at(i).iOrd;
		for (int j = i + 1; j < count; j++)
		{
			int dOrd = mSigInfos->at(j).iOrd;
			if (sOrd > dOrd)
			{
				index = j;
				sOrd = dOrd;
			}
		}
		if (index != i)
		{
			mSigInfos->swap(i,index);
			sSigInfos->swap(i,index);
		}
	}
	return true;
}

bool SCLModelAdapter::GetSignalInfosByIEDKey(const QString IEDKey, QList<QSignalInfo> *mSigInfos, QList<QSignalInfo> *sSigInfos)
{
	QVector<QVirtualLink> vLinks;
	GetVritualLinksOfIED(IEDKey,typeAll_,&vLinks);
	for (QVector<QVirtualLink>::Iterator j = vLinks.begin();j != vLinks.end(); ++j)
	{
		QVirtualLink a_vlink = *j;
		QString strSrcSignal = a_vlink.srcSignal;
		QString srcIED = GetParentKey(strSrcSignal);
		QSignalInfo signal_info;
		bool retIn  = false;
		bool retOut = false;
		if(IEDKey == srcIED)//本装置为输出;
		{
			//处理输出信号;
			retOut = GetInfoOfOutSignal(strSrcSignal,&signal_info);
			if (retOut)
			{
				mSigInfos->push_back(signal_info);

				//处理输入信号;
				QString strDestSignal = a_vlink.destSignal;
				bool ret = GetInfoOfInSignal(strDestSignal,&signal_info);
				if (ret)
				{
					sSigInfos->push_back(signal_info);
				}
			}
		}
		else//对端装置为输出;
		{
			retOut = GetInfoOfOutSignal(strSrcSignal,&signal_info);
			if (retOut)
			{
				mSigInfos->push_back(signal_info);

				QString strDestSignal = a_vlink.destSignal;
				bool ret = GetInfoOfInSignal(strDestSignal,&signal_info);
				if (ret)
				{
					sSigInfos->push_back(signal_info);
				}
			}
		}
	}

	//排序;
	int count  = mSigInfos->size();
	if (count < 1)
	{
		return false;
	}
	int index = 0;
	for (int i = 0; i < count - 1; i++)
	{
		index = i;
		int sOrd = mSigInfos->at(i).iOrd;
		for (int j = i + 1; j < count; j++)
		{
			int dOrd = mSigInfos->at(j).iOrd;
			if (sOrd > dOrd)
			{
				index = j;
				sOrd = dOrd;
			}
		}
		if (index != i)
		{
			mSigInfos->swap(i,index);
			sSigInfos->swap(i,index);
		}
	}

	return true;
}

bool SCLModelAdapter::GetCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList)
{
	if (SCLModelSvrAdapter!=NULL)
	{
        OutputDebugStringA(IEDKey.toStdString().c_str());
        OutputDebugStringA("\n SCLModelAdapter::GetCtrlBlockListByIED \n");
		vector<string> cbList;
		bool isSuc = SCLModelSvrAdapter->GetCtrlBlockListByIED(IEDKey.toLocal8Bit().constData(), cbList);
		for (int i = 0; i != cbList.size(); i++)
		{
			ctrlBlockList->append(cbList.at(i).c_str());
		}
		return isSuc;
	}
	return false;
}

bool SCLModelAdapter::GetDsSettingListByIED(QString IEDKey, QVector<QDataSetInfo>* dsSettingList)
{
	if (SCLModelSvrAdapter!=NULL)
	{
		vector<DataSetInfo> cbList;
		bool isSuc = SCLModelSvrAdapter->GetProtectionSettingList(IEDKey.toLocal8Bit().constData(), cbList);
		for (int i = 0; i != cbList.size(); i++)
		{
			QDataSetInfo temp;
			temp.IEDName = cbList[i].IEDName.c_str();
			temp.ApName = cbList[i].ApName.c_str();
			temp.LDInst = cbList[i].LDInst.c_str();
			temp.dataSetName = cbList[i].dataSetName.c_str();
			temp.dataSetDesc = cbList[i].dataSetDesc.c_str();

			dsSettingList->append(temp);
		}
		return isSuc;
	}
	return false;
}


bool SCLModelAdapter::GetCtrlBlockInfo(QString ctrlBlockKey, QCtrlBlockInfo& cbInfo)
{
	if (SCLModelSvrAdapter!=NULL)
	{
		//chenxin add 2014.10.23 --->
		if(ctrlBlockInfosMap.contains(ctrlBlockKey))
		{
			QMap<QString,QCtrlBlockInfo>::iterator it = ctrlBlockInfosMap.find(ctrlBlockKey);
			cbInfo = it.value();
			return true;
		}
		//chenxin add 2014.10.23 <---

		CtrlBlockInfo ctrlBlockInfo;
		bool isSus = SCLModelSvrAdapter->GetCtrlBlockInfo(ctrlBlockKey.toLocal8Bit().constData(), ctrlBlockInfo);
		cbInfo.name = ctrlBlockInfo.name.c_str();
		cbInfo.type = ctrlBlockInfo.type.c_str();
		cbInfo.apAPPID = ctrlBlockInfo.apAPPID.c_str();
		cbInfo.apMAC = ctrlBlockInfo.apMAC.c_str();
		cbInfo.apName = ctrlBlockInfo.apName.c_str();
		cbInfo.APPID = ctrlBlockInfo.APPID.c_str();
		cbInfo.apVLAN_ID = ctrlBlockInfo.apVLAN_ID.c_str();
		cbInfo.apVLAN_PRIORITY = ctrlBlockInfo.apVLAN_PRIORITY.c_str();
		cbInfo.dataSetName = ctrlBlockInfo.dataSetName.c_str();
		cbInfo.dataSetAddr = ctrlBlockInfo.dataSetAddr.c_str();
		cbInfo.ASDU = ctrlBlockInfo.ASDU;
		cbInfo.confRev = ctrlBlockInfo.confRev.c_str();
		cbInfo.gocbRef = ctrlBlockInfo.gocbRef.c_str();
		cbInfo.smpRate = ctrlBlockInfo.smpRate.c_str();
		cbInfo.maxTime = ctrlBlockInfo.maxTime.c_str();
		cbInfo.minTime = ctrlBlockInfo.minTime.c_str();

		cbInfo.LDInst = ctrlBlockInfo.LDInst.c_str();
		cbInfo.desc = ctrlBlockInfo.desc.c_str();
		cbInfo.intgPd = ctrlBlockInfo.intgPd.c_str();
		cbInfo.Buffered = ctrlBlockInfo.Buffered.c_str();
		cbInfo.bufTime = ctrlBlockInfo.bufTime.c_str();

		//chenxin add 2014.10.23 --->
		if(!ctrlBlockInfosMap.contains(ctrlBlockKey))
		{
			ctrlBlockInfosMap.insert(ctrlBlockKey,cbInfo);
		}
		//chenxin add 2014.10.23 <---


		return isSus;
	}
	return false;
}

bool SCLModelAdapter::GetDataSetSignalsByCtrlBlockID(QString cbKey, QVector<QString>* dsSignalList)
{
	if (SCLModelSvrAdapter!=NULL)
	{
		vector<string> dsSigs;
		bool isSus = SCLModelSvrAdapter->GetDataSetSignalsByCtrlBlockID(cbKey.toLocal8Bit().constData(), dsSigs);
		for (int i = 0; i != dsSigs.size(); i++)
		{
			dsSignalList->append(dsSigs.at(i).c_str());
		}
		return isSus;
	}
	return false;
}
bool SCLModelAdapter::GetSlaveCtrlBlockByIEDKeys(QString masterKey, QString slaveKey, QVector<QString>* ctrlBlockList)
{
	if (SCLModelSvrAdapter!=NULL)
	{
		vector<string> cbList;
		bool isSus = SCLModelSvrAdapter->GetSlaveCtrlBlockByIEDKeys(masterKey.toLocal8Bit().constData(), slaveKey.toLocal8Bit().constData(), cbList);
		for (int i = 0; i != cbList.size(); i++)
		{
			ctrlBlockList->append(cbList.at(i).c_str());
		}
		return isSus;
	}
	return false;
}

bool SCLModelAdapter::GetOutSignalsByCBKey(QString masterIEDKey, QString cbKey, QVector<QString>* outSignals)
{
	if (SCLModelSvrAdapter!=NULL)
	{
		vector<string> ouSigs;
		bool isSus = SCLModelSvrAdapter->GetOutSignalsByCBKey(masterIEDKey.toLocal8Bit().constData(), cbKey.toLocal8Bit().constData(), ouSigs);
		for (int i = 0; i != ouSigs.size(); i++)
		{
			outSignals->append(ouSigs.at(i).c_str());
		}
		return isSus;
	}
	return false;
}

bool SCLModelAdapter::GetOutSignalsByDataSetKey(QString masterIEDKey, QString dataSetKey, QVector<QString>* outSignals)
{
	if (SCLModelSvrAdapter!=NULL)
	{
		vector<string> ouSigs;
		bool isSus = SCLModelSvrAdapter->GetProtectionSettingSignalList(
			masterIEDKey.toLocal8Bit().constData(),  dataSetKey.toLocal8Bit().constData(), ouSigs);
		for (int i = 0; i != ouSigs.size(); i++)
		{
			outSignals->append(ouSigs.at(i).c_str());
		}
		return isSus;
	}
	return false;
}


bool SCLModelAdapter::GetAssoIEDListByCBKey(QString cbKey, QVector<QString>* assoIEDKeys)		//通过控制块获取对端IED;
{
	if (SCLModelSvrAdapter!=NULL)
	{
		vector<string> aIEDKeys;
		bool isSus = SCLModelSvrAdapter->GetAssoIEDListByCBKey(cbKey.toLocal8Bit().constData(), aIEDKeys);
		for (int i = 0; i != aIEDKeys.size(); i++)
		{
			assoIEDKeys->append(aIEDKeys.at(i).c_str());
		}
		return isSus;
	}
	return false;
}

int SCLModelAdapter::GetItemChangeType(QString ItemName)
{
	return 0;
}

int SCLModelAdapter::GetVLinkChangeType(QString masterIEDName, QString slaveIEDName, QString cbName)
{
	return 0;
}

bool SCLModelAdapter::GetPhyPortInfo(QString pPortKey, QPhyPortInfo& pPortInfo)
{
	if (SCLModelSvrAdapter!=NULL)
	{
		PhyPortInfo pInfo;
		bool isSus = SCLModelSvrAdapter->GetPhyPortInfo(pPortKey.toLocal8Bit().constData(), pInfo);
		pPortInfo.pIndex = pInfo.portName.c_str();
		//pPortInfo.desc = pInfo.desc.c_str();
		pPortInfo.IEDName = pInfo.IEDName.c_str();
		pPortInfo.plug = pInfo.plug.c_str();
		pPortInfo.type = pInfo.portType.c_str();
		pPortInfo.ID = pPortKey;
		pPortInfo.transMedia = pInfo.transMedia.c_str();
		pPortInfo.cable = pInfo.cable.c_str();
		//pPortInfo.rtType = pInfo.rtType.c_str();
		return isSus;
	}
	return false;
}

bool SCLModelAdapter::GetPhyPortListByIED(QString IEDKey, QVector<QString>* pPortList)
{
	if (SCLModelSvrAdapter!=NULL)
	{
		vector<string> pList;
		bool isSus = SCLModelSvrAdapter->GetPhyPortListByIED(IEDKey.toLocal8Bit().constData(), pList);
		for (int i = 0; i != pList.size(); i++)
		{
			pPortList->append(IEDKey + "." +pList.at(i).c_str());
		}
		return isSus;
	}
	return false;
}

bool SCLModelAdapter::GetPhyPortListByInSignalID(QString signalKey, QVector<QString> &pPortList)
{
	if (SCLModelSvrAdapter!=NULL)
	{
		vector<string> pKeyList;
		bool isSus = SCLModelSvrAdapter->GetPhyPortListByInSignalID(signalKey.toLocal8Bit().constData(), pKeyList);
		for (uint i = 0; i != pKeyList.size(); i++)
		{
			pPortList.append(pKeyList.at(i).c_str());
		}
		return isSus;
	}
	return false;
}

bool SCLModelAdapter::GetPhyPortKeyByOutSignalID(QString signalKey, QString* pPortKey)
{
	if (SCLModelSvrAdapter!=NULL)
	{
		string pKey = "";
		bool isSus = SCLModelSvrAdapter->GetPhyPortKeyByOutSignalID(signalKey.toLocal8Bit().constData(), pKey);
		*pPortKey = pKey.c_str();
		return isSus;
	}
	return false;
}

//模型编辑接口;
bool   SCLModelAdapter::AddVoltageLevel(QString voltageLevelName)                                         //增加电压等级;
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->AddVoltageLevel(voltageLevelName.toLocal8Bit().constData());
	}
	return false;
}

bool   SCLModelAdapter::DeleteVoltageLevel(QString voltageLevelName)                                      //删除电压等级
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->DeleteVoltageLevel(voltageLevelName.toLocal8Bit().constData());
	}
	return false;
}

bool   SCLModelAdapter::AddSubstation(QString voltageLevelKey,QString subName/*,QByteArray& fileData*/)        //增加变电站模型
{
	qDebug()<<::GetCurrentProcessId()<<"CMyLoadingThread::run	SCLModelAdapter::AddSubstation	1";
	if(SCLModelSvrAdapter!=NULL)
	{
		qDebug()<<::GetCurrentProcessId()<<"CMyLoadingThread::run	SCLModelAdapter::AddSubstation	2";
		if (SCLModelSvrAdapter->AddSubstation(voltageLevelKey.toLocal8Bit().constData() ,subName.toLocal8Bit().constData()))
		{
			qDebug()<<::GetCurrentProcessId()<<"CMyLoadingThread::run	SCLModelAdapter::AddSubstation	3";
			return true;
		}
	}
	qDebug()<<::GetCurrentProcessId()<<"CMyLoadingThread::run	SCLModelAdapter::AddSubstation	1	false";
	return false;
}

int   SCLModelAdapter::GetSubstationAddedProgress()
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->GetSubstationAddedProgress();
	}
	return 0;
}

bool   SCLModelAdapter::DeleteSubstation(QString substationKey)                                           //删除变电站模型
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->DeleteSubstation(substationKey.toLocal8Bit().constData());
	}
	return false;
}

bool   SCLModelAdapter::UpdateSubstationModel(QString voltageLevelKey,QString subName,QString SCDFilePath)//更新变电站模型
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->UpdateSubstationModel(voltageLevelKey.toLocal8Bit().constData()
			,subName.toLocal8Bit().constData()
			,SCDFilePath.toLocal8Bit().constData());
	}
	return false;
}

bool   SCLModelAdapter::AddPanelForSubstation(QString subKey,QString paneName)                            //为变电站增加屏柜
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->AddPanelForSubstation(subKey.toLocal8Bit().constData()
			,paneName.toLocal8Bit().constData());
	}
	return false;
}

bool   SCLModelAdapter::DeletePanelForSubstation(QString PanelKey)                                        //为变电站删除屏柜 
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->DeletePanelForSubstation(PanelKey.toLocal8Bit().constData());
	}
	return false;
}

bool   SCLModelAdapter::AddIEDForPanel(QString panelKey,QString IEDName)                                  //为屏柜增加装置
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->AddIEDForPanel(panelKey.toLocal8Bit().constData()
			,IEDName.toLocal8Bit().constData());
	}
	return false;
}

bool   SCLModelAdapter::DeleteIEDForPanel(QString IEDKey)                                                 //为屏柜删除装置  
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->DeleteIEDForPanel(IEDKey.toLocal8Bit().constData());
	}
	return false;
}

bool   SCLModelAdapter::AddPortForIED(QString IEDKey,QString portName)                                    //为装置增加物理端口
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->AddPortForIED(IEDKey.toLocal8Bit().constData(),portName.toLocal8Bit().constData());
	}
	return false;
}

bool   SCLModelAdapter::DeletePortForIED(QString portKey)                                                 //为装置删除物理端口
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->DeletePortForIED(portKey.toLocal8Bit().constData());
	}
	return false;
}

bool   SCLModelAdapter::AddInSignalForIED(QString IEDKey,QString inSignalName)                            //为装置增加输入信号
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->AddInSignalForIED(IEDKey.toLocal8Bit().constData()
			,inSignalName.toLocal8Bit().constData());
	}
	return false;
}

bool   SCLModelAdapter::DeleteInSignalForIED(QString inSignalKey)                                         //为装置删除输入信号 
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->DeleteInSignalForIED(inSignalKey.toLocal8Bit().constData());
	}
	return false;
}

bool   SCLModelAdapter::AddOutSignalForIED(QString IEDKey,QString outSignalName)                          //为装置增加输出信号
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->AddOutSignalForIED(IEDKey.toLocal8Bit().constData()
			,outSignalName.toLocal8Bit().constData());
	}
	return false;
}

bool   SCLModelAdapter::DeleteOutSignalForIED(QString outSignalKey)                                       //为装置删除输出信号
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->DeleteOutSignalForIED(outSignalKey.toLocal8Bit().constData());
	}
	return false;
}

bool   SCLModelAdapter::AddFunction(QString IEDKey,QString funName,QString funcDesc)                      //为装置增加功能 
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->AddFunction(IEDKey.toLocal8Bit().constData()
			,funName.toLocal8Bit().constData()
			,funcDesc.toLocal8Bit().constData());
	}
	return false;
}

bool   SCLModelAdapter::ModifyFunction(QString funKey,QString funcDesc)                                   //为装置修改功能描述  
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->ModifyFunction(funKey.toLocal8Bit().constData()
			,funcDesc.toLocal8Bit().constData());
	}
	return false;
}

bool   SCLModelAdapter::DeleteFunction(QString funKey)                                                    //为装置删除功能 
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->DeleteFunction(funKey.toLocal8Bit().constData());
	}
	return false;
}

bool   SCLModelAdapter::SetDescForPort(QString portKey,QString desc)                                      //设置物理端口的描述 
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->SetDescForPort(portKey.toLocal8Bit().constData()
			,desc.toLocal8Bit().constData());
	}
	return false;
}

bool   SCLModelAdapter::SetInSignalsListOfOutSignal(QString outSignalKey,QVector<QString>* inSignalKeys)          //设置输出信号关联的输入信号列表（两个装置之间） 
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempInSignalKeys;
		for(int i=0;i<inSignalKeys->size();i++)
		{
			tempInSignalKeys.push_back(inSignalKeys->at(i).toLocal8Bit().constData());
		}
		return SCLModelSvrAdapter->SetInSignalsListOfOutSignal(outSignalKey.toLocal8Bit().constData(),tempInSignalKeys);
	}
	return false;

}

bool   SCLModelAdapter::SetOutSignalsListOfInSignal(QString inSignalKey,QVector<QString>* outSignalKeys)          //设置输入信号关联的输出信号列表（两个装置之间）
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempOutSignalKeys;
		for(int i=0;i<outSignalKeys->size();i++)
		{
			tempOutSignalKeys.push_back(outSignalKeys->at(i).toLocal8Bit().constData());
		}
		return SCLModelSvrAdapter->SetOutSignalsListOfInSignal(inSignalKey.toLocal8Bit().constData(),tempOutSignalKeys);
	}
	return false;
}

bool   SCLModelAdapter::SetInternalOutSignalsListOfInSignal(QString inSignalKey,QVector<QString>* outSignalKeys)  //设置输入信号关联的输出信号列表（装置内部）
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempOutSignalKeys;
		for(int i=0;i<outSignalKeys->size();i++)
		{
			tempOutSignalKeys.push_back(outSignalKeys->at(i).toLocal8Bit().constData());
		}
		return SCLModelSvrAdapter->SetInternalOutSignalsListOfInSignal(inSignalKey.toLocal8Bit().constData(),tempOutSignalKeys);
	}
	return false;
}

bool   SCLModelAdapter::SetInternalInSignalsListOfOutSignal(QString outSignalKey,QVector<QString>* inSignalKeys)  //设置输出信号关联的输入信号列表（装置内部）
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempInSignalKeys;
		for(int i=0;i<inSignalKeys->size();i++)
		{
			tempInSignalKeys.push_back(inSignalKeys->at(i).toLocal8Bit().constData());
		}
		return SCLModelSvrAdapter->SetInternalInSignalsListOfOutSignal(outSignalKey.toLocal8Bit().constData(),tempInSignalKeys);
	}
	return false;
}


bool   SCLModelAdapter::AddFunctionOfInSignal(QString inSignalKey,QString funKey)                         //为输入信号增加功能关联  
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->AddFunctionOfInSignal(inSignalKey.toLocal8Bit().constData()
			,funKey.toLocal8Bit().constData());
	}
	return false;
}

bool   SCLModelAdapter::DeleteFunctionOfInSignal(QString inSignalKey,QVector<QString>* funcKeys)          //为输入信号删除功能关联 
{
	if(SCLModelSvrAdapter!=NULL)
	{
		vector<string> tempFuncKeys;
		for(int i=0;i<funcKeys->size();i++)
		{
			tempFuncKeys.push_back(funcKeys->at(i).toLocal8Bit().constData());
		}
		return SCLModelSvrAdapter->DeleteFunctionOfInSignal(inSignalKey.toLocal8Bit().constData(),tempFuncKeys);
	}
	return false;
}    

bool  SCLModelAdapter::AddTripForIED(QString IEDKey,QString tripName) //增加压板 
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->AddTripForIED(IEDKey.toLocal8Bit().constData()
			,tripName.toLocal8Bit().constData());
	}
	return false;
}

bool  SCLModelAdapter::DeleteTripForIED(QString tripKey)//删除压板  
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->DeleteTripForIED(tripKey.toLocal8Bit().constData());
	}
	return false;
}

bool  SCLModelAdapter::SetFunctionTripKey(QString functionKey,QString tripKey)//设置功能的功能压板（可能不存在） 
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->SetFunctionTripKey(functionKey.toLocal8Bit().constData(),
			tripKey.toLocal8Bit().constData());
	}
	return false;
}

bool  SCLModelAdapter::SetIEDTripKey(QString IEDKey,QString tripKey)     //设置设备的压板（表示断电一类的操作）
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->SetIEDTripKey(IEDKey.toLocal8Bit().constData()
			,tripKey.toLocal8Bit().constData());
	}
	return false;
}

bool  SCLModelAdapter::SetPortTripKey(QString portKey,QString tripKey)   //设置端口的压板（表示拨光纤一类的操作）
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->SetPortTripKey(portKey.toLocal8Bit().constData()
			,tripKey.toLocal8Bit().constData());
	}
	return false;
}

bool  SCLModelAdapter::SetSignalTripKey(QString signalKey,QString tripKey)  //设置信号的压板（软压板）
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->SetSignalTripKey(signalKey.toLocal8Bit().constData()
			,tripKey.toLocal8Bit().constData());
	}
	return false;
}

//压板状态
bool   SCLModelAdapter::GetSoftTripState(QString tripKey,bool &state)                //获得压板状态
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->GetSoftTripState(tripKey.toLocal8Bit().constData(),state);
	}
	return false;
}

bool   SCLModelAdapter::SetSoftTripState(QString tripKey,bool state)     //设置压板状态
{
	if(SCLModelSvrAdapter!=NULL)
	{
		return SCLModelSvrAdapter->SetSoftTripState(tripKey.toLocal8Bit().constData(),state);
	}
	return false;
}

// 取得子网下IED的AP列表
bool SCLModelAdapter::GetAPListOfIEDFromSubnet(const QString& subNetKey, const QString IEDName, QVector<QString> &APList)
{
	if (SCLModelSvrAdapter != NULL)
	{
		QString substationID = GetParentKey(subNetKey);
		QString subNetName = GetNameFromKey(subNetKey);
		vector<string> tempAPList;
		bool isSus = SCLModelSvrAdapter->GetAPListOfIEDFromSubnet(substationID.toLocal8Bit().constData(), subNetName.toLocal8Bit().constData(), 
			IEDName.toLocal8Bit().constData(),tempAPList);

		for (uint i = 0; i != tempAPList.size(); i++)
		{
			APList.append(tempAPList.at(i).c_str());
		}
		return isSus;
	}
	return false;
}

// 取得子网下IED某个AP包含的物理端口列表
bool SCLModelAdapter::GetPortListOfIEDAPFromSubNet(const QString& subNetKey, const QString IEDName, const QString APName, QVector<QString> &PortList)
{
	if (SCLModelSvrAdapter != NULL)
	{
		QString substationID = GetParentKey(subNetKey);
		QString subNetName = GetNameFromKey(subNetKey);
		vector<string> tempPortList;
		bool isSus = SCLModelSvrAdapter->GetPortListOfIEDAPFromSubNet(substationID.toLocal8Bit().constData(), subNetName.toLocal8Bit().constData(), 
			IEDName.toLocal8Bit().constData(),APName.toLocal8Bit().constData(),tempPortList);
		for (uint i = 0; i != tempPortList.size(); i++)
		{
			PortList.append(tempPortList.at(i).c_str());
		}
		return isSus;
	}
	return false;
}

// 取得IED下AP的列表
bool SCLModelAdapter::GetAPListFromIED(const QString& IEDKey, QVector<QString> &APList)
{
	if (SCLModelSvrAdapter != NULL)
	{
		QString substationID = GetParentKey(IEDKey);
		QString IEDName = GetNameFromKey(IEDKey);
		vector<string> tempAPList;
		bool isSus = SCLModelSvrAdapter->GetAPListFromIED(substationID.toLocal8Bit().constData(), IEDName.toLocal8Bit().constData(), tempAPList);
		for (uint i = 0; i != tempAPList.size(); i++)
		{
			APList.append(tempAPList.at(i).c_str());
		}
		return isSus;
	}
	return false;
}

// 取得IED下某个AP对应的物理端口列表
bool SCLModelAdapter::GetAPPortListFormIED(const QString& IEDKey, const QString &APName, QVector<QString> &PortList)
{
	if (SCLModelSvrAdapter != NULL)
	{
		QString substationID = GetParentKey(IEDKey);
		QString IEDName = GetNameFromKey(IEDKey);
		vector<string> tempPortList;
		bool isSus = SCLModelSvrAdapter->GetAPPortListFormIED(substationID.toLocal8Bit().constData(), IEDName.toLocal8Bit().constData(), APName.toLocal8Bit().constData(), tempPortList);
		for (uint i = 0; i != tempPortList.size(); i++)
		{
			PortList.append(tempPortList.at(i).c_str());
		}
		return isSus;
	}
	return false;
}


// 获取SSD==================================================================================================

bool SCLModelAdapter::GetSSDInfoFromSCD(const QString subStationKey, SubstationCache &ssdInfo)
{
    if (SCLModelSvrAdapter != NULL)
    {
        return SCLModelSvrAdapter->GetSSDInfo(subStationKey.toLocal8Bit().constData(), ssdInfo);
    }
    return false;
}




