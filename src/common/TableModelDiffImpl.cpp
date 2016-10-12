#include "TableModelDiffImpl.h"
#include <QStringList>

/** 
  * @brief    TableModelDiffImpl
  * 
  * ���캯��
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
  * ��������
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
  * ��û�׼�ļ�����Ϣ
  * 
  * @class    TableModelDiffImpl
  * @param    FileInfoStru& FileInfo :	�����׼�ļ�����Ϣ
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
  * ��ñ��Ƚ��ļ���Ϣ
  * 
  * @class    TableModelDiffImpl
  * @param    FileInfoStru& FileInfo : ����Ƚ��ļ�����Ϣ
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
  * �������װ��ID
  * 
  * @class    TableModelDiffImpl
  * @param    QStringList& IEDKeys : ����װ�õ�ID
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
  * ���װ����Ϣ
  * 
  * @class    TableModelDiffImpl
  * @param    QString IEDKey    : װ�õ�Key
  * @param    IEDStru& IEDInfo  : ����װ����Ϣ
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
  * ��������źŵ���Ϣ
  * 
  * @class    TableModelDiffImpl
  * @param    QString signalKey      : �����ź�Key
  * @param    SignalStru *signalInfo : �����źŵ���Ϣ
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
  * �������źŵ���Ϣ
  * 
  * @class    TableModelDiffImpl
  * @param    QString signalKey      : ����źŵ�key
  * @param    SignalStru *signalInfo : ����źŵ���Ϣ
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
  * ��ȡ��ǰIED��GOOSE���ƿ��б�
  * 
  * @class    TableModelDiffImpl
  * @param    QString IEDKey                   : ��ǰIED��Key
  * @param    QVector<QString>* ctrlBlockList  : IED��Goose���ƿ��б�
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
  * ��ȡ��ǰIED��SV���ƿ��б�
  * 
  * @class    TableModelDiffImpl
  * @param    QString IEDKey                   : ��ǰIED��Key
  * @param    QVector<QString>* ctrlBlockList  : ��ǰIED��SV���ƿ��б�
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
  * ��ȡSV���ƿ���Ϣ
  * 
  * @class    TableModelDiffImpl
  * @param    QString ctrlBlockKey : ��ǰ���ƿ��Key
  * @param    SVCBStru& cbInfo     : SV���ƿ����Ϣ
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
  * ��ȡGoose���ƿ���Ϣ
  * 
  * @class    TableModelDiffImpl
  * @param    QString ctrlBlockKey : ��ǰ���ƿ��Key
  * @param    GOOSECBStru& cbInfo     : Goose���ƿ����Ϣ
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
  * ��ȡGoose���ƿ���Ϣ
  * 
  * @class    TableModelDiffImpl
  * @param    QString ctrlBlockKey : ��ǰ���ƿ��Key
  * @param    GOOSECBStru& cbInfo     : Goose���ƿ����Ϣ
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
  * ͨ�����ƿ��ȡ����ź��б�
  * 
  * @class    TableModelDiffImpl
  * @param    QString IEDKey                   : ��ǰIED
  * @param    QString cbKey                    : ���ƿ�Key
  * @param    QVector<QString>* outSignalKeys  : ����ź��б�
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
  * ͨ��IED��ȡ�����ź��б�(��Input�е��ź�);
  * 
  * @class    TableModelDiffImpl
  * @param    QString IEDKey                  : ��ǰIED
  * @param    QVector<QString>* inSignalKeys  : �����ź��б�
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
  * ȡ��IED��Inputs��Ϣ
  * 
  * @class    TableModelDiffImpl
  * @param    QString IEDKey                : ��ǰIED��Key
  * @param    QVector<ExtRefStru>* ExtRefs  : �洢Inputs�ṹ
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
  * ͨ�������źŻ�ȡ�Զ�IED��SV���ƿ�;
  * 
  * @class    TableModelDiffImpl
  * @param    QString signalKey : �����źŵ�Key
  * @param    SVCBStru* CBKey   : �Զ˵�SV���ƿ�
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
  * ͨ�������źŻ�ȡ�Զ�IED�Ŀ��ƿ�;
  * 
  * @class    TableModelDiffImpl
  * @param    QString signalKey   : �����źŵ�Key
  * @param    GOOSECBStru* CBKey  : Goose���ƿ����Ϣ
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
  * ͨ������źŻ�ȡIED��SV���ƿ�;
  * 
  * @class    TableModelDiffImpl
  * @param    QString signalKey : �����źŵ�Key
  * @param    SVCBStru* CBKey   : �Զ˵�SV���ƿ�
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
  * ͨ������źŻ�ȡIED�Ŀ��ƿ�;
  * 
  * @class    TableModelDiffImpl
  * @param    QString signalKey   : �����źŵ�Key
  * @param    GOOSECBStru* CBKey  : Goose���ƿ����Ϣ
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
  * ͨ������źŻ�ȡ�Զ˵������ź��б�;
  * 
  * @class    TableModelDiffImpl
  * @param    QString outSignalKey            : ����ź�
  * @param    QVector<QString>* inSignalKeys  : ����ź�����Ӧ�������ź��б�
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
  * �����ϸ����
  * 
  * @class    TableModelDiffImpl
  * @param    QString subnetKey          :
  * @param    QVector<QString> &subIEDs  :
  * @param    
  * @return   bool
  * @author   ţ����
  * @date     2014/12/1
  * @remarks  
  */ 
bool TableModelDiffImpl::GetIEDListBySubnetKey(QString subnetKey, QVector<QString> &subIEDs)		//ͨ������Key��ȡ������IED;
{
	return true;
}

/** 
  * @brief    GetApInfo
  * 
  * �����ϸ����
  * 
  * @class    TableModelDiffImpl
  * @param    QString subnetKey :
  * @param    QString IEDKey    :
  * @param    QAPInfo &apInfo   :
  * @param    
  * @return   bool
  * @author   ţ����
  * @date     2014/12/1
  * @remarks  
  */ 
bool TableModelDiffImpl::GetApInfo(QString subnetKey, QString IEDKey, QString APName, QAPInfo &apInfo)			//ͨ������Key�Լ�IEDKey��ȡ�������Ϣ;
{
	return true;
}

bool TableModelDiffImpl::GetAPListOfIEDFromSubnet(const QString& subNetKey, const QString IEDName, QVector<QString> &APList)// ȡ��������IED��AP�б�
{
	return true;
}

bool TableModelDiffImpl::GetPortListOfIEDAPFromSubNet(const QString& subNetKey, const QString IEDName, const QString APName, QVector<QString> &PortList)// ȡ��������IEDĳ��AP����������˿��б�
{
	return true;
}

bool TableModelDiffImpl::GetAPListFromIED(const QString& IEDKey, QVector<QString> &APList)// ȡ��IED��AP���б�
{
	return true;
}

bool TableModelDiffImpl::GetAPPortListFormIED(const QString& IEDKey, const QString &APName, QVector<QString> &PortList)// ȡ��IED��ĳ��AP��Ӧ������˿��б�
{
	return true;
}

/** 
  * @brief    SetAttrValueFromBasicStru
  * 
  * ����Դ��������Ŀ������ֵ
  * 
  * @class    TableModelDiffImpl
  * @param    BasicStru &DestBasic : Ŀ������ֵ
  * @param    BasicStru &SrcBasic  : Դ����ֵ
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
  * ����Դ��������Ŀ������ֵ
  * 
  * @class    TableModelDiffImpl
  * @param    BasicStru &DestBasic : Ŀ������ֵ
  * @param    BasicStru &SrcBasic  : Դ����ֵ
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
  * ͨ��NameTranslator����ʵʱ���е�Key��ȡIED������
  * 
  * @class    GraphicModelDiffImpl
  * @param    QString qString : ʵʱ���е�Key
  * @return   QString
  * @author   zhang-xch
  * @date     2014/5/15
  * @remarks  
  */ 
QString TableModelDiffImpl::NameTranslator_GetIEDKeyFromSCDDiffResult(QString qString)
{
	return QString::fromStdString(m_NameTranslator.GetIEDKeyFromSCDDiffResult(qString.toStdString()));
}
