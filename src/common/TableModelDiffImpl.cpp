#include "TableModelDiffImpl.h"
#include <QStringList>

/** 
  * @brief    TableModelDiffImpl
  * 
  * 构造函数
  * 
  * @class    TableModelDiffImpl
  * @return   
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
TableModelDiffImpl::TableModelDiffImpl()
{
	m_ScdDiffCompare = SCDDiffCompare::GetInstance();
}

/** 
  * @brief    ~TableModelDiffImpl
  * 
  * 析构函数
  * 
  * @class    TableModelDiffImpl
  * @return   
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
TableModelDiffImpl::~TableModelDiffImpl()
{

}

/** 
  * @brief    GetBaseFileInfo
  * 
  * 获得基准文件的信息
  * 
  * @class    TableModelDiffImpl
  * @param    FileInfoStru& FileInfo :	保存基准文件的信息
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
bool TableModelDiffImpl::GetBaseFileInfo(FileInfoStru& FileInfo)
{
	FileInfoStru *pFileInfo = &m_ScdDiffCompare->m_SCDDiffResult.sFileInfo;
	FileInfo.state = pFileInfo->state;
	FileInfo.baseSubstationName = pFileInfo->baseSubstationName;
	FileInfo.basefileName = pFileInfo->basefileName;
	FileInfo.basefileTime = pFileInfo->basefileTime;
	FileInfo.basefileKey = m_ScdDiffCompare->m_BaseSubStationKey;
	FileInfo.crc = pFileInfo->crc;
	FileInfo.version.baseAttributeName = pFileInfo->version.baseAttributeName;
	FileInfo.reVersion.baseAttributeName = pFileInfo->reVersion.baseAttributeName;
	return true;
}
	
/** 
  * @brief    GetOtherFileInfo
  * 
  * 获得被比较文件信息
  * 
  * @class    TableModelDiffImpl
  * @param    FileInfoStru& FileInfo : 保存比较文件的信息
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
bool TableModelDiffImpl::GetOtherFileInfo(FileInfoStru& FileInfo)
{
	FileInfoStru *pFileInfo = &m_ScdDiffCompare->m_SCDDiffResult.sFileInfo;
	FileInfo.state = pFileInfo->state;
	FileInfo.otherSubstationName = pFileInfo->otherSubstationName;
	FileInfo.otherfileName = pFileInfo->otherfileName;
	FileInfo.otherfileTime = pFileInfo->otherfileTime;
	FileInfo.otherfileKey = m_ScdDiffCompare->m_OtherSubStationKey;
	FileInfo.crc = pFileInfo->crc;
	FileInfo.version.otherAttributeName = pFileInfo->version.otherAttributeName;
	FileInfo.reVersion.otherAttributeName = pFileInfo->reVersion.otherAttributeName;
	return true;
}

/** 
  * @brief    GetIEDList
  * 
  * 获得所有装置ID
  * 
  * @class    TableModelDiffImpl
  * @param    QStringList& IEDKeys : 保存装置的ID
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
bool TableModelDiffImpl::GetIEDList(QString panelKey, QVector<QString>* IEDKeys)
{
	for (uint i = 0; i < m_ScdDiffCompare->m_SCDDiffResult.vIEDStru.size(); i++)
	{
		IEDStru *pIEDStru = &m_ScdDiffCompare->m_SCDDiffResult.vIEDStru.at(i);
		IEDKeys->append(pIEDStru->ID);
	}
	return true;
}

/** 
  * @brief    GetIEDInfo
  * 
  * 获得装置信息
  * 
  * @class    TableModelDiffImpl
  * @param    QString IEDKey    : 装置的Key
  * @param    IEDStru& IEDInfo  : 保存装置信息
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
bool TableModelDiffImpl::GetIEDInfo(QString IEDKey, IEDStru& IEDInfo)
{
	DifferMap::iterator it = m_ScdDiffCompare->m_IEDDiffMap.find(IEDKey);
	if (it != m_ScdDiffCompare->m_IEDDiffMap.end())
	{
		IEDStru *pIED = (IEDStru *)(it->second);
		IEDInfo.ID = pIED->ID;
		IEDInfo.state = pIED->state;
		IEDInfo.IEDName = pIED->IEDName;
		
		SetAttrValueFromBasicStru(IEDInfo.IEDCRC, pIED->IEDCRC);
		SetAttrValueFromBasicStru(IEDInfo.IEDType, pIED->IEDType);
		SetAttrValueFromBasicStru(IEDInfo.IEDDesc, pIED->IEDDesc);
		SetAttrValueFromBasicStru(IEDInfo.IEDManu, pIED->IEDManu);

	}
	return true;
}
  
/** 
  * @brief    GetInfoOfInSignal
  * 
  * 获得输入信号的信息
  * 
  * @class    TableModelDiffImpl
  * @param    QString signalKey      : 输入信号Key
  * @param    SignalStru *signalInfo : 输入信号的信息
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
bool TableModelDiffImpl::GetInfoOfInSignal(QString signalKey, SignalStru *signalInfo)
{
	DifferMap::iterator it = m_ScdDiffCompare->m_InSigDiffMap.find(signalKey);
	if (it != m_ScdDiffCompare->m_InSigDiffMap.end())
	{
		*signalInfo = *((SignalStru *)(it->second));
		return true;
	}
	return false;
}

/** 
  * @brief    GetInfoOfOutSignal
  * 
  * 获得输出信号的信息
  * 
  * @class    TableModelDiffImpl
  * @param    QString signalKey      : 输出信号的key
  * @param    SignalStru *signalInfo : 输出信号的信息
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
bool TableModelDiffImpl::GetInfoOfOutSignal(QString signalKey,SignalStru *signalInfo) 
{
	DifferMap::iterator it = m_ScdDiffCompare->m_OutSigDiffMap.find(signalKey);
	if (it != m_ScdDiffCompare->m_OutSigDiffMap.end())
	{
		*signalInfo = *((SignalStru *)(it->second));
		return true;
	}
	return false;
}

/** 
  * @brief    GetGooseCtrlBlockListByIED
  * 
  * 获取当前IED的GOOSE控制块列表
  * 
  * @class    TableModelDiffImpl
  * @param    QString IEDKey                   : 当前IED的Key
  * @param    QVector<QString>* ctrlBlockList  : IED的Goose控制块列表
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
bool TableModelDiffImpl::GetGooseCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList)
{
	DifferMap::iterator it = m_ScdDiffCompare->m_IEDDiffMap.find(IEDKey);
	if (it != m_ScdDiffCompare->m_IEDDiffMap.end())
	{
		IEDStru *pIED = (IEDStru*)(it->second);
		for (uint j = 0; j < pIED->vGOOSECB.size(); j++)
		{
			ctrlBlockList->push_back(pIED->vGOOSECB.at(j).ID);
		}
		return true;
	}
	return false;
}

/** 
  * @brief    GetSVCtrlBlockListByIED
  * 
  * 获取当前IED的SV控制块列表
  * 
  * @class    TableModelDiffImpl
  * @param    QString IEDKey                   : 当前IED的Key
  * @param    QVector<QString>* ctrlBlockList  : 当前IED的SV控制块列表
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
bool TableModelDiffImpl::GetSVCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList)
{
	DifferMap::iterator it = m_ScdDiffCompare->m_IEDDiffMap.find(IEDKey);
	if (it != m_ScdDiffCompare->m_IEDDiffMap.end())
	{
		IEDStru *pIED = (IEDStru*)(it->second);
		for (uint j = 0; j < pIED->vSVCB.size(); j++)
		{
			ctrlBlockList->push_back(pIED->vSVCB.at(j).ID);
		}
		return true;
	}
	return false;
}

int TableModelDiffImpl::GetSigCountByIED(QString IEDKey, bool IsInSig, QSignalTypeEnum sigType)
{
	return 0;
}

/** 
  * @brief    GetCtrlBlockInfo
  * 
  * 获取SV控制块信息
  * 
  * @class    TableModelDiffImpl
  * @param    QString ctrlBlockKey : 当前控制块的Key
  * @param    SVCBStru& cbInfo     : SV控制块的信息
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
bool TableModelDiffImpl::GetCtrlBlockInfo(QString ctrlBlockKey, SVCBStru& cbInfo)
{
	DifferMap::iterator it = m_ScdDiffCompare->m_CBDiffMap.find(ctrlBlockKey);
	if (it != m_ScdDiffCompare->m_CBDiffMap.end())
	{
		SVCBStru *pSV = (SVCBStru *)(it->second);
		SetAttrValueFromBasicStru(cbInfo.apAppID, pSV->apAppID);
		SetAttrValueFromBasicStru(cbInfo.AppID, pSV->AppID);
		SetAttrValueFromBasicIntStru(cbInfo.ASDUCount, pSV->ASDUCount);
		SetAttrValueFromBasicStru(cbInfo.CBDesc, pSV->CBDesc);
		SetAttrValueFromBasicStru(cbInfo.CBName, pSV->CBName);
		SetAttrValueFromBasicStru(cbInfo.ConfVersion, pSV->ConfVersion);
		SetAttrValueFromBasicStru(cbInfo.DataSetAddr, pSV->DataSetAddr);
		SetAttrValueFromBasicIntStru(cbInfo.DataSetCount, pSV->DataSetCount);
		SetAttrValueFromBasicStru(cbInfo.DataSetDesc, pSV->DataSetDesc);
		SetAttrValueFromBasicStru(cbInfo.DataSetName, pSV->DataSetName);
		SetAttrValueFromBasicStru(cbInfo.MACAddr, pSV->MACAddr);
		SetAttrValueFromBasicStru(cbInfo.smpRate, pSV->smpRate);
		SetAttrValueFromBasicStru(cbInfo.VLANID, pSV->VLANID);
		SetAttrValueFromBasicStru(cbInfo.VLANPriority, pSV->VLANPriority);
		SetAttrValueFromBasicStru(cbInfo.VLink, pSV->VLink);
		cbInfo.state = pSV->state;
		cbInfo.ID = pSV->ID;
		return true;
	}
	return false;
}

/** 
  * @brief    GetCtrlBlockInfo
  * 
  * 获取Goose控制块信息
  * 
  * @class    TableModelDiffImpl
  * @param    QString ctrlBlockKey : 当前控制块的Key
  * @param    GOOSECBStru& cbInfo     : Goose控制块的信息
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
bool TableModelDiffImpl::GetCtrlBlockInfo(QString ctrlBlockKey, GOOSECBStru& cbInfo)
{
	DifferMap::iterator it = m_ScdDiffCompare->m_CBDiffMap.find(ctrlBlockKey);
	if (it != m_ScdDiffCompare->m_CBDiffMap.end())
	{
		GOOSECBStru *pGoose = (GOOSECBStru *)(it->second);
		SetAttrValueFromBasicStru(cbInfo.apAppID, pGoose->apAppID);
		SetAttrValueFromBasicStru(cbInfo.AppID, pGoose->AppID);
		SetAttrValueFromBasicStru(cbInfo.CBDesc, pGoose->CBDesc);
		SetAttrValueFromBasicStru(cbInfo.CBName, pGoose->CBName);
		SetAttrValueFromBasicStru(cbInfo.ConfVersion, pGoose->ConfVersion);
		SetAttrValueFromBasicStru(cbInfo.DataSetAddr, pGoose->DataSetAddr);
		SetAttrValueFromBasicStru(cbInfo.GocbRef, pGoose->GocbRef);
		SetAttrValueFromBasicStru(cbInfo.DataSetDesc, pGoose->DataSetDesc);
		SetAttrValueFromBasicStru(cbInfo.DataSetName, pGoose->DataSetName);
		SetAttrValueFromBasicIntStru(cbInfo.DataSetCount, pGoose->DataSetCount);
		SetAttrValueFromBasicStru(cbInfo.MACAddr, pGoose->MACAddr);
		SetAttrValueFromBasicStru(cbInfo.MaxTime, pGoose->MaxTime);
		SetAttrValueFromBasicStru(cbInfo.MinTime, pGoose->MinTime);
		SetAttrValueFromBasicStru(cbInfo.VLANID, pGoose->VLANID);
		SetAttrValueFromBasicStru(cbInfo.VLANPriority, pGoose->VLANPriority);
		SetAttrValueFromBasicStru(cbInfo.VLink, pGoose->VLink);
		cbInfo.state = pGoose->state;
		cbInfo.ID = pGoose->ID;
		return true;
	}
	return false;
}

/** 
  * @brief    GetCtrlBlockInfo
  * 
  * 获取Goose控制块信息
  * 
  * @class    TableModelDiffImpl
  * @param    QString ctrlBlockKey : 当前控制块的Key
  * @param    GOOSECBStru& cbInfo     : Goose控制块的信息
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
bool TableModelDiffImpl::GetCtrlBlockInfo(QString ctrlBlockKey, ReportCBStru& cbInfo)
{
	DifferMap::iterator it = m_ScdDiffCompare->m_CBDiffMap.find(ctrlBlockKey);
	if (it != m_ScdDiffCompare->m_CBDiffMap.end())
	{
		ReportCBStru *pGoose = (ReportCBStru *)(it->second);
		SetAttrValueFromBasicStru(cbInfo.apAppID, pGoose->apAppID);
		SetAttrValueFromBasicStru(cbInfo.AppID, pGoose->AppID);
		SetAttrValueFromBasicStru(cbInfo.CBDesc, pGoose->CBDesc);
		SetAttrValueFromBasicStru(cbInfo.CBName, pGoose->CBName);
		SetAttrValueFromBasicStru(cbInfo.ConfVersion, pGoose->ConfVersion);
		SetAttrValueFromBasicStru(cbInfo.DataSetAddr, pGoose->DataSetAddr);
		SetAttrValueFromBasicStru(cbInfo.DataSetDesc, pGoose->DataSetDesc);
		SetAttrValueFromBasicStru(cbInfo.DataSetName, pGoose->DataSetName);
		SetAttrValueFromBasicIntStru(cbInfo.DataSetCount, pGoose->DataSetCount);
		SetAttrValueFromBasicStru(cbInfo.MACAddr, pGoose->MACAddr);
		SetAttrValueFromBasicStru(cbInfo.VLANID, pGoose->VLANID);
		SetAttrValueFromBasicStru(cbInfo.VLANPriority, pGoose->VLANPriority);
		SetAttrValueFromBasicStru(cbInfo.VLink, pGoose->VLink);
		cbInfo.state = pGoose->state;
		cbInfo.ID = pGoose->ID;
		return true;
	}
	return false;
}
/** 
  * @brief    GetOutSignalsByCBKey
  * 
  * 通过控制块获取输出信号列表
  * 
  * @class    TableModelDiffImpl
  * @param    QString IEDKey                   : 当前IED
  * @param    QString cbKey                    : 控制块Key
  * @param    QVector<QString>* outSignalKeys  : 输出信号列表
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
bool TableModelDiffImpl::GetOutSignalsByCBKey(QString IEDKey, QString cbKey, QVector<QString>* outSignalKeys)
{
	DifferMap::iterator it = m_ScdDiffCompare->m_IEDDiffMap.find(IEDKey);
	if (it != m_ScdDiffCompare->m_IEDDiffMap.end())
	{
		IEDStru *pIED = (IEDStru*)(it->second);

		for (uint j = 0; j < pIED->vGOOSECB.size(); j++)
		{
			GOOSECBStru *pGoose = &pIED->vGOOSECB.at(j);
			if (pGoose->ID == cbKey)
			{
				for (uint k = 0; k < pGoose->vSignal.size(); k++)
				{
					outSignalKeys->push_back(pGoose->vSignal.at(k).ID);
				}
				return true;
			}
		}

		for (uint j = 0; j < pIED->vSVCB.size(); j++)
		{
			SVCBStru *pSV = &pIED->vSVCB.at(j);
			if (pSV->ID == cbKey)
			{
				for (uint k = 0; k < pSV->vSignal.size(); k++)
				{
					outSignalKeys->push_back(pSV->vSignal.at(k).ID);
				}
				return true;
			}
		}
	}
	return false;
}

/** 
  * @brief    GetInSignalsByIEDKey
  * 
  * 通过IED获取输入信号列表(即Input中的信号);
  * 
  * @class    TableModelDiffImpl
  * @param    QString IEDKey                  : 当前IED
  * @param    QVector<QString>* inSignalKeys  : 输入信号列表
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
bool TableModelDiffImpl::GetInSignalsByIEDKey(QString IEDKey, QVector<QString>* inSignalKeys)
{
	DifferMap::iterator it = m_ScdDiffCompare->m_IEDDiffMap.find(IEDKey);
	if (it != m_ScdDiffCompare->m_IEDDiffMap.end())
	{
		IEDStru *pIED = (IEDStru*)(it->second);
		for (uint j = 0; j < pIED->vInput.size(); j++)
		{
			InputStru *pInput = &pIED->vInput.at(j);
			for (uint k = 0; k < pInput->vExtRef.size(); k++)
			{
				inSignalKeys->push_back(pInput->vExtRef.at(k).InSigInfo.ID);
			}
		}
		return true;
	}
	return false;
}

/** 
  * @brief    GetInputsOfIED
  * 
  * 取得IED的Inputs信息
  * 
  * @class    TableModelDiffImpl
  * @param    QString IEDKey                : 当前IED的Key
  * @param    QVector<ExtRefStru>* ExtRefs  : 存储Inputs结构
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/17
  * @remarks  
  */ 
bool TableModelDiffImpl::GetInputsOfIED(QString IEDKey, QVector<ExtRefStru> &ExtRefs)
{
	DifferMap::iterator it = m_ScdDiffCompare->m_IEDDiffMap.find(IEDKey);
	if (it != m_ScdDiffCompare->m_IEDDiffMap.end())
	{
		IEDStru *pIED = (IEDStru*)(it->second);

		for (uint i = 0; i < pIED->vInput.size(); i++)
		{
			for (uint j = 0; j < pIED->vInput.at(i).vExtRef.size(); j++)
			{
				ExtRefStru pExtRef = pIED->vInput.at(i).vExtRef.at(j);

				ExtRefs.push_back(pExtRef);
			}
		}
		return true;

	}
	return false;
}

/** 
  * @brief    GetCtrlBlockOfOutIEDByInsignal
  * 
  * 通过输入信号获取对端IED的SV控制块;
  * 
  * @class    TableModelDiffImpl
  * @param    QString signalKey : 输入信号的Key
  * @param    SVCBStru* CBKey   : 对端的SV控制块
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
bool TableModelDiffImpl::GetCtrlBlockOfOutIEDByInsignal(QString signalKey, SVCBStru* CBKey)
{
	DifferMap::iterator it = m_ScdDiffCompare->m_InputsDiffMap.find(signalKey);
	if (it != m_ScdDiffCompare->m_InputsDiffMap.end())
	{
		ExtRefStru *pExtRef = (ExtRefStru*)(it->second);
		QString srcIEDKey = NameTranslator_GetIEDKeyFromSCDDiffResult(pExtRef->srcSignalID);
		DifferMap::iterator itIED = m_ScdDiffCompare->m_IEDDiffMap.find(srcIEDKey);
		if (itIED != m_ScdDiffCompare->m_IEDDiffMap.end())
		{
			IEDStru *pIED = (IEDStru*)(itIED->second);
			for (uint j = 0; j < pIED->vSVCB.size(); j++)
			{
				SVCBStru *pSVCB = &pIED->vSVCB.at(j);
				for (uint k = 0; k < pSVCB->vSignal.size(); k++)
				{
					if (pExtRef->srcSignalID == pSVCB->vSignal.at(k).ID)
					{
						*CBKey = *pSVCB;
						return true;
					}
				}
			}
		}
	}
	return false;
}

/** 
  * @brief    GetCtrlBlockOfOutIEDByInsignal
  * 
  * 通过输入信号获取对端IED的控制块;
  * 
  * @class    TableModelDiffImpl
  * @param    QString signalKey   : 输入信号的Key
  * @param    GOOSECBStru* CBKey  : Goose控制块的信息
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
bool TableModelDiffImpl::GetCtrlBlockOfOutIEDByInsignal(QString signalKey, GOOSECBStru* CBKey)
{
	DifferMap::iterator it = m_ScdDiffCompare->m_InputsDiffMap.find(signalKey);
	if (it != m_ScdDiffCompare->m_InputsDiffMap.end())
	{
		ExtRefStru *pExtRef = (ExtRefStru*)(it->second);
		QString srcIEDKey = NameTranslator_GetIEDKeyFromSCDDiffResult(pExtRef->srcSignalID);
		DifferMap::iterator itIED = m_ScdDiffCompare->m_IEDDiffMap.find(srcIEDKey);
		if (itIED != m_ScdDiffCompare->m_IEDDiffMap.end())
		{
			IEDStru *pIED = (IEDStru*)(itIED->second);
			for (uint j = 0; j < pIED->vGOOSECB.size(); j++)
			{
				GOOSECBStru *pGooseCB = &pIED->vGOOSECB.at(j);
				for (uint k = 0; k < pGooseCB->vSignal.size(); k++)
				{
					if (pExtRef->srcSignalID == pGooseCB->vSignal.at(k).ID)
					{
						*CBKey = *pGooseCB;
						return true;
					}
				}
			}
		}
	}
	return false;
}


/** 
  * @brief    GetCtrlBlockOfOutIEDByOutsignal
  * 
  * 通过输出信号获取IED的SV控制块;
  * 
  * @class    TableModelDiffImpl
  * @param    QString signalKey : 输入信号的Key
  * @param    SVCBStru* CBKey   : 对端的SV控制块
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
bool TableModelDiffImpl::GetCtrlBlockOfOutIEDByOutsignal(QString signalKey, SVCBStru* CBKey)
{
	DifferMap::iterator it = m_ScdDiffCompare->m_OutSigDiffMap.find(signalKey);
	if (it != m_ScdDiffCompare->m_OutSigDiffMap.end())
	{
		SignalStru *pSignal = (SignalStru *)(it->second);
		QString ctrlBlockKey = pSignal->ctrlBlockID;
		QString srcIEDKey = NameTranslator_GetIEDKeyFromSCDDiffResult(signalKey);
		DifferMap::iterator itIED = m_ScdDiffCompare->m_IEDDiffMap.find(srcIEDKey);
		if (itIED != m_ScdDiffCompare->m_IEDDiffMap.end())
		{
			IEDStru *pIED = (IEDStru*)(itIED->second);
			for (uint j = 0; j < pIED->vSVCB.size(); j++)
			{
				SVCBStru *pSVCB = &pIED->vSVCB.at(j);

				if (pSVCB->ID == ctrlBlockKey)
				{
					*CBKey = *pSVCB;
					return true;
				}
			}
		}
	}
	return false;
}

/** 
  * @brief    GetCtrlBlockOfOutIEDByOutsignal
  * 
  * 通过输出信号获取IED的控制块;
  * 
  * @class    TableModelDiffImpl
  * @param    QString signalKey   : 输入信号的Key
  * @param    GOOSECBStru* CBKey  : Goose控制块的信息
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
bool TableModelDiffImpl::GetCtrlBlockOfOutIEDByOutsignal(QString signalKey, GOOSECBStru* CBKey)
{
	DifferMap::iterator it = m_ScdDiffCompare->m_OutSigDiffMap.find(signalKey);
	if (it != m_ScdDiffCompare->m_OutSigDiffMap.end())
	{
		SignalStru *pSignal = (SignalStru *)(it->second);
		QString ctrlBlockKey = pSignal->ctrlBlockID;
		QString srcIEDKey = NameTranslator_GetIEDKeyFromSCDDiffResult(signalKey);
		DifferMap::iterator itIED = m_ScdDiffCompare->m_IEDDiffMap.find(srcIEDKey);
		if (itIED != m_ScdDiffCompare->m_IEDDiffMap.end())
		{
			IEDStru *pIED = (IEDStru*)(itIED->second);
			for (uint j = 0; j < pIED->vGOOSECB.size(); j++)
			{
				GOOSECBStru *pGooseCB = &pIED->vGOOSECB.at(j);

				if (pGooseCB->ID == ctrlBlockKey)
				{
					*CBKey = *pGooseCB;
					return true;
				}
			}
		}
	}
	return false;
}

/** 
  * @brief    GetInSignalsOfOutsignal
  * 
  * 通过输出信号获取对端的输入信号列表;
  * 
  * @class    TableModelDiffImpl
  * @param    QString outSignalKey            : 输出信号
  * @param    QVector<QString>* inSignalKeys  : 输出信号所对应的输入信号列表
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
bool TableModelDiffImpl::GetInSignalsOfOutsignal(QString outSignalKey, QVector<QString>* inSignalKeys)
{
	QString IEDKey = NameTranslator_GetIEDKeyFromSCDDiffResult(outSignalKey);

	DifferMap::iterator it = m_ScdDiffCompare->m_IEDDiffMap.find(IEDKey);
	if (it != m_ScdDiffCompare->m_IEDDiffMap.end())
	{
		IEDStru *pIED = (IEDStru*)(it->second);

		for (uint i = 0; i < pIED->vVirLink.size(); i++)
		{
			if (pIED->vVirLink.at(i).srcSig == outSignalKey)
			{
				inSignalKeys->push_back(pIED->vVirLink.at(i).destSig);
			}
		}
		return true;
	}
	return false;
}

/** 
  * @brief    GetIEDListBySubnetKey
  * 
  * 添加详细描述
  * 
  * @class    TableModelDiffImpl
  * @param    QString subnetKey          :
  * @param    QVector<QString> &subIEDs  :
  * @param    
  * @return   bool
  * @author   牛子明
  * @date     2014/12/1
  * @remarks  
  */ 
bool TableModelDiffImpl::GetIEDListBySubnetKey(QString subnetKey, QVector<QString> &subIEDs)		//通过子网Key获取子网内IED;
{
	return true;
}

/** 
  * @brief    GetApInfo
  * 
  * 添加详细描述
  * 
  * @class    TableModelDiffImpl
  * @param    QString subnetKey :
  * @param    QString IEDKey    :
  * @param    QAPInfo &apInfo   :
  * @param    
  * @return   bool
  * @author   牛子明
  * @date     2014/12/1
  * @remarks  
  */ 
bool TableModelDiffImpl::GetApInfo(QString subnetKey, QString IEDKey, QString APName, QAPInfo &apInfo)			//通过子网Key以及IEDKey获取接入点信息;
{
	return true;
}

bool TableModelDiffImpl::GetAPListOfIEDFromSubnet(const QString& subNetKey, const QString IEDName, QVector<QString> &APList)// 取得子网下IED的AP列表
{
	return true;
}

bool TableModelDiffImpl::GetPortListOfIEDAPFromSubNet(const QString& subNetKey, const QString IEDName, const QString APName, QVector<QString> &PortList)// 取得子网下IED某个AP包含的物理端口列表
{
	return true;
}

bool TableModelDiffImpl::GetAPListFromIED(const QString& IEDKey, QVector<QString> &APList)// 取得IED下AP的列表
{
	return true;
}

bool TableModelDiffImpl::GetAPPortListFormIED(const QString& IEDKey, const QString &APName, QVector<QString> &PortList)// 取得IED下某个AP对应的物理端口列表
{
	return true;
}

/** 
  * @brief    SetAttrValueFromBasicStru
  * 
  * 根据源属性设置目的属性值
  * 
  * @class    TableModelDiffImpl
  * @param    BasicStru &DestBasic : 目的属性值
  * @param    BasicStru &SrcBasic  : 源属性值
  * @return   void
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
void TableModelDiffImpl::SetAttrValueFromBasicStru(BasicStru &DestBasic, BasicStru &SrcBasic)
{
	DestBasic.baseAttributeName = SrcBasic.baseAttributeName;
	DestBasic.otherAttributeName = SrcBasic.otherAttributeName;
	DestBasic.state = SrcBasic.state;
}

/** 
  * @brief    SetAttrValueFromBasicStru
  * 
  * 根据源属性设置目的属性值
  * 
  * @class    TableModelDiffImpl
  * @param    BasicStru &DestBasic : 目的属性值
  * @param    BasicStru &SrcBasic  : 源属性值
  * @return   void
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
void TableModelDiffImpl::SetAttrValueFromBasicIntStru(BasicIntStru &DestBasic, BasicIntStru &SrcBasic)
{
	DestBasic.baseAttributeName = SrcBasic.baseAttributeName;
	DestBasic.otherAttributeName = SrcBasic.otherAttributeName;
	DestBasic.state = SrcBasic.state;
}

/** 
  * @brief    NameTranslator_GetIEDKeyFromSCDDiffResult
  * 
  * 通过NameTranslator，从实时库中的Key提取IED的名称
  * 
  * @class    GraphicModelDiffImpl
  * @param    QString qString : 实时库中的Key
  * @return   QString
  * @author   zhang-xch
  * @date     2014/5/15
  * @remarks  
  */ 
QString TableModelDiffImpl::NameTranslator_GetIEDKeyFromSCDDiffResult(QString qString)
{
	return QString::fromStdString(m_NameTranslator.GetIEDKeyFromSCDDiffResult(qString.toStdString()));
}
