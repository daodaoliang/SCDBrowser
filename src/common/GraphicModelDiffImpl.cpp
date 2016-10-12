#include "GraphicModelDiffImpl.h"
#include <QStringList>
#include "QDebug"

/** 
  * @brief    GraphicModelDiffImpl
  * 
  * 构造函数
  * 
  * @class    GraphicModelDiffImpl
  * @return   
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
GraphicModelDiffImpl::GraphicModelDiffImpl(SCDDiffCompare *scdDiffer)
{
	SCLModel = SCLModelAdapter::GetInstance();
	m_SCDDiffCompare = scdDiffer;
}

/** 
  * @brief    ~GraphicModelDiffImpl
  * 
  * 析构函数
  * 
  * @class    GraphicModelDiffImpl
  * @return   
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
GraphicModelDiffImpl::~GraphicModelDiffImpl()
{

}

/** 
  * @brief    GetBaseFileInfo
  * 
  * 获得基准文件信息
  * 
  * @class    GraphicModelDiffImpl
  * @param    QSubStationInfo& FileInfo :保存文件结果
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/15
  * @remarks  
  */ 
bool GraphicModelDiffImpl::GetBaseFileInfo(QSubStationInfo& FileInfo)
{
	FileInfoStru *pFileInfo = &m_SCDDiffCompare->m_SCDDiffResult.sFileInfo;
	if (pFileInfo != NULL)
	{
		FileInfo.Name = pFileInfo->baseSubstationName;
		FileInfo.fileName = pFileInfo->basefileName;
		FileInfo.crc = pFileInfo->crc.baseAttributeName;
		FileInfo.version = pFileInfo->version.baseAttributeName;
		FileInfo.reversion = pFileInfo->reVersion.baseAttributeName;
		return true;
	}
	return false;
}
	
/** 
  * @brief    GetOtherFileInfo
  * 
  * 获得被比较文件信息
  * 
  * @class    GraphicModelDiffImpl
  * @param    QSubStationInfo& FileInfo :保存文件信息
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/15
  * @remarks  
  */ 
bool GraphicModelDiffImpl::GetOtherFileInfo(QSubStationInfo& FileInfo)
{
	FileInfoStru *pFileInfo = &m_SCDDiffCompare->m_SCDDiffResult.sFileInfo;
	if (pFileInfo != NULL)
	{
		FileInfo.Name = pFileInfo->otherSubstationName;
		FileInfo.fileName = pFileInfo->otherfileName;
		FileInfo.crc = pFileInfo->crc.otherAttributeName;
		FileInfo.version = pFileInfo->version.otherAttributeName;
		FileInfo.reversion = pFileInfo->reVersion.otherAttributeName;
		return true;
	}
	return false;
}

/** 
  * @brief    GetIEDList
  * 
  * 获得所有装置的ID
  * 
  * @class    GraphicModelDiffImpl
  * @param    QStringList& IEDKeys :保存IED的Key
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/15
  * @remarks  
  */ 
bool GraphicModelDiffImpl::GetIEDList(QString panelKey, QVector<QString>* IEDKeys)
{
	for (uint i = 0; i < m_SCDDiffCompare->m_SCDDiffResult.vIEDStru.size(); i++)
	{
		IEDStru *pIEDStru = &m_SCDDiffCompare->m_SCDDiffResult.vIEDStru.at(i);
		IEDKeys->push_back(pIEDStru->ID);
	}
	return true;
}

bool GraphicModelDiffImpl::GetIEDListBySubNetKey(QString subNetKey, QVector<QString> *subIEDs)
{
	return true;
}

bool GraphicModelDiffImpl::GetSubNetKeysByIEDKey(QString IEDKey, QVector<QString> *subNetKeys)
{
	return true;
}

/** 
  * @brief    GetIEDInfo
  * 
  * 获得IEDKey所对应的装置的信息
  * 
  * @class    GraphicModelDiffImpl
  * @param    QString IEDKey     : 要查找装置的IEDKey
  * @param    QIEDInfo& IEDInfo  : 保存IEDKey所对应的装置的信息
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/15
  * @remarks  
  */ 
bool GraphicModelDiffImpl::GetIEDInfo(QString IEDKey, QIEDInfo& IEDInfo)
{
	DifferMap::iterator it = m_SCDDiffCompare->m_IEDDiffMap.find(IEDKey);
	if (it != m_SCDDiffCompare->m_IEDDiffMap.end())
	{
		IEDStru *pIED = (IEDStru *)(it->second);
		IEDInfo.name = pIED->IEDName;
		IEDInfo.DiffState = pIED->state;
		SetAttributeValueFromBasicStru(IEDInfo.type, pIED->IEDType);
		SetAttributeValueFromBasicStru(IEDInfo.manu, pIED->IEDManu);
		SetAttributeValueFromBasicStru(IEDInfo.desc, pIED->IEDDesc);
		SetAttributeValueFromBasicStru(IEDInfo.crc, pIED->IEDCRC);

		IEDInfo.diffDesc = SetIEDDiffDesc(pIED);
		return true;
	}
	return false;
}

/** 
  * @brief    SetIEDDiffDesc
  * 
  * 设置IED变更描述
  * 
  * @class    GraphicModelDiffImpl
  * @param    IEDStru *pIED : 当前IED
  * @return   QString
  * @author   zhang-xch
  * @date     2014/6/19
  * @remarks  
  */ 
QString GraphicModelDiffImpl::SetIEDDiffDesc(IEDStru *pIED)
{
	QString diffDesc = "";
	diffDesc = QObject::tr("当前路径：") + pIED->IEDName + "\n\n";
	BasicStru *pBasicStru = &pIED->IEDType;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pIED->IEDManu;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pIED->IEDDesc;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pIED->IEDCRC;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	// GOOSE变化信息
	int iGooseCount = 0;
	for (uint i = 0; i < pIED->vGOOSECB.size(); i++)
	{
		if (pIED->vGOOSECB.at(i).state != DIFFSTATE_NORMAL)
		{
			iGooseCount++;
		}
	}
	if (iGooseCount >0)
	{
		diffDesc = diffDesc + QObject::tr("GOOSE控制块变更个数：") + QString::number(iGooseCount) + "\n";
	}
	// SV变化信息
	int iSvCount = 0;
	for (uint i = 0; i < pIED->vSVCB.size(); i++)
	{
		if (pIED->vSVCB.at(i).state != DIFFSTATE_NORMAL)
		{
			iSvCount++;
		}
	}
	if (iSvCount >0)
	{
		diffDesc = diffDesc + QObject::tr("SV控制块变更个数：") + QString::number(iSvCount) + "\n";
	}
	// 虚回路变更信息
	int iInputsCount = 0;
	for (uint j = 0; j < pIED->vInput.size(); j++)
	{
		if (pIED->vInput.at(j).ExtRefCount.state != DIFFSTATE_NORMAL)
		{
			diffDesc = diffDesc + QObject::tr("虚链接个数发生变化: \n基准文件个数：") + QString::number(pIED->vInput.at(j).ExtRefCount.baseAttributeName) + QObject::tr("\n比较文件个数：") + QString::number(pIED->vInput.at(j).ExtRefCount.otherAttributeName) + "\n";
		}
		
		for (uint i = 0; i < pIED->vInput.at(j).vExtRef.size(); i++)
		{
			if (pIED->vInput.at(j).vExtRef.at(i).state != DIFFSTATE_NORMAL)
			{
				iInputsCount++;
			}
		}
	}
	
	if (iInputsCount >0)
	{
		diffDesc = diffDesc + QObject::tr("虚回路变更个数：") + QString::number(iInputsCount) + "\n";
	}

	// 物理端口变更信息
	int iPhyPortCount = 0;
	for (uint i = 0; i < pIED->vPortStru.size(); i++)
	{
		if (pIED->vPortStru.at(i).state != DIFFSTATE_NORMAL)
		{
			iPhyPortCount++;
		}
	}
	if (iPhyPortCount >0)
	{
		diffDesc = diffDesc + QObject::tr("物理端口变更个数：") + QString::number(iPhyPortCount) + "\n";
	}

	return diffDesc;
}
  
/** 
  * @brief    GetInfoOfInSignal
  * 
  * 根据输入信号获得输入信号的信息
  * 
  * @class    GraphicModelDiffImpl
  * @param    QString signalKey       : 输入信号的Key
  * @param    QSignalInfo *signalInfo : 输入信号的信息
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/15
  * @remarks  
  */ 
bool GraphicModelDiffImpl::GetInfoOfInSignal(QString signalKey,QSignalInfo *signalInfo)
{
	DifferMap::iterator it = m_SCDDiffCompare->m_InSigDiffMap.find(signalKey);
	if (it != m_SCDDiffCompare->m_InSigDiffMap.end())
	{
		SignalStru *pSignal = (SignalStru *)(it->second);
		signalInfo->state = pSignal->state;
		signalInfo->DiffState = pSignal->state;
		SetAttributeValueFromBasicStru(signalInfo->BDAType, pSignal->BDAType);
		SetAttributeValueFromBasicStru(signalInfo->desc, pSignal->Desc);
		SetAttributeValueFromBasicStru(signalInfo->DOIDesc, pSignal->DODesc);
		SetAttributeValueFromBasicStru(signalInfo->dUVal, pSignal->DOIdU);
		SetAttributeValueFromBasicStru(signalInfo->fc, pSignal->FC);
		signalInfo->ID = pSignal->ID;
		if (pSignal->Ord.state == DIFFSTATE_REMOVED)
		{
			signalInfo->iOrd = pSignal->Ord.otherAttributeName;
		}else {
			signalInfo->iOrd = pSignal->Ord.baseAttributeName;
		}
		SetAttributeValueFromBasicStru(signalInfo->LnDesc, pSignal->LNDesc);
		signalInfo->name = pSignal->ID;
		QString portIndex;
		SetAttributeValueFromBasicStru(portIndex, pSignal->PortIndex);
		signalInfo->portIndexList = m_NameTranslator.GetSignalPortListFromQString(portIndex);
		SetAttributeValueFromBasicStru(signalInfo->softTripKey, pSignal->TripName);

		signalInfo->diffDesc = SetSignalDiffDesc(pSignal);
		return true;
	}
	return false;
}

/** 
  * @brief    GetInfoOfOutSignal
  * 
  * 根据输出信号Key获得输出信号的信息
  * 
  * @class    GraphicModelDiffImpl
  * @param    QString signalKey       : 输出信号的Key
  * @param    QSignalInfo *signalInfo : 输出信号的信息
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/15
  * @remarks  
  */ 
bool GraphicModelDiffImpl::GetInfoOfOutSignal(QString signalKey,QSignalInfo *signalInfo)
{
	DifferMap::iterator it = m_SCDDiffCompare->m_OutSigDiffMap.find(signalKey);
	if (it != m_SCDDiffCompare->m_OutSigDiffMap.end())
	{
		SignalStru *pSignal = (SignalStru *)(it->second);
		signalInfo->state = pSignal->state;
		signalInfo->DiffState = pSignal->state;
		SetAttributeValueFromBasicStru(signalInfo->BDAType, pSignal->BDAType);
		SetAttributeValueFromBasicStru(signalInfo->desc, pSignal->Desc);
		SetAttributeValueFromBasicStru(signalInfo->DOIDesc, pSignal->DODesc);
		SetAttributeValueFromBasicStru(signalInfo->dUVal, pSignal->DOIdU);
		SetAttributeValueFromBasicStru(signalInfo->fc, pSignal->FC);
		signalInfo->ID = pSignal->ID;
		if (pSignal->Ord.state == DIFFSTATE_REMOVED)
		{
			signalInfo->iOrd = pSignal->Ord.otherAttributeName;
		}else {
			signalInfo->iOrd = pSignal->Ord.baseAttributeName;
		}
		SetAttributeValueFromBasicStru(signalInfo->LnDesc, pSignal->LNDesc);
		signalInfo->name = pSignal->ID;

		QString portIndex;
		SetAttributeValueFromBasicStru(portIndex, pSignal->PortIndex);
		signalInfo->portIndexList = m_NameTranslator.GetSignalPortListFromQString(portIndex);

		SetAttributeValueFromBasicStru(signalInfo->softTripKey, pSignal->TripName);

		signalInfo->diffDesc = SetSignalDiffDesc(pSignal);
		return true;
	}
	return false;
}

/** 
  * @brief    SetSignalDiffDesc
  * 
  * 设置信号变更描述
  * 
  * @class    GraphicModelDiffImpl
  * @param    SignalStru *pSignal : 当前信号
  * @return   QString
  * @author   zhang-xch
  * @date     2014/6/19
  * @remarks  
  */ 
QString GraphicModelDiffImpl::SetSignalDiffDesc(SignalStru *pSignal)
{
	QString diffDesc = "";
	QString IED = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pSignal->ID.toStdString()));
	QString ObjName = QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pSignal->ID.toStdString()));
	diffDesc = QObject::tr("当前路径：") + IED + " " + ObjName + "\n\n" ;
	BasicStru *pBasicStru = &pSignal->BDAType;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pSignal->Desc;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pSignal->DODesc;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pSignal->DOIdU;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pSignal->FC;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pSignal->LNDesc;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pSignal->PortIndex;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pSignal->TripName;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	BasicIntStru *pBasicIntStru = &pSignal->Ord;
	if (pBasicIntStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicIntStru->AttributeDesc + QObject::tr(":\n基准文件：") + QString::number(pBasicIntStru->baseAttributeName) + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}

	return diffDesc;
}
/** 
  * @brief    GetSignalInfosByIEDKeyAndCBKey
  * 
  * 获得masterIEDKey为关键字IED的CBkey控制块发送给slaveIEDKey为关键字IED的虚链接列表
  * 
  * @class    GraphicModelDiffImpl
  * @param    const QString masterIEDKey              : 主文件的IEDKey
  * @param    const QString slaveIEDKey               : 从文件的IEDKey
  * @param    const QString CBkey                     : 输出信号所在的控制块Key
  * @param    QVector<QVirtualLink> *VirtualLinkList  : 虚链接列表
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/15
  * @remarks  
  */ 
bool GraphicModelDiffImpl::GetSignalInfosByIEDKeyAndCBKey(const QString masterIEDKey, const QString slaveIEDKey, const QString CBkey, QVector<QVirtualLink> *VirtualLinkList)
{
	DifferMap::iterator it  = m_SCDDiffCompare->m_IEDDiffMap.find(masterIEDKey);
	QVector<VirtualLinkStur> vVLinkVector;
	if (it != m_SCDDiffCompare->m_IEDDiffMap.end())
	{
		IEDStru *pIED = (IEDStru*)(it->second);
		for (uint i = 0; i < pIED->vVirLink.size(); i++)
		{
			if (slaveIEDKey == NameTranslator_GetIEDKeyFromSCDDiffResult(pIED->vVirLink.at(i).destSig))
			{
				vVLinkVector.push_back(pIED->vVirLink.at(i));
			}
		}

		QVirtualLink tempQVLink;
		for (int k = 0; k < vVLinkVector.size(); k++)
		{
			bool isInclude = false;
			for (uint i = 0; i < pIED->vGOOSECB.size(); i++)
			{
				if (pIED->vGOOSECB.at(i).ID == CBkey)
				{
					for (uint j = 0; j < pIED->vGOOSECB.at(i).vSignal.size(); j++)
					{
						SignalStru *pSig = &pIED->vGOOSECB.at(i).vSignal.at(j);
						if (pSig->ID == vVLinkVector.at(k).srcSig)
						{
							isInclude = true;
							break;
						}
					}
				}
			}
			if (isInclude)
			{
				tempQVLink.DiffState = vVLinkVector.at(k).state;
				tempQVLink.srcSignal = vVLinkVector.at(k).srcSig;
				tempQVLink.destSignal = vVLinkVector.at(k).destSig;
				VirtualLinkList->push_back(tempQVLink);
				continue;
			}
			for (uint i = 0; i < pIED->vSVCB.size(); i++)
			{
				if (pIED->vSVCB.at(i).ID == CBkey)
				{
					for (uint j = 0; j < pIED->vSVCB.at(i).vSignal.size(); j++)
					{
						SignalStru *pSig = &pIED->vSVCB.at(i).vSignal.at(j);
						if (pSig->ID == vVLinkVector.at(k).srcSig)
						{
							isInclude = true;
							break;
						}
					}
				}
			}
			if (isInclude)
			{
				tempQVLink.DiffState = vVLinkVector.at(k).state;
				tempQVLink.srcSignal = vVLinkVector.at(k).srcSig;
				tempQVLink.destSignal = vVLinkVector.at(k).destSig;
				VirtualLinkList->push_back(tempQVLink);
				continue;
			}
		}
	}

	return false;
}

/** 
  * @brief    GetVirLinkStateByIEDKeyAndCBKey
  * 
  * 获得masterIEDKey为关键字IED的CBkey控制块发送给slaveIEDKey为关键字IED的虚链接列表缺陷状态
  * 
  * @class    GraphicModelDiffImpl
  * @param    const QString masterIEDKey : 主文件的IEDKey
  * @param    const QString slaveIEDKey  : 从文件的IEDKey
  * @param    const QString CBkey        : 输出信号所在的控制块Key
  * @return   DiffStates
  * @author   zhang-xch
  * @date     2014/6/4
  * @remarks  
  */ 
VirLinkDiff   GraphicModelDiffImpl::GetVirLinkStateByIEDKeyAndCBKey(const QString masterIEDKey, const QString slaveIEDKey, const QString CBkey)
{
	VirLinkDiff VLinkDiff;
	QVector<QVirtualLink> VirtualLinkList;
	GetSignalInfosByIEDKeyAndCBKey(masterIEDKey, slaveIEDKey, CBkey, &VirtualLinkList);
	bool isRemoved = false, isAdd = false; 
	DiffStates state = DIFFSTATE_NORMAL;
	int iCount = 0;
	if (VirtualLinkList.size() > 0)
	{
		for (int i = 0; i < VirtualLinkList.size(); i++)
		{
			QVirtualLink tempLink = VirtualLinkList.at(i);
			if (tempLink.DiffState == DIFFSTATE_REMOVED)
			{
				isRemoved = true;
				iCount++;
				state = DIFFSTATE_REMOVED;
			}else if (tempLink.DiffState == DIFFSTATE_ADDED)
			{
				isAdd = true;
				iCount++;
				state = DIFFSTATE_ADDED;
			}else if (tempLink.DiffState == DIFFSTATE_EDITED)
			{
				state = DIFFSTATE_EDITED;
				iCount++;
			}
		}
	}
	if (isRemoved && isAdd)
	{
		state = DIFFSTATE_EDITED;;
	}
	VLinkDiff.state = state;
	VLinkDiff.iCount = iCount;

	QString masterIED = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(masterIEDKey.toStdString()));
	QString slaverIED = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(slaveIEDKey.toStdString()));
	QString ObjName = QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(CBkey.toStdString()));
	VLinkDiff.diffDesc = "当前路径：从IED "  + masterIED + " 经过控制块 " + ObjName + " 发送到 " + slaverIED;
	return VLinkDiff;
}

/** 
  * @brief    GetSignalInfosByIEDKey
  * 
  * 根据IEDKey取得与这个IED相关的所有的虚链接
  * 
  * @class    GraphicModelDiffImpl
  * @param    const QString IEDKey                    : 相关的IEDKey
  * @param    QVector<QVirtualLink> *VirtualLinkList  : 与这个IED相关的所有的虚链接列表
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/15
  * @remarks  
  */ 
bool GraphicModelDiffImpl::GetSignalInfosByIEDKey(const QString IEDKey, QVector<QVirtualLink> *VirtualLinkList)
{
	DifferMap::iterator it = m_SCDDiffCompare->m_IEDDiffMap.find(IEDKey);
	if (it != m_SCDDiffCompare->m_IEDDiffMap.end())
	{
		IEDStru *pIED = (IEDStru *)(it->second);
		for (uint j = 0; j < pIED->vVirLink.size(); j++)
		{
			VirtualLinkStur *pVLink = &pIED->vVirLink.at(j);
			QVirtualLink virLink;
			virLink.DiffState = pVLink->state;
			virLink.srcSignal = pVLink->srcSig;
			virLink.destSignal = pVLink->destSig;
			VirtualLinkList->push_back(virLink);
		}
		return true;
	}
	return false;
}

int	GraphicModelDiffImpl::GetSignalCountByIEDKey(const QString IEDKey, QSignalTypeEnum sigType)
{
	return 0;
}

/** 
  * @brief    GetCtrlBlockListByIED
  * 
  * 获取当前IED的控制块列表
  * 
  * @class    GraphicModelDiffImpl
  * @param    QString IEDKey                   : IED的Key
  * @param    QVector<QString>* ctrlBlockList  : 存储控制块列表
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/15
  * @remarks  
  */ 
bool GraphicModelDiffImpl::GetCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList)
{
	DifferMap::iterator it = m_SCDDiffCompare->m_IEDDiffMap.find(IEDKey);
	if (it != m_SCDDiffCompare->m_IEDDiffMap.end())
	{
		IEDStru *pIED = (IEDStru*)(it->second);
		for (uint j = 0; j < pIED->vGOOSECB.size(); j++)
		{
			ctrlBlockList->push_back(pIED->vGOOSECB.at(j).ID);
		}
		for (uint j = 0; j < pIED->vSVCB.size(); j++)
		{
			ctrlBlockList->push_back(pIED->vSVCB.at(j).ID);
		}
		return true;
	}
	return false;
}

/** 
  * @brief    GetCtrlBlockInfo
  * 
  * 根据控制块的Key获取控制块信息
  * 
  * @class    GraphicModelDiffImpl
  * @param    QString ctrlBlockKey    : 控制块的Key
  * @param    QCtrlBlockInfo& cbInfo  : 保存控制块信息
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/15
  * @remarks  
  */ 
bool GraphicModelDiffImpl::GetCtrlBlockInfo(QString ctrlBlockKey, QCtrlBlockInfo& cbInfo)
{
	if (ctrlBlockKey == "接收")
	{
		return false;
	}
	QString IEDKey = NameTranslator_GetIEDKeyFromSCDDiffResult(ctrlBlockKey);
	DifferMap::iterator it = m_SCDDiffCompare->m_IEDDiffMap.find(IEDKey);
	if (it != m_SCDDiffCompare->m_IEDDiffMap.end())
	{
		IEDStru *pIED = (IEDStru*)(it->second);
		for (uint j = 0; j < pIED->vGOOSECB.size(); j++)
		{
			GOOSECBStru *pGooseCB = &pIED->vGOOSECB.at(j);
			if (ctrlBlockKey == pGooseCB->ID)
			{
				SetAttributeValueFromBasicStru(cbInfo.apAPPID, pGooseCB->apAppID);
				SetAttributeValueFromBasicStru(cbInfo.apMAC, pGooseCB->MACAddr);
				SetAttributeValueFromBasicStru(cbInfo.APPID, pGooseCB->AppID);
				SetAttributeValueFromBasicStru(cbInfo.apVLAN_ID, pGooseCB->VLANID);
				SetAttributeValueFromBasicStru(cbInfo.apVLAN_PRIORITY, pGooseCB->VLANPriority);
				SetAttributeValueFromBasicStru(cbInfo.confRev, pGooseCB->ConfVersion);
				SetAttributeValueFromBasicStru(cbInfo.dataSetAddr, pGooseCB->DataSetAddr);
				SetAttributeValueFromBasicStru(cbInfo.dataSetName, pGooseCB->DataSetName);
				SetAttributeValueFromBasicStru(cbInfo.gocbRef, pGooseCB->GocbRef);
				SetAttributeValueFromBasicStru(cbInfo.maxTime, pGooseCB->MaxTime);
				SetAttributeValueFromBasicStru(cbInfo.minTime, pGooseCB->MinTime);
				SetAttributeValueFromBasicStru(cbInfo.name, pGooseCB->CBName);
				if (pGooseCB->DataSetCount.state == DIFFSTATE_REMOVED)
				{
					cbInfo.ASDU = pGooseCB->DataSetCount.otherAttributeName;
				}else {
					cbInfo.ASDU = pGooseCB->DataSetCount.baseAttributeName;
				}
				cbInfo.type = pGooseCB->type;
				cbInfo.DiffState = pGooseCB->state;
				cbInfo.diffDesc = SetGooseDiffDesc(pGooseCB);
				return true;
			}
		}
		for (uint j = 0; j < pIED->vSVCB.size(); j++)
		{
			SVCBStru *pSVCB = &pIED->vSVCB.at(j);
			if (ctrlBlockKey == pSVCB->ID)
			{
				SetAttributeValueFromBasicStru(cbInfo.apAPPID, pSVCB->apAppID);
				SetAttributeValueFromBasicStru(cbInfo.apMAC, pSVCB->MACAddr);
				SetAttributeValueFromBasicStru(cbInfo.APPID, pSVCB->AppID);
				SetAttributeValueFromBasicStru(cbInfo.apVLAN_ID, pSVCB->VLANID);
				SetAttributeValueFromBasicStru(cbInfo.apVLAN_PRIORITY, pSVCB->VLANPriority);
				if (pSVCB->DataSetCount.state == DIFFSTATE_REMOVED)
				{
					cbInfo.ASDU = pSVCB->DataSetCount.otherAttributeName;
				}else {
					cbInfo.ASDU = pSVCB->DataSetCount.baseAttributeName;
				}

				SetAttributeValueFromBasicStru(cbInfo.confRev, pSVCB->ConfVersion);
				SetAttributeValueFromBasicStru(cbInfo.dataSetAddr, pSVCB->DataSetAddr);
				SetAttributeValueFromBasicStru(cbInfo.dataSetName, pSVCB->DataSetName);
				SetAttributeValueFromBasicStru(cbInfo.name, pSVCB->CBName);
				SetAttributeValueFromBasicStru(cbInfo.smpRate, pSVCB->smpRate);
				cbInfo.type = pSVCB->type;
				cbInfo.DiffState = pSVCB->state;
				cbInfo.diffDesc = SetSVDiffDesc(pSVCB);
				return true;
			}
		}

	}
	return false;
}

/** 
  * @brief    SetGooseDiffDesc
  * 
  * 设置GOOSE差异信息
  * 
  * @class    GraphicModelDiffImpl
  * @param    GOOSECBStru *pGooseCB :
  * @return   QString
  * @author   zhang-xch
  * @date     2014/6/19
  * @remarks  
  */ 
QString GraphicModelDiffImpl::SetGooseDiffDesc(GOOSECBStru *pGooseCB)
{
	QString diffDesc = "";

	QString IED = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pGooseCB->ID.toStdString()));
	QString ObjName = QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pGooseCB->ID.toStdString()));
	diffDesc = QObject::tr("当前路径：") + IED + " " + ObjName + "\n\n" ;

	BasicStru *pBasicStru = &pGooseCB->apAppID;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pGooseCB->MACAddr;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pGooseCB->AppID;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pGooseCB->VLANID;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pGooseCB->VLANPriority;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pGooseCB->ConfVersion;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pGooseCB->DataSetAddr;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pGooseCB->DataSetName;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pGooseCB->GocbRef;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pGooseCB->MaxTime;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pGooseCB->MinTime;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pGooseCB->CBName;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}

	BasicIntStru *pBasicIntStru = &pGooseCB->DataSetCount;
	if (pBasicIntStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicIntStru->AttributeDesc + QObject::tr(":\n基准文件：") + QString::number(pBasicIntStru->baseAttributeName) + "\n比较文件：" + QString::number(pBasicIntStru->otherAttributeName) + "\n";
	}
	// 信号差异信息
	int iSignalCount = 0;
	for (uint i = 0; i < pGooseCB->vSignal.size(); i++)
	{
		if (pGooseCB->vSignal.at(i).state != DIFFSTATE_NORMAL)
		{
			iSignalCount++;
		}
	}
	if (iSignalCount > 0)
	{
		diffDesc = diffDesc + QObject::tr("GOOSE控制块下信号变更个数：") + QString::number(iSignalCount) + "\n";
	}

	return diffDesc;
}


/** 
  * @brief    SetSVDiffDesc
  * 
  * 设置SV差异信息
  * 
  * @class    GraphicModelDiffImpl
  * @param    SVCBStru *pSVCB :
  * @return   QString
  * @author   zhang-xch
  * @date     2014/6/19
  * @remarks  
  */ 
QString GraphicModelDiffImpl::SetSVDiffDesc(SVCBStru *pSVCB)
{
	QString diffDesc = "";

	QString IED = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pSVCB->ID.toStdString()));
	QString ObjName = QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pSVCB->ID.toStdString()));
	diffDesc = QObject::tr("当前路径：") + IED + " " + ObjName + "\n\n" ;

	BasicStru *pBasicStru = &pSVCB->apAppID;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pSVCB->MACAddr;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pSVCB->AppID;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pSVCB->VLANID;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pSVCB->VLANPriority;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pSVCB->ConfVersion;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pSVCB->DataSetAddr;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pSVCB->DataSetName;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pSVCB->smpRate;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pSVCB->CBName;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}

	BasicIntStru *pBasicIntStru = &pSVCB->DataSetCount;
	if (pBasicIntStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicIntStru->AttributeDesc + QObject::tr(":\n基准文件：") + QString::number(pBasicIntStru->baseAttributeName) + "\n比较文件：" + QString::number(pBasicIntStru->otherAttributeName) + "\n";
	}
	// 信号差异信息
	int iSignalCount = 0;
	for (uint i = 0; i < pSVCB->vSignal.size(); i++)
	{
		if (pSVCB->vSignal.at(i).state != DIFFSTATE_NORMAL)
		{
			iSignalCount++;
		}
	}
	if (iSignalCount > 0)
	{
		diffDesc = diffDesc + QObject::tr("SV控制块下信号变更个数：") + QString::number(iSignalCount) + "\n";
	}

	return diffDesc;
}

/** 
  * @brief    GetSlaveCtrlBlockByIEDKeys
  * 
  * 获取从slaver发送到maseter的信号所经过的slaver的控制块
  * 
  * @class    GraphicModelDiffImpl
  * @param    QString masterKey                : 目的装置的Key
  * @param    QString slaveKey                 : 源装置的Key
  * @param    QVector<QString>* ctrlBlockList  : 控制块列表
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/15
  * @remarks  
  */ 
bool GraphicModelDiffImpl::GetSlaveCtrlBlockByIEDKeys(QString masterKey, QString slaveKey, QVector<QString>* ctrlBlockList)
{
	QVector<QString> srcSignalKeyList;
	DifferMap::iterator it = m_SCDDiffCompare->m_IEDDiffMap.find(slaveKey);
	if (it != m_SCDDiffCompare->m_IEDDiffMap.end())
	{
		IEDStru *pIED = (IEDStru*)(it->second);

		for (uint j = 0; j < pIED->vGOOSECB.size(); j++)
		{
			GOOSECBStru *pGooseCB = &pIED->vGOOSECB.at(j);
			for (int k = 0; k < pGooseCB->vIEDList.size(); k++)
			{
				if (masterKey == pGooseCB->vIEDList.at(k))
				{
					ctrlBlockList->push_back(pGooseCB->ID);
					break;
				}
			}
		}
		for (uint j = 0; j < pIED->vSVCB.size(); j++)
		{
			SVCBStru *pSVCB = &pIED->vSVCB.at(j);
			for (int k = 0; k < pSVCB->vIEDList.size(); k++)
			{
				if (masterKey == pSVCB->vIEDList.at(k))
				{
					ctrlBlockList->push_back(pSVCB->ID);
					break;
				}
			}
		}

		// 判断是否存在从MasterKey发送到SlaverKey的虚链接，如果有，在CB列表中添加“接收”控制块
		for (uint j = 0; j < pIED->vVirLink.size(); j++)
		{
			VirtualLinkStur *pVLink = &pIED->vVirLink.at(j);
			QString srcIED = NameTranslator_GetIEDKeyFromSCDDiffResult(pVLink->srcSig);
			QString destIED = NameTranslator_GetIEDKeyFromSCDDiffResult(pVLink->destSig);
			if ((srcIED == masterKey) && (destIED == slaveKey))
			{
				ctrlBlockList->push_back("接收");
				break;
			}
		}

		if (ctrlBlockList->size() > 0)
		{
			return true;
		}

	}
	return false;
}

/** 
  * @brief    GetOutSignalsByCBKey
  * 
  * 获取masterIEDKey下cbKey控制块的输出信号
  * 
  * @class    GraphicModelDiffImpl
  * @param    QString masterIEDKey          : 提供的IED
  * @param    QString cbKey                 : 提供的控制块
  * @param    QVector<QString>* outSignals  : 输出信号列表
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/15
  * @remarks  
  */ 
bool GraphicModelDiffImpl::GetOutSignalsByCBKey(QString masterIEDKey, QString cbKey, QVector<QString>* outSignals)
{
	DifferMap::iterator it = m_SCDDiffCompare->m_IEDDiffMap.find(masterIEDKey);
	if (it != m_SCDDiffCompare->m_IEDDiffMap.end())
	{
		IEDStru *pIED = (IEDStru*)(it->second);

		for (uint j = 0; j < pIED->vGOOSECB.size(); j++)
		{
			GOOSECBStru *pGooseCB = &pIED->vGOOSECB.at(j);
			if (pGooseCB->ID == cbKey)
			{
				for (uint k = 0; k < pGooseCB->vSignal.size(); k++)
				{
					outSignals->push_back(pGooseCB->vSignal.at(k).ID);
				}
				return true;
			}
		}
		for (uint j = 0; j < pIED->vSVCB.size(); j++)
		{
			SVCBStru *pSVCB = &pIED->vSVCB.at(j);
			if (pSVCB->ID == cbKey)
			{
				for (uint k = 0; k < pSVCB->vSignal.size(); k++)
				{
					outSignals->push_back(pSVCB->vSignal.at(k).ID);
				}
				return true;
			}
		}
	}
	return false;
}

/** 
  * @brief    GetPhyPortInfo
  * 
  * 获取物理端口信息;
  * 
  * @class    GraphicModelDiffImpl
  * @param    QString pPortKey         : 物理端口的Key
  * @param    QPhyPortInfo& pPortInfo  : 物理端口的信息
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/15
  * @remarks  
  */ 
bool GraphicModelDiffImpl::GetPhyPortInfo(QString pPortKey, QPhyPortInfo& pPortInfo)
{
	DifferMap::iterator it = m_SCDDiffCompare->m_PhyPortDiffMap.find(pPortKey);
	if (it != m_SCDDiffCompare->m_PhyPortDiffMap.end())
	{
		PhysicsPortStru* pPhyPort = (PhysicsPortStru*)(it->second);
		pPortInfo.ID = pPhyPort->ID;
		pPortInfo.DiffState = pPhyPort->state;
		SetAttributeValueFromBasicStru(pPortInfo.pIndex, pPhyPort->pIndex);
		SetAttributeValueFromBasicStru(pPortInfo.plug, pPhyPort->plug);
		SetAttributeValueFromBasicStru(pPortInfo.type, pPhyPort->type);
		pPortInfo.diffDesc = SetPhyPortDiffDesc(pPhyPort);
		return  true;
	}
	return false;
}

/** 
  * @brief    SetPhyPortDiffDesc
  * 
  * 设置物理端口差异信息
  * 
  * @class    GraphicModelDiffImpl
  * @param    PhysicsPortStru* pPhyPort :
  * @return   QString
  * @author   zhang-xch
  * @date     2014/6/19
  * @remarks  
  */ 
QString GraphicModelDiffImpl::SetPhyPortDiffDesc(PhysicsPortStru* pPhyPort)
{
	QString diffDesc = "";

	QString IED = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pPhyPort->ID.toStdString()));
	QString ObjName = QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pPhyPort->ID.toStdString()));
	diffDesc = QObject::tr("当前路径：") + IED + " " + ObjName + "\n\n" ;

	BasicStru *pBasicStru;

	pBasicStru = &pPhyPort->pIndex;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}
	pBasicStru = &pPhyPort->plug;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}

	pBasicStru = &pPhyPort->type;
	if (pBasicStru->state != DIFFSTATE_NORMAL)
	{
		diffDesc = diffDesc + pBasicStru->AttributeDesc + QObject::tr(":\n基准文件：") + pBasicStru->baseAttributeName + "\n比较文件：" + pBasicStru->otherAttributeName + "\n";
	}

	return diffDesc;
}

/** 
  * @brief    GetPhyPortListByIED
  * 
  * 根据IED获取物理端口;
  * 
  * @class    GraphicModelDiffImpl
  * @param    QString IEDKey               : IED的Key
  * @param    QVector<QString>* pPortList  : 物理端口存放列表
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/15
  * @remarks  
  */ 
bool GraphicModelDiffImpl::GetPhyPortListByIED(QString IEDKey, QVector<QString>* pPortList)
{
	DifferMap::iterator it = m_SCDDiffCompare->m_IEDDiffMap.find(IEDKey);
	if (it != m_SCDDiffCompare->m_IEDDiffMap.end())
	{
		IEDStru *pIED = (IEDStru*)(it->second);
		for (uint j = 0; j < pIED->vPortStru.size(); j++)
		{
			pPortList->push_back(pIED->vPortStru.at(j).ID);
		}
		return true;
	}
	return false;
}

/** 
  * @brief    GetPhyPortKeyByInSignalID
  * 
  * 根据输入信号获取物理端口; 
	此处获取的端口原则为：base中有，则使用base的Port，否则使用other的Port
  * 
  * @class    GraphicModelDiffImpl
  * @param    QString signalKey : 输入信号
  * @param    QString* pPortKey : 信号对应的物理端口的Key
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/15
  * @remarks  
  */ 
bool GraphicModelDiffImpl::GetPhyPortListByInSignalID(QString signalKey, QVector<QString> &pPortList)
{
	DifferMap::iterator it = m_SCDDiffCompare->m_InSigDiffMap.find(signalKey);
	if (it != m_SCDDiffCompare->m_InSigDiffMap.end())
	{
		SignalStru *pSignal = (SignalStru *)(it->second);

		QString dest;
		SetAttributeValueFromBasicStru(dest, pSignal->PortIndex);
				
		pPortList = m_NameTranslator.GetSignalPortListFromQString(dest);
		return true;
	}
	return false;
}

/** 
  * @brief    GetPhyPortKeyByOutSignalID
  * 
  * 根据输出信号获取端口;
	此处获取的端口原则为：base中有，则使用base的Port，否则使用other的Port
  * 
  * @class    GraphicModelDiffImpl
  * @param    QString signalKey : 输出信号
  * @param    QString* pPortKey : 信号对应的物理端口的Key
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
bool GraphicModelDiffImpl::GetPhyPortKeyByOutSignalID(QString signalKey, QString* pPortKey)
{
	// 注释By zxc 2014.12.10
	return true;
}

/** 
  * @brief    GetIEDListByLogicalRealitionWithCurrentIED
  * 
  * 获得IED的关联IED列表（逻辑）
	直接从SCLModel的模型中获取比较
  * 
  * @class    GraphicModelDiffImpl
  * @param    QString IEDKey                      : IED的Key
  * @param    QVector<QString>* associatedIEDKeys : IED的关联IED列表
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
bool GraphicModelDiffImpl::GetIEDListByLogicalRealitionWithCurrentIED(QString IEDKey,QVector<QString>* associatedIEDKeys)
{
	// 根据SubStationKey和IEDKey拼出完整Key，去SCLModel中取得关联列表
	QString baseIEDKey = m_SCDDiffCompare->m_BaseSubStationKey + "." + IEDKey;
	QVector<QString> baseAssociatedIEDKeys;
	SCLModel->GetIEDListByLogicalRealitionWithCurrentIED(baseIEDKey, &baseAssociatedIEDKeys);
	// 根据SubStationKey和IEDKey拼出完整Key，去SCLModel中取得关联列表
	QString otherIEDKey = m_SCDDiffCompare->m_OtherSubStationKey + "." + IEDKey;
	QVector<QString> otherAssociatedIEDKeys;
	SCLModel->GetIEDListByLogicalRealitionWithCurrentIED(otherIEDKey, &otherAssociatedIEDKeys);

	// 对两个IED列表进行比较，筛选
	for (int i = 0; i < baseAssociatedIEDKeys.size(); i++)
	{
		bool isFound = false;
		QString baseIED = NameTranslator_GetKeyNameWithoutSubStation(baseAssociatedIEDKeys.at(i));
		for (int j = 0; j < otherAssociatedIEDKeys.size(); j++)
		{
			QString otherIED = NameTranslator_GetKeyNameWithoutSubStation(otherAssociatedIEDKeys.at(j));
			if (baseIED == otherIED)
			{
				associatedIEDKeys->push_back(baseIED);
				otherAssociatedIEDKeys.remove(j); // 将相同的在other中去掉
				isFound = true;
				break;
			}
		}

		// base独有的IED列表
		if (!isFound)
		{
			associatedIEDKeys->push_back(baseIED);
		}
	}

	// other独有的IED列表
	for (int i = 0; i < otherAssociatedIEDKeys.size(); i++)
	{
		QString otherIED = NameTranslator_GetKeyNameWithoutSubStation(otherAssociatedIEDKeys.at(i));
		associatedIEDKeys->push_back(otherIED);
	}

	return true;
}

/** 
  * @brief    SetAttributeValueFromBasicStru
  * 
  * 根据比较结果设置属性值
  * 
  * @class    GraphicModelDiffImpl
  * @param    QString &DestString : 被设置的属性值
  * @param    BasicStru &SrcStru  : 比较结果
  * @return   void
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
void GraphicModelDiffImpl::SetAttributeValueFromBasicStru(QString &DestString, BasicStru &SrcStru)
{
	DestString.clear();
	if (SrcStru.state == DIFFSTATE_ADDED)
	{
		DestString = SrcStru.otherAttributeName;
	}else {
		if (SrcStru.baseAttributeName.size() != 0)
		{
			DestString = SrcStru.baseAttributeName;
		}else 
		{
			DestString = SrcStru.otherAttributeName;
		}
		
	}
}

/** 
  * @brief    NameTranslator_GetKeyNameWithoutSubStation
  * 
  * 通过NameTranslator，将实时库中的Key转换为差异化比较所用的Key
  * 
  * @class    GraphicModelDiffImpl
  * @param    QString qString : 实时库中的Key
  * @return   QString
  * @author   zhang-xch
  * @date     2014/5/15
  * @remarks  
  */ 
QString GraphicModelDiffImpl::NameTranslator_GetKeyNameWithoutSubStation(QString qString)
{
	return QString::fromStdString(m_NameTranslator.GetKeyNameWithoutSubStation(qString.toStdString()));
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
QString GraphicModelDiffImpl::NameTranslator_GetIEDKeyFromSCDDiffResult(QString qString)
{
	return QString::fromStdString(m_NameTranslator.GetIEDKeyFromSCDDiffResult(qString.toStdString()));
}
