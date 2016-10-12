#include "SCDDifferCompare.h"
#include "SCLModelSvrAdapter.h"
#include "QSettings"
#include "windows.h"

SCDDiffCompare *SCDDiffCompare::m_pSCDInstance = NULL;

SCDDiffCompare::SCDDiffCompare()
{
	SCLModel = SCLModelAdapter::GetInstance();
	SetConfigureInfo();
	bIEDSelected = true;
	bSVCBSelected = true;
	bSVDataSetSelected = true;
	bGooseCBSelected = true;
	bGooseDataSetSelected = true;
	bInputsSelected = true;
	bPhyPortSelected = true;

	m_CompareSpeed = 0;
}

SCDDiffCompare::~SCDDiffCompare()
{

};

bool SCDDiffCompare::GetFileInfoSelectedState()
{
	return ConfigureInfo[FILEINFO_VERSION] || ConfigureInfo[FILEINFO_REVERSION];
}

bool SCDDiffCompare::GetIEDSelectedState()
{
	return IsIEDSelect();
}

/** 
  * @brief    BuildSignalCache
  * 
  * �ж�DLL�ӿ��Ƿ���ڣ�������������ڴ棬����ֱ�ӷ���
  * 
  * @class    SCDDiffCompare
  * @param    const std::string file1 : ��׼�ļ�
  * @param    const std::string file2 : �Ƚ��ļ�
  * @return   bool
  * @author   zhang-xch
  * @date     2015/6/8
  * @remarks  
  */ 
bool    SCDDiffCompare::BuildSignalCache(const std::string &strFileMark1, const std::string &strFileMark2)
{
    //std::string (SCLModelAdapterForClient:: *p)(const std::string& subStationKey) = NULL;

    //(PROC &)p = GetProcAddress(GetModuleHandleA("SCLDataAccessComponent.dll"), "?GetSubstationPrivateXMLInfo@SCLModelAdapterForClient@@UAE?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@ABV23@@Z");


    //(((SCLModelAdapterForClient *)NULL)->*p)("aaa");
// 
//     (PROC &)p = GetProcAddress(GetModuleHandleA("SCLDataAccessComponent.dll"), "?GetSubstationImportTime@SCLModelAdapterForClient@@QAE?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@ABV23@@Z");
// 
// 
//     (((SCLModelAdapterForClient *)NULL)->*p)("aaa");
    typedef bool (* PBuildSignalCache)(const std::string&);
    HINSTANCE hInst;
    BOOL  fFreeResult = false;
    hInst = GetModuleHandleA("SCLDataAccessComponent.dll");
    PBuildSignalCache BuildSignalCache = (PBuildSignalCache)GetProcAddress(hInst, 
        "?BuildSignalCache@SclDacLib@@SA_NABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z");//��ȡDll�ĵ�������
    if(!BuildSignalCache)
    {
        return false;
    }
    else
    {
        BuildSignalCache(strFileMark1);
        BuildSignalCache(strFileMark2);
        return true;
    }
}

/** 
  * @brief    GetTotalDiff
  * 
  * ȡ��SCD�ļ��Ĳ��컯�ȽϽ��
  * 
  * @class    SCDDiffCompare
  * @param    QString baseSubStationKey   :��׼�ļ��Ĺؼ���
  * @param    QString otherSubStationKey  :�Ƚ��ļ��Ĺؼ���
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/8
  * @remarks  
  */ 
bool SCDDiffCompare::GetTotalDiff(QString baseSubStationKey, QString otherSubStationKey)
{
    // ����ǰж��ȫ��
    std::vector<std::string> loadList = SclDacLib::GetLoadedFileMarks();
    for(uint i=0; i<loadList.size(); i++)
    {
        SclDacLib::UnloadScdFile(loadList.at(i));
    }

    qDebug() << "SCDDiffCompare::GetTotalDiff---------Start";
	//chenxin add 2014.10.22 --->
	SCLNameTranslator translator;
	m_CompareSpeed = 2;
	string baseSubstationID = translator.GetNameFromKey(baseSubStationKey.toLocal8Bit().constData());
	string otherSubstationID = translator.GetNameFromKey(otherSubStationKey.toLocal8Bit().constData());
	if (!SclDacLib::LoadScdFile(baseSubstationID))
	{
		qDebug()<<"SCDDiffCompare::GetTotalDiff    LoadScdFile(baseSubstationID)  ʧ��";
		return false;
	}
	m_CompareSpeed = 7;
	if (!SclDacLib::LoadScdFile(otherSubstationID))
	{
		qDebug()<<"SCDDiffCompare::GetTotalDiff    LoadScdFile(otherSubstationID)  ʧ��";
		return false;
	}
    qDebug() << "SCDDiffCompare::BuildSignalCache---------Start";
    QSettings *subconfig = new QSettings("../cfg/SubStationInfo.ini", QSettings::IniFormat);
    bool isBuildSignalCache = subconfig->value("config/IsBuildSignalCache").toBool();
    if (isBuildSignalCache)
    {
        BuildSignalCache(baseSubstationID, otherSubstationID);
    }
    qDebug() << "SCDDiffCompare::BuildSignalCache---------End";
	m_CompareSpeed = 12;
	//chenxin add 2014.10.22 <---
	
	m_SCDDiffResult.vIEDStru.clear();

	//��ȡ�û�������Ϣ
	QSettings *config = new QSettings("../cfg/CompareConf", QSettings::IniFormat);
	for (int i = 0; i < ITEMCOUNT; i++)
	{
		ConfigureInfo[i] = config->value("config/" + m_ConfPair[i].confEnDesc).toBool();
	}

	bIEDSelected = IsIEDSelect();
	bSVCBSelected = IsSVCBSelected();
	bSVDataSetSelected = IsSVDataSetSelected();
	bGooseCBSelected = IsGOOSECBSelected();
	bGooseDataSetSelected = IsGOOSEDataSetSelected();
	bInputsSelected = IsInputsSelected();
	bPhyPortSelected = IsPhyPortSelected();
	m_BaseSubStationKey = baseSubStationKey;
	m_OtherSubStationKey = otherSubStationKey;
	QSubStationInfo baseSubStationInfo, otherSubStationInfo;

	if (ConfigureInfo[FILEINFO_VERSION] || 
		ConfigureInfo[FILEINFO_REVERSION]) //|| ConfigureInfo[FILEINFO_CRC])
	{
		OutputDebugStringA("GetTotalDiff		1\n");
		SCLModel->GetSubstationInfo(baseSubStationKey, baseSubStationInfo);
		SCLModel->GetSubstationInfo(otherSubStationKey, otherSubStationInfo);
		m_SCDDiffResult.sFileInfo.basefileName = baseSubStationInfo.fileName;
		m_SCDDiffResult.sFileInfo.baseSubstationName = baseSubStationInfo.Name;
		// m_SCDDiffResult.sFileInfo.basefileTime = QString::fromStdString(m_NameTranslator.GetFileTimeFromKey(baseSubStationKey.toStdString()));
		m_SCDDiffResult.sFileInfo.otherfileName = otherSubStationInfo.fileName;
		m_SCDDiffResult.sFileInfo.otherSubstationName = otherSubStationInfo.Name;
		// m_SCDDiffResult.sFileInfo.otherfileTime = QString::fromStdString(m_NameTranslator.GetFileTimeFromKey(otherSubStationKey.toStdString()));
		OutputDebugStringA("GetTotalDiff		2\n");
		if (GetFileInfoDiff(baseSubStationInfo, otherSubStationInfo))
		{
			m_SCDDiffResult.sFileInfo.state = DIFFSTATE_EDITED;
		}
		QVector<QHItemInfo> baseHItemList, otherHItemList;
		SCLModel->GetSCDHItem(baseSubStationKey, baseHItemList);
		SCLModel->GetSCDHItem(otherSubStationKey, otherHItemList);
		// QVector<QHItemInfo>::iterator bastIt = baseHItemList.last();
		if (baseHItemList.size() > 0)
		{
			QHItemInfo bastIt = baseHItemList.last();
			m_SCDDiffResult.sFileInfo.baseSubstationHitmTime = bastIt.when;
		}
		if (otherHItemList.size() > 0)
		{
			QHItemInfo otherIt = otherHItemList.last();
			m_SCDDiffResult.sFileInfo.otherSubstationHitmTime = otherIt.when;
		}		
	}
	m_CompareSpeed = 15;
	if (bIEDSelected)
	{
		QVector<QString> basePanelKeyList, otherPanelKeyList;
		SCLModel->GetPanelList(baseSubStationKey, &basePanelKeyList);
		SCLModel->GetPanelList(otherSubStationKey, &otherPanelKeyList);

		QVector<QString> baseIEDList, otherIEDList;
		// ȡ��base�ļ���IEDList
		for (int i = 0; i < basePanelKeyList.size(); i++)
		{
			SCLModel->GetIEDList(basePanelKeyList.at(i), &baseIEDList);
		}
		// ȡ��other�ļ���IEDList
		for (int i = 0; i < otherPanelKeyList.size(); i++)
		{
			SCLModel->GetIEDList(otherPanelKeyList.at(i), &otherIEDList);
		}
		// ʹ��ǰ�����
		m_SCDDiffResult.vIEDStru.clear();
		VIEDStru vStrIED;
		GetIedDiff(baseIEDList, otherIEDList, vStrIED);
		m_SCDDiffResult.vIEDStru = vStrIED;

	}
	m_CompareSpeed = 96;
	CreatMap();

	m_CompareSpeed = 100;
    qDebug() << "SCDDiffCompare::GetTotalDiff---------End";
	return true;
}

void SCDDiffCompare::ClearMap()
{
	m_IEDDiffMap.clear();
	m_CBDiffMap.clear();
	m_PhyPortDiffMap.clear();
	m_VLinkDiffMap.clear();
	m_InputsDiffMap.clear();
	m_InSigDiffMap.clear();		
	m_OutSigDiffMap.clear();

	m_IEDUnNormalDiffMap.clear();
	m_SVCBUnNormalDiffMap.clear();
	m_GOOSECBUnNormalDiffMap.clear();
	m_InputsUnNormalDiffMap.clear();
	m_InputsOtherUnNormalDiffMap.clear();
	m_PhyPortUnNormalDiffMap.clear();
	m_OutSigUnNormalDiffMap.clear();
	m_InSigUnNormalDiffMap.clear();
}

/** 
  * @brief    CreatMap
  * 
  * ����Map
  * 
  * @class    SCDDiffCompare
  * @return   void
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
void SCDDiffCompare::CreatMap()
{
	ClearMap();

	for (uint i = 0; i < m_SCDDiffResult.vIEDStru.size(); i++)
	{
		// ����IEDMap
		IEDStru *pIEDStru = &m_SCDDiffResult.vIEDStru.at(i);
		m_IEDDiffMap.insert(make_pair(pIEDStru->ID, (void*)(pIEDStru)));

		if (pIEDStru->state != DIFFSTATE_NORMAL)
		{
			m_IEDUnNormalDiffMap.insert(make_pair(pIEDStru->ID, (void*)(pIEDStru)));
		}

		// �������ƿ�Map������ź�Map
		for (uint m = 0; m < pIEDStru->vSVCB.size(); m++)
		{
			SVCBStru *pSVCBStru = &pIEDStru->vSVCB.at(m);
			m_CBDiffMap.insert(make_pair(pSVCBStru->ID, (void*)(pSVCBStru)));

			if (pSVCBStru->state != DIFFSTATE_NORMAL && pIEDStru->state == DIFFSTATE_EDITED)
			{
				m_SVCBUnNormalDiffMap.insert(make_pair(pSVCBStru->ID, (void*)(pSVCBStru)));
			}

			for (uint n = 0; n < pSVCBStru->vSignal.size(); n++)
			{
				SignalStru *pSignalStru = &pSVCBStru->vSignal.at(n);
				m_OutSigDiffMap.insert(make_pair(pSignalStru->ID, (void*)(pSignalStru)));

				if (pSignalStru->state != DIFFSTATE_NORMAL  && pIEDStru->state == DIFFSTATE_EDITED)
				{
					m_OutSigUnNormalDiffMap.insert(make_pair(pSignalStru->ID, (void*)(pSignalStru)));
				}
			}
		}

		// �������ƿ�Map������ź�Map
		for (uint m = 0; m < pIEDStru->vGOOSECB.size(); m++)
		{
			GOOSECBStru *pGooseCBStru = &pIEDStru->vGOOSECB.at(m);
			m_CBDiffMap.insert(make_pair(pGooseCBStru->ID, (void*)(pGooseCBStru)));

			if (pGooseCBStru->state != DIFFSTATE_NORMAL && pIEDStru->state == DIFFSTATE_EDITED)
			{
				m_GOOSECBUnNormalDiffMap.insert(make_pair(pGooseCBStru->ID, (void*)(pGooseCBStru)));
			}

			for (uint n = 0; n < pGooseCBStru->vSignal.size(); n++)
			{
				SignalStru *pSignalStru = &pGooseCBStru->vSignal.at(n);
				m_OutSigDiffMap.insert(make_pair(pSignalStru->ID, (void*)(pSignalStru)));

				if (pSignalStru->state != DIFFSTATE_NORMAL && pIEDStru->state == DIFFSTATE_EDITED)
				{
					m_OutSigUnNormalDiffMap.insert(make_pair(pSignalStru->ID, (void*)(pSignalStru)));
				}
			}
		}

		// ����InputsMap�������ź�Map
		for (uint m = 0; m < pIEDStru->vInput.size(); m++)
		{
			InputStru *pInputStru = &pIEDStru->vInput.at(m);
			for (uint n = 0; n < pInputStru->vExtRef.size(); n++)
			{
				ExtRefStru *pExtRefStru = &pInputStru->vExtRef.at(n);
				DifferMap::iterator it = m_InputsDiffMap.find(pExtRefStru->ID);
				if (it != m_InputsDiffMap.end())
				{
					m_InputsOtherDiffMap.insert(make_pair(pExtRefStru->ID, (void*)(pExtRefStru)));

					if (pExtRefStru->state != DIFFSTATE_NORMAL && pIEDStru->state == DIFFSTATE_EDITED)
					{
						m_InputsOtherUnNormalDiffMap.insert(make_pair(pExtRefStru->ID, (void*)(pExtRefStru)));
					}

				}
				else 
				{
					m_InputsDiffMap.insert(make_pair(pExtRefStru->ID, (void*)(pExtRefStru)));

					if (pExtRefStru->state != DIFFSTATE_NORMAL && pIEDStru->state == DIFFSTATE_EDITED)
					{
						m_InputsUnNormalDiffMap.insert(make_pair(pExtRefStru->ID, (void*)(pExtRefStru)));
					}
				}
				
				SignalStru *pSig = &pExtRefStru->InSigInfo;
				m_InSigDiffMap.insert(make_pair(pSig->ID, (void*)(pSig)));

				if (pSig->state != DIFFSTATE_NORMAL && pIEDStru->state == DIFFSTATE_EDITED)
				{
					m_InSigUnNormalDiffMap.insert(make_pair(pSig->ID, (void*)(pSig)));
				}
			}
		}

		// ��������˿�Map
		for (uint m = 0; m < pIEDStru->vPortStru.size(); m++)
		{
			PhysicsPortStru *pPhyPort = &pIEDStru->vPortStru.at(m);
			m_PhyPortDiffMap.insert(make_pair(pPhyPort->ID, (void*)(pPhyPort)));

			if (pPhyPort->state != DIFFSTATE_NORMAL && pIEDStru->state == DIFFSTATE_EDITED)
			{
				m_PhyPortUnNormalDiffMap.insert(make_pair(pPhyPort->ID, (void*)(pPhyPort)));
			}
		}

		// ����������Map
		for (uint m = 0; m < pIEDStru->vVirLink.size(); m++)
		{
			VirtualLinkStur *pVirLink = &pIEDStru->vVirLink.at(m);
			m_VLinkDiffMap.insert(make_pair(pVirLink->srcSig, (void*)(pVirLink)));
		}
	}
}

/** 
  * @brief    GetFileInfoDiff
  * 
  * ȡ���ļ���Ϣ����
  * 
  * @class    SCDDiffCompare
  * @param    QSubStationInfo baseSubStationInfo   :��׼�ļ��ı��վ��Ϣ
  * @param    QSubStationInfo otherSubStationInfo  :�Ƚ��ļ��ı��վ��Ϣ
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/8
  * @remarks  
  */ 
bool SCDDiffCompare::GetFileInfoDiff(QSubStationInfo baseSubStationInfo, QSubStationInfo otherSubStationInfo)
{
	bool isEdit = false;

	if (SetValue(FILEINFO_VERSION, m_SCDDiffResult.sFileInfo.version, baseSubStationInfo.version, otherSubStationInfo.version, DIFFSTATE_NORMAL))
	{
		isEdit = true;
	}

	if (SetValue(FILEINFO_REVERSION, m_SCDDiffResult.sFileInfo.reVersion, baseSubStationInfo.reversion, otherSubStationInfo.reversion, DIFFSTATE_NORMAL))
	{
		isEdit = true;
	}

	// TODO
	// �ļ���Ϣ�е�CRCУ����Ƚϣ��ݲ�����
	if (SetValue(FILEINFO_CRC, m_SCDDiffResult.sFileInfo.crc, baseSubStationInfo.crc, otherSubStationInfo.crc, DIFFSTATE_NORMAL))
	{
		isEdit = true;
	}


	return isEdit;
}

/** 
  * @brief    GetIedDiff
  * 
  * ȡ��IED�Ĳ���
  * 
  * @class    SCDDiffCompare
  * @param    QVector<QString> &baseIEDList   :��׼�ļ��е�IED�б�
  * @param    QVector<QString> &otherIEDList  :�Ƚ��ļ��е�IED�б�
  * @param    VIEDStru &vStrIED               :IED�ȽϽ���洢�ṹ
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/8
  * @remarks  
  */ 
bool SCDDiffCompare::GetIedDiff(QVector<QString> &baseIEDList, QVector<QString> &otherIEDList, VIEDStru &vStrIED)
{
	SearchMap otherIEDMap;
	for (int j = 0; j < otherIEDList.size(); j++)
	{
		QIEDInfo otherIEDInfo;
		SCLModel->GetIEDInfo(otherIEDList.at(j), otherIEDInfo);
		otherIEDMap.insert(make_pair(otherIEDInfo.name, otherIEDList.at(j)));
	}

	for (int i = 0; i < baseIEDList.size(); i++)
	{
		float iSpeed = i;
		m_CompareSpeed = 15 + iSpeed/baseIEDList.size() * 60;
		IEDStru strIED;
		QIEDInfo baseIEDInfo, otherIEDInfo;
		bool isFound = false; // �Ƿ���other���ҵ�base��ָ��IED��

		SCLModel->GetIEDInfo(baseIEDList.at(i), baseIEDInfo);

		SearchMap::iterator it = otherIEDMap.find(baseIEDInfo.name);
		if (it != otherIEDMap.end())
		{
			QIEDInfo otherIEDInfo;
			SCLModel->GetIEDInfo(it->second, otherIEDInfo);

			if (SetValue(IED_DESC, strIED.IEDDesc, baseIEDInfo.desc, otherIEDInfo.desc, DIFFSTATE_NORMAL))
			{
				strIED.state = DIFFSTATE_EDITED;
			}			
			if (SetValue(IED_TYPE, strIED.IEDType, baseIEDInfo.type, otherIEDInfo.type, DIFFSTATE_NORMAL))
			{
				strIED.state = DIFFSTATE_EDITED;
			}
			if (SetValue(IED_MANU, strIED.IEDManu, baseIEDInfo.manu, otherIEDInfo.manu, DIFFSTATE_NORMAL))
			{
				strIED.state = DIFFSTATE_EDITED;
			}
			// TODO
			if (SetValue(IED_CRC, strIED.IEDCRC, baseIEDInfo.crc, otherIEDInfo.crc, DIFFSTATE_NORMAL))
			{
				strIED.state = DIFFSTATE_EDITED;
			}
			strIED.IEDName = baseIEDInfo.name;
			strIED.ID = NameTranslator_GetKeyNameWithoutSubStation(baseIEDList.at(i));

			QVector<QString> baseCBKey, otherCBKey;

			SCLModel->GetCtrlBlockListByIED(baseIEDList.at(i), &baseCBKey);
			SCLModel->GetCtrlBlockListByIED(it->second, &otherCBKey);
			if (GetControlBlockDiff(baseCBKey, otherCBKey, strIED, DIFFSTATE_NORMAL))
			{
				strIED.state = DIFFSTATE_EDITED;
			}

			// ��������
			QVector<QVirtualLink> baseVLink, otherVLink;
			SCLModel->GetVritualLinksOfIED(baseIEDList.at(i), typeAll_, &baseVLink);
			SCLModel->GetVritualLinksOfIED(it->second, typeAll_, &otherVLink);
			if (GetInputsDiff(baseVLink, otherVLink, strIED, DIFFSTATE_NORMAL))
			{
				strIED.state = DIFFSTATE_EDITED;
			}
			QVector<QVirtualLink> BaseVLink, OtherVLink;
			SCLModel->GetVritualLinksOfIED(baseIEDList.at(i), typeAll_, &BaseVLink);
			SCLModel->GetVritualLinksOfIED(it->second, typeAll_, &OtherVLink);
			GetVLinkDiff(BaseVLink, OtherVLink, strIED, DIFFSTATE_NORMAL);

			// ����˿��б�
			QVector<QString> basePhyPortList, otherPhyPortList;
			SCLModel->GetPhyPortListByIED(baseIEDList.at(i), &basePhyPortList);
			SCLModel->GetPhyPortListByIED(it->second, &otherPhyPortList);
			if (GetPhyPortDiff(basePhyPortList, otherPhyPortList, strIED, DIFFSTATE_NORMAL))
			{
				strIED.state = DIFFSTATE_EDITED;
			}
			otherIEDMap.erase(it);
		}
		else
		{
			strIED.state = DIFFSTATE_REMOVED;
			SetValue(IED_DESC, strIED.IEDDesc, baseIEDInfo.desc, "", DIFFSTATE_REMOVED);
			SetValue(IED_TYPE, strIED.IEDType, baseIEDInfo.type, "", DIFFSTATE_REMOVED);
			SetValue(IED_MANU, strIED.IEDManu, baseIEDInfo.manu, "", DIFFSTATE_REMOVED);
			// TODO
			SetValue(IED_CRC, strIED.IEDCRC, baseIEDInfo.crc, "", DIFFSTATE_REMOVED);
			strIED.IEDName = baseIEDInfo.name;
			strIED.ID = NameTranslator_GetKeyNameWithoutSubStation(baseIEDList.at(i));

			QVector<QString> baseCBKey, otherCBKey;

			SCLModel->GetCtrlBlockListByIED(baseIEDList.at(i), &baseCBKey);
			GetControlBlockDiff(baseCBKey, otherCBKey, strIED, DIFFSTATE_REMOVED);

			// ��������
			QVector<QVirtualLink> baseVLink, otherVLink;
			SCLModel->GetVritualLinksOfIED(baseIEDList.at(i), typeAll_, &baseVLink);
			GetInputsDiff(baseVLink, otherVLink, strIED, DIFFSTATE_REMOVED);
			QVector<QVirtualLink> BaseVLink, OtherVLink;
			SCLModel->GetVritualLinksOfIED(baseIEDList.at(i), typeAll_, &BaseVLink);
			GetVLinkDiff(BaseVLink, OtherVLink, strIED, DIFFSTATE_REMOVED);

			// ����˿��б�
			QVector<QString> basePhyPortList, otherPhyPortList;
			SCLModel->GetPhyPortListByIED(baseIEDList.at(i), &basePhyPortList);
			GetPhyPortDiff(basePhyPortList, otherPhyPortList, strIED, DIFFSTATE_REMOVED);
		}
		vStrIED.push_back(strIED);
	}
	m_CompareSpeed = 75;
	float iSpeed = 0;
	for (SearchMap::iterator it = otherIEDMap.begin(); it != otherIEDMap.end(); it++)
	{

		m_CompareSpeed = 75 + iSpeed/baseIEDList.size() * 20;
		iSpeed++;
		
		IEDStru strIED;
		strIED.state = DIFFSTATE_ADDED;

		QIEDInfo otherIEDInfo;
		SCLModel->GetIEDInfo(it->second, otherIEDInfo);

		SetValue(IED_DESC, strIED.IEDDesc, "", otherIEDInfo.desc, DIFFSTATE_ADDED);
		SetValue(IED_TYPE, strIED.IEDType, "", otherIEDInfo.type, DIFFSTATE_ADDED);
		SetValue(IED_MANU, strIED.IEDManu, "", otherIEDInfo.manu, DIFFSTATE_ADDED);
		// TODO
		SetValue(IED_CRC, strIED.IEDCRC, "", otherIEDInfo.crc, DIFFSTATE_ADDED);
		strIED.IEDName = otherIEDInfo.name;
		strIED.ID = NameTranslator_GetKeyNameWithoutSubStation(it->second);

		QVector<QString> baseCBKey, otherCBKey;
		SCLModel->GetCtrlBlockListByIED(it->second, &otherCBKey);
		GetControlBlockDiff(baseCBKey, otherCBKey, strIED, DIFFSTATE_ADDED);

		// ��������
		QVector<QVirtualLink> baseVLink, otherVLink;
		SCLModel->GetVritualLinksOfIED(it->second, typeAll_, &otherVLink);
		GetInputsDiff(baseVLink, otherVLink, strIED, DIFFSTATE_ADDED);
		QVector<QVirtualLink> BaseVLink, OtherVLink;
		SCLModel->GetVritualLinksOfIED(it->second, typeAll_, &OtherVLink);
		GetVLinkDiff(BaseVLink, OtherVLink, strIED, DIFFSTATE_ADDED);
		// ����˿��б�
		QVector<QString> basePhyPortList, otherPhyPortList;
		SCLModel->GetPhyPortListByIED(it->second, &otherPhyPortList);
		GetPhyPortDiff(basePhyPortList, otherPhyPortList, strIED, DIFFSTATE_ADDED);
		vStrIED.push_back(strIED);
	}
	return true;
}

bool SCDDiffCompare::GetVLinkDiff(QVector<QVirtualLink> &baseInputs, QVector<QVirtualLink> &otherInputs, IEDStru& strIED, DiffStates parentState)
{
	bool isEdit = false;
	for (int i = 0; i < baseInputs.size(); i++)
	{
		VirtualLinkStur strVirLink;
		bool isFound = false;
		string baseSrcSig = m_NameTranslator.GetKeyNameWithoutSubStation(baseInputs.at(i).srcSignal.toStdString());
		string baseDestSig = m_NameTranslator.GetKeyNameWithoutSubStation(baseInputs.at(i).destSignal.toStdString());
		string baseVLink = baseSrcSig + baseDestSig;

			for (int j = 0; j < otherInputs.size(); j++)
			{
				string otherSrcSig = m_NameTranslator.GetKeyNameWithoutSubStation(otherInputs.at(j).srcSignal.toStdString());
				string otherDestSig = m_NameTranslator.GetKeyNameWithoutSubStation(otherInputs.at(j).destSignal.toStdString());
				string otherVLink = otherSrcSig + otherDestSig;

				if (baseVLink == otherVLink)
				{
					strVirLink.state = DIFFSTATE_NORMAL;
					strVirLink.srcSig = QString::fromStdString(baseSrcSig);
					strVirLink.destSig = QString::fromStdString(baseDestSig);

					otherInputs.remove(j);
					isFound = true;
					break;
				}
			}
			if (!isFound)
			{
				strVirLink.state = DIFFSTATE_REMOVED;
				strVirLink.srcSig = QString::fromStdString(baseSrcSig);
				strVirLink.destSig = QString::fromStdString(baseDestSig);
				isEdit = true;
			}
			strIED.vVirLink.push_back(strVirLink);
	}	

	for (int j = 0; j < otherInputs.size(); j++)
	{
		VirtualLinkStur strVirLink;
		string otherSrcSig = m_NameTranslator.GetKeyNameWithoutSubStation(otherInputs.at(j).srcSignal.toStdString());
		string otherDestSig = m_NameTranslator.GetKeyNameWithoutSubStation(otherInputs.at(j).destSignal.toStdString());
	
		strVirLink.state = DIFFSTATE_ADDED;
		strVirLink.srcSig = QString::fromStdString(otherSrcSig);
		strVirLink.destSig = QString::fromStdString(otherDestSig);
		strIED.vVirLink.push_back(strVirLink);
		isEdit = true;
	}
	return isEdit;
}

bool SCDDiffCompare::GetControlBlockDiff(QVector<QString> &baseCBKey, QVector<QString> &otherCBKey, IEDStru& strIED, DiffStates parentState)
{  
	SearchMap otherCBMap;
	for (int j = 0; j < otherCBKey.size(); j++)
	{
		QString otherKey =  NameTranslator_GetKeyNameWithoutSubStation(otherCBKey.at(j));
		otherCBMap.insert(make_pair(otherKey, otherCBKey.at(j)));
	}
	
	bool isEdit = false;
	for (int i = 0; i < baseCBKey.size(); i++)
	{
		bool isFound = false;
		QCtrlBlockInfo baseBlockInfo;
		SCLModel->GetCtrlBlockInfo(baseCBKey.at(i), baseBlockInfo);

		QString baseKey = NameTranslator_GetKeyNameWithoutSubStation(baseCBKey.at(i));	
		QVector<QString> baseSignalKeyList;
		SCLModel->GetDataSetSignalsByCtrlBlockID(baseCBKey.at(i), &baseSignalKeyList);

		SearchMap::iterator it = otherCBMap.find(baseKey);
		if (it != otherCBMap.end())
		{
			QVector<QString> otherSignalKeyList;
			SCLModel->GetDataSetSignalsByCtrlBlockID(it->second, &otherSignalKeyList);
			QCtrlBlockInfo otherBlockInfo;
			SCLModel->GetCtrlBlockInfo(it->second, otherBlockInfo);
			if (baseBlockInfo.type == otherBlockInfo.type)
			{
				if (baseBlockInfo.type == "GSEControl")
				{
					GOOSECBStru strGoose;
					if (SetValue(IED_GOOSE_APPID, strGoose.AppID, baseBlockInfo.APPID, otherBlockInfo.APPID, DIFFSTATE_NORMAL))
					{
						strGoose.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_GOOSE_VLANID, strGoose.VLANID, baseBlockInfo.apVLAN_ID, otherBlockInfo.apVLAN_ID, DIFFSTATE_NORMAL))
					{
						strGoose.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_GOOSE_VLANPRIORITY, strGoose.VLANPriority, baseBlockInfo.apVLAN_PRIORITY, otherBlockInfo.apVLAN_PRIORITY, DIFFSTATE_NORMAL))
					{
						strGoose.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_GOOSE_MACADDR, strGoose.MACAddr, baseBlockInfo.apMAC, otherBlockInfo.apMAC, DIFFSTATE_NORMAL))
					{
						strGoose.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_GOOSE_CONFVERSION, strGoose.ConfVersion, baseBlockInfo.confRev, otherBlockInfo.confRev, DIFFSTATE_NORMAL))
					{
						strGoose.state = DIFFSTATE_EDITED;
					}			
					if (SetValue(IED_GOOSE_MAXTIME, strGoose.MaxTime, baseBlockInfo.maxTime, otherBlockInfo.maxTime, DIFFSTATE_NORMAL))
					{
						strGoose.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_GOOSE_MINTIME, strGoose.MinTime, baseBlockInfo.minTime, otherBlockInfo.minTime, DIFFSTATE_NORMAL))
					{
						strGoose.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_GOOSE_CBNAME, strGoose.CBName, baseBlockInfo.name, otherBlockInfo.name, DIFFSTATE_NORMAL))
					{
						strGoose.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_GOOSE_DATASETNAME, strGoose.DataSetName, baseBlockInfo.dataSetName, otherBlockInfo.dataSetName, DIFFSTATE_NORMAL))
					{
						strGoose.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_GOOSE_DATASETCOUNT, strGoose.DataSetCount, baseBlockInfo.ASDU, otherBlockInfo.ASDU, DIFFSTATE_NORMAL))
					{
						strGoose.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_GOOSE_GOCBREF, strGoose.GocbRef, baseBlockInfo.gocbRef, otherBlockInfo.gocbRef, DIFFSTATE_NORMAL))
					{
						strGoose.state = DIFFSTATE_EDITED;
					}
					strGoose.type = baseBlockInfo.type;
					strGoose.ID = baseKey;

					VSignalStru vStrSignal;
					if (GetDataSetDiff(baseSignalKeyList, otherSignalKeyList, typeGoose_, vStrSignal, DIFFSTATE_NORMAL))
					{
						strGoose.state = DIFFSTATE_EDITED;
					}

					// ��ȡ����IED�б�
					QVector<QString> baseIEDList;
					SCLModel->GetAssoIEDListByCBKey(baseCBKey.at(i), &baseIEDList);
					QVector<QString> otherIEDList;
					SCLModel->GetAssoIEDListByCBKey(it->second, &otherIEDList);
					QVector<QString> vIEDList;
					GetAssoIEDListDiff(baseIEDList, otherIEDList, vIEDList, DIFFSTATE_NORMAL);
				
					if (strGoose.state == DIFFSTATE_EDITED)
					{
						isEdit = true;
					}
					strGoose.vSignal = vStrSignal;
					strGoose.vIEDList = vIEDList;
					strIED.vGOOSECB.push_back(strGoose);
				}
				else if (baseBlockInfo.type == "SampledValueControl")
				{
					SVCBStru strSV;
					if (SetValue(IED_SV_APPID, strSV.AppID, baseBlockInfo.APPID, otherBlockInfo.APPID, DIFFSTATE_NORMAL))
					{
						strSV.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_SV_VLANID, strSV.VLANID, baseBlockInfo.apVLAN_ID, otherBlockInfo.apVLAN_ID, DIFFSTATE_NORMAL))
					{
						strSV.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_SV_VLANPRIORITY, strSV.VLANPriority, baseBlockInfo.apVLAN_PRIORITY, otherBlockInfo.apVLAN_PRIORITY, DIFFSTATE_NORMAL))
					{
						strSV.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_SV_MACADDR, strSV.MACAddr, baseBlockInfo.apMAC, otherBlockInfo.apMAC, DIFFSTATE_NORMAL))
					{
						strSV.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_SV_CONFVERSION, strSV.ConfVersion, baseBlockInfo.confRev, otherBlockInfo.confRev, DIFFSTATE_NORMAL))
					{
						strSV.state = DIFFSTATE_EDITED;
					}			
					if (SetValue(IED_SV_SMPRATE, strSV.smpRate, baseBlockInfo.smpRate, otherBlockInfo.smpRate, DIFFSTATE_NORMAL))
					{
						strSV.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_SV_CBNAME, strSV.CBName, baseBlockInfo.name, otherBlockInfo.name, DIFFSTATE_NORMAL))
					{
						strSV.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_SV_DATASETNAME, strSV.DataSetName, baseBlockInfo.dataSetName, otherBlockInfo.dataSetName, DIFFSTATE_NORMAL))
					{
						strSV.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_SV_DATASETCOUNT, strSV.DataSetCount, baseBlockInfo.ASDU, otherBlockInfo.ASDU, DIFFSTATE_NORMAL))
					{
						strSV.state = DIFFSTATE_EDITED;
					}
					strSV.type = baseBlockInfo.type;
					strSV.ID = it->first;

					VSignalStru vStrSignal;
					if (GetDataSetDiff(baseSignalKeyList, otherSignalKeyList, typeSV_, vStrSignal, DIFFSTATE_NORMAL))
					{
						strSV.state = DIFFSTATE_EDITED;
					}

					// ��ȡBase����IED�б�
					QVector<QString> baseIEDList;
					SCLModel->GetAssoIEDListByCBKey(baseCBKey.at(i), &baseIEDList);
					QVector<QString> otherIEDList;
					SCLModel->GetAssoIEDListByCBKey(it->second, &otherIEDList);
					QVector<QString> vIEDList;
					GetAssoIEDListDiff(baseIEDList, otherIEDList, vIEDList, DIFFSTATE_NORMAL);
					
					if (strSV.state == DIFFSTATE_EDITED)
					{
						isEdit = true;
					}
					strSV.vSignal = vStrSignal;
					strSV.vIEDList = vIEDList;
					strIED.vSVCB.push_back(strSV);
				}
				otherCBMap.erase(it);
			}
		}
		else
		{
			if (baseBlockInfo.type == "GSEControl")
			{
				GOOSECBStru strGoose;

				if (bGooseCBSelected || strIED.state == DIFFSTATE_REMOVED)
				{
					strGoose.state = DIFFSTATE_REMOVED;
				}

				SetValue(IED_GOOSE_APPID, strGoose.AppID, baseBlockInfo.APPID, "", DIFFSTATE_REMOVED);
				SetValue(IED_GOOSE_VLANID, strGoose.VLANID, baseBlockInfo.apVLAN_ID, "", DIFFSTATE_REMOVED);
				SetValue(IED_GOOSE_VLANPRIORITY, strGoose.VLANPriority, baseBlockInfo.apVLAN_PRIORITY, "", DIFFSTATE_REMOVED);
				SetValue(IED_GOOSE_MACADDR, strGoose.MACAddr, baseBlockInfo.apMAC, "", DIFFSTATE_REMOVED);
				SetValue(IED_GOOSE_CONFVERSION, strGoose.ConfVersion, baseBlockInfo.confRev, "", DIFFSTATE_REMOVED);
				SetValue(IED_GOOSE_MAXTIME, strGoose.MaxTime, baseBlockInfo.maxTime, "", DIFFSTATE_REMOVED);
				SetValue(IED_GOOSE_MINTIME, strGoose.MinTime, baseBlockInfo.minTime, "", DIFFSTATE_REMOVED);
				SetValue(IED_GOOSE_CBNAME, strGoose.CBName, baseBlockInfo.name, "", DIFFSTATE_REMOVED);
				SetValue(IED_GOOSE_DATASETNAME, strGoose.DataSetName, baseBlockInfo.dataSetName, "", DIFFSTATE_REMOVED);
				SetValue(IED_GOOSE_GOCBREF, strGoose.GocbRef, baseBlockInfo.gocbRef, "", DIFFSTATE_REMOVED);
				SetValue(IED_GOOSE_DATASETCOUNT, strGoose.DataSetCount, baseBlockInfo.ASDU, 0, DIFFSTATE_REMOVED);

				strGoose.ID = baseKey;
				strGoose.type = baseBlockInfo.type;
			
				QVector<QString> otherSignalKeyList;
				VSignalStru vStrSignal;
				GetDataSetDiff(baseSignalKeyList, otherSignalKeyList, typeGoose_, vStrSignal, DIFFSTATE_REMOVED);

				// ��ȡBase����IED�б�
				QVector<QString> baseIEDList;
				QVector<QString> otherIEDList;
				SCLModel->GetAssoIEDListByCBKey(baseCBKey.at(i), &baseIEDList);	
				QVector<QString> vIEDList;				
				GetAssoIEDListDiff(baseIEDList, otherIEDList, vIEDList, DIFFSTATE_NORMAL);

				strGoose.vIEDList = vIEDList;
				strGoose.vSignal = vStrSignal;
				strIED.vGOOSECB.push_back(strGoose);

			}
			else if (baseBlockInfo.type == "SampledValueControl")
			{
				SVCBStru strSV;

				if (bSVCBSelected || strIED.state == DIFFSTATE_REMOVED)
				{
					strSV.state = DIFFSTATE_REMOVED;
				}

				SetValue(IED_SV_APPID, strSV.AppID, baseBlockInfo.APPID, "", DIFFSTATE_REMOVED);
				SetValue(IED_SV_VLANID, strSV.VLANID, baseBlockInfo.apVLAN_ID, "", DIFFSTATE_REMOVED);
				SetValue(IED_SV_VLANPRIORITY, strSV.VLANPriority, baseBlockInfo.apVLAN_PRIORITY, "", DIFFSTATE_REMOVED);
				SetValue(IED_SV_MACADDR, strSV.MACAddr, baseBlockInfo.apMAC, "", DIFFSTATE_REMOVED);
				SetValue(IED_SV_SMPRATE, strSV.smpRate, baseBlockInfo.smpRate, "", DIFFSTATE_REMOVED);
				SetValue(IED_SV_CONFVERSION, strSV.ConfVersion, baseBlockInfo.confRev, "", DIFFSTATE_REMOVED);
				SetValue(IED_SV_CBNAME, strSV.CBName, baseBlockInfo.name, "", DIFFSTATE_REMOVED);
				SetValue(IED_SV_DATASETNAME, strSV.DataSetName, baseBlockInfo.dataSetName, "", DIFFSTATE_REMOVED);
				SetValue(IED_SV_DATASETCOUNT, strSV.DataSetCount, baseBlockInfo.ASDU, 0, DIFFSTATE_REMOVED);
				strSV.type = baseBlockInfo.type;
				strSV.ID = baseKey;

				QVector<QString> otherSignalKeyList;
				VSignalStru vStrSignal;
				GetDataSetDiff(baseSignalKeyList, otherSignalKeyList, typeSV_, vStrSignal, DIFFSTATE_REMOVED);

				// ��ȡBase����IED�б�
				QVector<QString> baseIEDList;
				SCLModel->GetAssoIEDListByCBKey(baseCBKey.at(i), &baseIEDList);
				QVector<QString> otherIEDList;
				QVector<QString> vIEDList;
				GetAssoIEDListDiff(baseIEDList, otherIEDList, vIEDList, DIFFSTATE_NORMAL);

				strSV.vIEDList = vIEDList;
				strSV.vSignal = vStrSignal;
				strIED.vSVCB.push_back(strSV);
			}
		}
	}
	for (SearchMap::iterator it = otherCBMap.begin(); it != otherCBMap.end(); it++)
	{
		QVector<QString> otherSignalKeyList;
		SCLModel->GetDataSetSignalsByCtrlBlockID(it->second, &otherSignalKeyList);
		QCtrlBlockInfo otherBlockInfo;
		SCLModel->GetCtrlBlockInfo(it->second, otherBlockInfo);

		if (otherBlockInfo.type == "GSEControl")
		{
			GOOSECBStru strGoose;

			if (bGooseCBSelected || strIED.state == DIFFSTATE_ADDED)
			{
				strGoose.state = DIFFSTATE_ADDED;
			}

			SetValue(IED_GOOSE_APPID, strGoose.AppID, "", otherBlockInfo.APPID, DIFFSTATE_ADDED);
			SetValue(IED_GOOSE_VLANID, strGoose.VLANID, "", otherBlockInfo.apVLAN_ID, DIFFSTATE_ADDED);
			SetValue(IED_GOOSE_VLANPRIORITY, strGoose.VLANPriority, "", otherBlockInfo.apVLAN_PRIORITY, DIFFSTATE_ADDED);
			SetValue(IED_GOOSE_MACADDR, strGoose.MACAddr, "", otherBlockInfo.apMAC, DIFFSTATE_ADDED);
			SetValue(IED_GOOSE_CONFVERSION, strGoose.ConfVersion, "", otherBlockInfo.confRev, DIFFSTATE_ADDED);
			SetValue(IED_GOOSE_MAXTIME, strGoose.MaxTime, "", otherBlockInfo.maxTime, DIFFSTATE_ADDED);
			SetValue(IED_GOOSE_MINTIME, strGoose.MinTime, "", otherBlockInfo.minTime, DIFFSTATE_ADDED);
			SetValue(IED_GOOSE_CBNAME, strGoose.CBName, "", otherBlockInfo.name, DIFFSTATE_ADDED);
			SetValue(IED_GOOSE_DATASETNAME, strGoose.DataSetName, "", otherBlockInfo.dataSetName, DIFFSTATE_ADDED);
			SetValue(IED_GOOSE_DATASETCOUNT, strGoose.DataSetCount, 0, otherBlockInfo.ASDU, DIFFSTATE_ADDED);
			SetValue(IED_GOOSE_GOCBREF, strGoose.GocbRef, "", otherBlockInfo.gocbRef, DIFFSTATE_ADDED);
			strGoose.type = otherBlockInfo.type;
			strGoose.ID = it->first;
			
			QVector<QString> baseSignalKeyList;
			VSignalStru vStrSignal;
			GetDataSetDiff(baseSignalKeyList, otherSignalKeyList, typeGoose_, vStrSignal, DIFFSTATE_ADDED);


			// ��ȡBase����IED�б�
			QVector<QString> baseIEDList;
			// ��ȡOther����IED�б�
			QVector<QString> otherIEDList;
			SCLModel->GetAssoIEDListByCBKey(it->second, &otherIEDList);
			QVector<QString> vIEDList;
			GetAssoIEDListDiff(baseIEDList, otherIEDList, vIEDList, DIFFSTATE_NORMAL);

			strGoose.vIEDList = vIEDList;
			strGoose.vSignal = vStrSignal;
			strIED.vGOOSECB.push_back(strGoose);

		}
		else if (otherBlockInfo.type == "SampledValueControl")
		{
			SVCBStru strSV;

			if (bSVCBSelected || strIED.state == DIFFSTATE_ADDED )
			{
				strSV.state = DIFFSTATE_ADDED;
			}

			SetValue(IED_SV_APPID, strSV.AppID, "", otherBlockInfo.APPID, DIFFSTATE_ADDED);
			SetValue(IED_SV_VLANID, strSV.VLANID, "", otherBlockInfo.apVLAN_ID, DIFFSTATE_ADDED);
			SetValue(IED_SV_VLANPRIORITY, strSV.VLANPriority, "", otherBlockInfo.apVLAN_PRIORITY, DIFFSTATE_ADDED);
			SetValue(IED_SV_MACADDR, strSV.MACAddr, "", otherBlockInfo.apMAC, DIFFSTATE_ADDED);
			SetValue(IED_SV_CONFVERSION, strSV.ConfVersion, "", otherBlockInfo.confRev, DIFFSTATE_ADDED);
            SetValue(IED_SV_SMPRATE, strSV.smpRate, "", otherBlockInfo.smpRate, DIFFSTATE_ADDED);
			SetValue(IED_SV_CBNAME, strSV.CBName, "", otherBlockInfo.name, DIFFSTATE_ADDED);
			SetValue(IED_SV_DATASETNAME, strSV.DataSetName, "", otherBlockInfo.dataSetName, DIFFSTATE_ADDED);
			SetValue(IED_SV_DATASETCOUNT, strSV.DataSetCount, 0, otherBlockInfo.ASDU, DIFFSTATE_ADDED);
			strSV.type = otherBlockInfo.type;
			strSV.ID = it->first;

			QVector<QString> baseSignalKeyList;
			VSignalStru vStrSignal;
			GetDataSetDiff(baseSignalKeyList, otherSignalKeyList, typeSV_, vStrSignal, DIFFSTATE_ADDED);

			// ��ȡBase����IED�б�
			QVector<QString> baseIEDList;
			QVector<QString> otherIEDList;
			SCLModel->GetAssoIEDListByCBKey(it->second, &otherIEDList);
			QVector<QString> vIEDList;
			GetAssoIEDListDiff(baseIEDList, otherIEDList, vIEDList, DIFFSTATE_NORMAL);
			
			strSV.vIEDList = vIEDList;
			strSV.vSignal = vStrSignal;
			strIED.vSVCB.push_back(strSV);
		}
	}
	return isEdit;
}

bool SCDDiffCompare::GetAssoIEDListDiff(QVector<QString> &baseIEDList, QVector<QString> &otherIEDList, QVector<QString> &vIEDList, DiffStates parentState)
{
	SearchMap otherIEDListMap;
	for (int j = 0; j < otherIEDList.size(); j++)
	{
		QString otherIED = NameTranslator_GetKeyNameWithoutSubStation(otherIEDList.at(j));
		otherIEDListMap.insert(make_pair(otherIED, otherIEDList.at(j)));
	}

	bool isEdit = false;
	for (int i = 0; i < baseIEDList.size(); i++)
	{
		bool isFound = false; 
		QString baseIED = NameTranslator_GetKeyNameWithoutSubStation(baseIEDList.at(i));

		SearchMap::iterator it = otherIEDListMap.find(baseIED);
		if (it != otherIEDListMap.end())
		{
			vIEDList.push_back(baseIED);
			otherIEDListMap.erase(it);
		}
		else 
		{
			vIEDList.push_back(baseIED);
			isEdit = true;
		}
	}

	for (SearchMap::iterator it = otherIEDListMap.begin(); it != otherIEDListMap.end(); it++)
	{
		QString otherIED = NameTranslator_GetKeyNameWithoutSubStation(it->second);
		vIEDList.push_back(otherIED);
		isEdit = true;
	}
	return isEdit;
}

/** 
  * @brief    GetDataSetDiff
  * 
  * ��ȡ���ݼ��е�����ź�
  * 
  * @class    SCDDiffCompare
  * @param    QVector<QString> &baseSignalKeyList   : ��׼�ļ��е�����ź��б�
  * @param    QVector<QString> &otherSignalKeyList  : �Ƚ��ļ��е�����ź��б�
  * @param    QSignalTypeEnum signalType            : �ź�����
  * @param    VSignalStru &vStrSignal               : ����ź��б�
  * @param    DiffStates parentState                : ���ƿ�״̬
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */
bool SCDDiffCompare::GetDataSetDiff(QVector<QString> &baseSignalKeyList, QVector<QString> &otherSignalKeyList, QSignalTypeEnum signalType, VSignalStru &vStrSignal, DiffStates parentState)
{
	SearchMap otherSignalKeyListMap;
	for (int j = 0; j < otherSignalKeyList.size(); j++)
	{
		QString otherSignal = NameTranslator_GetKeyNameWithoutSubStation(otherSignalKeyList.at(j));
		otherSignalKeyListMap.insert(make_pair(otherSignal, otherSignalKeyList.at(j)));
	}


	bool isEdit = false;
	
	for (int i = 0; i < baseSignalKeyList.size(); i++)
	{
		SignalStru strDataSet;
		bool isFound = false;
		QSignalInfo baseSignalInfo;
		SCLModel->GetInfoOfOutSignal(baseSignalKeyList.at(i), &baseSignalInfo);
		QString baseSignal = NameTranslator_GetKeyNameWithoutSubStation(baseSignalKeyList.at(i));

		if (baseSignalInfo.type == signalType)
		{

			SearchMap::iterator it = otherSignalKeyListMap.find(baseSignal);
			if (it != otherSignalKeyListMap.end())
			{
				QSignalInfo otherSignlaInfo;
				SCLModel->GetInfoOfOutSignal(it->second, &otherSignlaInfo);
				if (otherSignlaInfo.type == signalType)
				{
					if (signalType == typeSV_)
					{
						if (SetValue(IED_SV_DATASET_ORD, strDataSet.Ord, baseSignalInfo.iOutSigOrd, otherSignlaInfo.iOutSigOrd, DIFFSTATE_NORMAL))
						{
							strDataSet.state = DIFFSTATE_EDITED;
						}
						if (SetValue(IED_SV_DATASET_DOIDU, strDataSet.DOIdU, baseSignalInfo.dUVal, otherSignlaInfo.dUVal, DIFFSTATE_NORMAL))
						{
							strDataSet.state = DIFFSTATE_EDITED;
						}
						if (SetValue(IED_SV_DATASET_DODESC, strDataSet.DODesc, baseSignalInfo.DOIDesc, otherSignlaInfo.DOIDesc, DIFFSTATE_NORMAL))
						{
							strDataSet.state = DIFFSTATE_EDITED;
						}
						if (SetValue(IED_SV_DATASET_LNDESC, strDataSet.LNDesc, baseSignalInfo.LnDesc, otherSignlaInfo.LnDesc, DIFFSTATE_NORMAL))
						{
							strDataSet.state = DIFFSTATE_EDITED;
						}
						if (SetValue(IED_SV_DATASET_FC, strDataSet.FC, baseSignalInfo.fc, otherSignlaInfo.fc, DIFFSTATE_NORMAL))
						{
							strDataSet.state = DIFFSTATE_EDITED;
						}
						if (SetValue(IED_SV_DATASET_BDATYPE, strDataSet.BDAType, baseSignalInfo.BDAType, otherSignlaInfo.BDAType, DIFFSTATE_NORMAL))
						{
							strDataSet.state = DIFFSTATE_EDITED;
						}
						if (SetValue(IED_SV_DATASET_SIGDESC, strDataSet.Desc, baseSignalInfo.desc, otherSignlaInfo.desc, DIFFSTATE_NORMAL))
						{
							strDataSet.state = DIFFSTATE_EDITED;
						}
					}
					else if (signalType == typeGoose_){
						if (SetValue(IED_GOOSE_DATASET_ORD, strDataSet.Ord, baseSignalInfo.iOutSigOrd, otherSignlaInfo.iOutSigOrd, DIFFSTATE_NORMAL))
						{
							strDataSet.state = DIFFSTATE_EDITED;
						}
						if (SetValue(IED_GOOSE_DATASET_DOIDU, strDataSet.DOIdU, baseSignalInfo.dUVal, otherSignlaInfo.dUVal, DIFFSTATE_NORMAL))
						{
							strDataSet.state = DIFFSTATE_EDITED;
						}
						if (SetValue(IED_GOOSE_DATASET_DODESC, strDataSet.DODesc, baseSignalInfo.DOIDesc, otherSignlaInfo.DOIDesc, DIFFSTATE_NORMAL))
						{
							strDataSet.state = DIFFSTATE_EDITED;
						}
						if (SetValue(IED_GOOSE_DATASET_LNDESC, strDataSet.LNDesc, baseSignalInfo.LnDesc, otherSignlaInfo.LnDesc, DIFFSTATE_NORMAL))
						{
							strDataSet.state = DIFFSTATE_EDITED;
						}
						if (SetValue(IED_GOOSE_DATASET_FC, strDataSet.FC, baseSignalInfo.fc, otherSignlaInfo.fc, DIFFSTATE_NORMAL))
						{
							strDataSet.state = DIFFSTATE_EDITED;
						}
						if (SetValue(IED_GOOSE_DATASET_BDATYPE, strDataSet.BDAType, baseSignalInfo.BDAType, otherSignlaInfo.BDAType, DIFFSTATE_NORMAL))
						{
							strDataSet.state = DIFFSTATE_EDITED;
						}
						if (SetValue(IED_GOOSE_DATASET_SIGDESC, strDataSet.Desc, baseSignalInfo.desc, otherSignlaInfo.desc, DIFFSTATE_NORMAL))
						{
							strDataSet.state = DIFFSTATE_EDITED;
						}
					}
					
					strDataSet.ctrlBlockID = NameTranslator_GetKeyNameWithoutSubStation(baseSignalInfo.ctrlBlockID);
					strDataSet.type = baseSignalInfo.type;
					strDataSet.ID = baseSignal;

					if (strDataSet.state == DIFFSTATE_EDITED)
					{
						isEdit = true;
					}
					otherSignalKeyListMap.erase(it);
				}	
			}
			else 
			{
				// ��base�ļ��ж��е��ź�
				strDataSet.type = baseSignalInfo.type;
				strDataSet.ID = baseSignal;

				if (signalType == typeSV_)
				{
					if (bSVDataSetSelected)
					{
						strDataSet.state = DIFFSTATE_REMOVED;
					}

					SetValue(IED_SV_DATASET_ORD, strDataSet.Ord, baseSignalInfo.iOutSigOrd, 0, DIFFSTATE_REMOVED);
					SetValue(IED_SV_DATASET_DOIDU, strDataSet.DOIdU, baseSignalInfo.dUVal, "", DIFFSTATE_REMOVED);
					SetValue(IED_SV_DATASET_DODESC, strDataSet.DODesc, baseSignalInfo.DOIDesc, "", DIFFSTATE_REMOVED);
					SetValue(IED_SV_DATASET_LNDESC, strDataSet.LNDesc, baseSignalInfo.LnDesc, "", DIFFSTATE_REMOVED);
					SetValue(IED_SV_DATASET_FC, strDataSet.FC, baseSignalInfo.fc, "", DIFFSTATE_REMOVED);
					SetValue(IED_SV_DATASET_BDATYPE, strDataSet.BDAType, baseSignalInfo.BDAType, "", DIFFSTATE_REMOVED);
					SetValue(IED_SV_DATASET_SIGDESC, strDataSet.Desc, baseSignalInfo.desc, "", DIFFSTATE_REMOVED);	
				}else if (signalType == typeGoose_)
				{
					if (bGooseDataSetSelected)
					{
						strDataSet.state = DIFFSTATE_REMOVED;
					}
					SetValue(IED_GOOSE_DATASET_ORD, strDataSet.Ord, baseSignalInfo.iOutSigOrd, 0, DIFFSTATE_REMOVED);
					SetValue(IED_GOOSE_DATASET_DOIDU, strDataSet.DOIdU, baseSignalInfo.dUVal, "", DIFFSTATE_REMOVED);
					SetValue(IED_GOOSE_DATASET_DODESC, strDataSet.DODesc, baseSignalInfo.DOIDesc, "", DIFFSTATE_REMOVED);
					SetValue(IED_GOOSE_DATASET_LNDESC, strDataSet.LNDesc, baseSignalInfo.LnDesc, "", DIFFSTATE_REMOVED);
					SetValue(IED_GOOSE_DATASET_FC, strDataSet.FC, baseSignalInfo.fc, "", DIFFSTATE_REMOVED);
					SetValue(IED_GOOSE_DATASET_BDATYPE, strDataSet.BDAType, baseSignalInfo.BDAType, "", DIFFSTATE_REMOVED);
					SetValue(IED_GOOSE_DATASET_SIGDESC, strDataSet.Desc, baseSignalInfo.desc, "", DIFFSTATE_REMOVED);	
				}
				
				strDataSet.ctrlBlockID = NameTranslator_GetKeyNameWithoutSubStation(baseSignalInfo.ctrlBlockID);
			}
			vStrSignal.push_back(strDataSet);
		}
	}

	// ��other�ļ����������ź�
	for (SearchMap::iterator it = otherSignalKeyListMap.begin(); it != otherSignalKeyListMap.end(); it++)
	{
		SignalStru strDataSet;
		QSignalInfo otherSignlaInfo;
		SCLModel->GetInfoOfOutSignal(it->second, &otherSignlaInfo);

		if (otherSignlaInfo.type == signalType)
		{
			if (signalType == typeSV_)
			{
				if (bSVDataSetSelected)
				{
					strDataSet.state = DIFFSTATE_ADDED;
				}
				SetValue(IED_SV_DATASET_ORD, strDataSet.Ord, 0, otherSignlaInfo.iOutSigOrd, DIFFSTATE_ADDED);
				SetValue(IED_SV_DATASET_DOIDU, strDataSet.DOIdU, "", otherSignlaInfo.dUVal, DIFFSTATE_ADDED);
				SetValue(IED_SV_DATASET_DODESC, strDataSet.DODesc, "", otherSignlaInfo.DOIDesc, DIFFSTATE_ADDED);
				SetValue(IED_SV_DATASET_LNDESC, strDataSet.LNDesc, "", otherSignlaInfo.LnDesc, DIFFSTATE_ADDED);
				SetValue(IED_SV_DATASET_FC, strDataSet.FC, "", otherSignlaInfo.fc, DIFFSTATE_ADDED);
				SetValue(IED_SV_DATASET_BDATYPE, strDataSet.BDAType, "", otherSignlaInfo.BDAType, DIFFSTATE_ADDED);
				SetValue(IED_SV_DATASET_SIGDESC, strDataSet.Desc, "", otherSignlaInfo.desc, DIFFSTATE_ADDED);
			}else if (signalType ==typeGoose_)
			{
				if (bGooseDataSetSelected)
				{
					strDataSet.state = DIFFSTATE_ADDED;
				}
				SetValue(IED_GOOSE_DATASET_ORD, strDataSet.Ord, 0, otherSignlaInfo.iOutSigOrd, DIFFSTATE_ADDED);
				SetValue(IED_GOOSE_DATASET_DOIDU, strDataSet.DOIdU, "", otherSignlaInfo.dUVal, DIFFSTATE_ADDED);
				SetValue(IED_GOOSE_DATASET_DODESC, strDataSet.DODesc, "", otherSignlaInfo.DOIDesc, DIFFSTATE_ADDED);
				SetValue(IED_GOOSE_DATASET_LNDESC, strDataSet.LNDesc, "", otherSignlaInfo.LnDesc, DIFFSTATE_ADDED);
				SetValue(IED_GOOSE_DATASET_FC, strDataSet.FC, "", otherSignlaInfo.fc, DIFFSTATE_ADDED);
				SetValue(IED_GOOSE_DATASET_BDATYPE, strDataSet.BDAType, "", otherSignlaInfo.BDAType, DIFFSTATE_ADDED);
				SetValue(IED_GOOSE_DATASET_SIGDESC, strDataSet.Desc, "", otherSignlaInfo.desc, DIFFSTATE_ADDED);
			}
			
			strDataSet.type = otherSignlaInfo.type;
			strDataSet.ID = it->first;
			strDataSet.ctrlBlockID = NameTranslator_GetKeyNameWithoutSubStation(otherSignlaInfo.ctrlBlockID);
			vStrSignal.push_back(strDataSet);
		}		
	}
	return isEdit;
}

/** 
  * @brief    GetInputsDiff
  * 
  * ȡ�����·�Ĳ��죺DestΪ��װ�õ����·
  * 
  * @class    SCDDiffCompare
  * @param    QVector<QVirtualLink> &baseInputs   : ��׼�ļ���������
  * @param    QVector<QVirtualLink> &otherInputs  : �Ƚ��ļ���������
  * @param    IEDStru& strIED                     : ���ڴ洢���
  * @param    DiffStates parentState              : IED��״̬
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/16
  * @remarks  
  */ 
bool SCDDiffCompare::GetInputsDiff(QVector<QVirtualLink> &baseInputs, QVector<QVirtualLink> &otherInputs, IEDStru& strIED, DiffStates parentState)
{
	InputStru strInput;
	bool isEdit = false;

	strInput.state = parentState;
	if (baseInputs.size() == 0 && otherInputs.size() == 0)
	{
		return false;
	}

	if (SetValue(IED_INPUTS_EXTREFCOUNT, strInput.ExtRefCount, baseInputs.size(), otherInputs.size(), parentState))
	{
		if (parentState == DIFFSTATE_NORMAL)
		{
			strInput.state = DIFFSTATE_EDITED;
			isEdit = true;
		}
	}

	for (int i = 0; i < baseInputs.size(); i++)
	{
		ExtRefStru strExtRef;
		bool isFound = false;

		QString baseSrcSig = NameTranslator_GetKeyNameWithoutSubStation(baseInputs.at(i).srcSignal);
		QString baseDestSig = NameTranslator_GetKeyNameWithoutSubStation(baseInputs.at(i).destSignal);
		QString baseVLink = baseSrcSig + baseDestSig;
		QSignalInfo baseDestSignalInfo, baseSrcSignalInfo;
		SCLModel->GetInfoOfInSignal(baseInputs.at(i).destSignal, &baseDestSignalInfo);
		SCLModel->GetInfoOfOutSignal(baseInputs.at(i).srcSignal, &baseSrcSignalInfo);

		if (NameTranslator_GetIEDKeyFromSCDDiffResult(baseDestSig) == strIED.ID)
		{

			for (int j = 0; j < otherInputs.size(); j++)
			{
				QString otherSrcSig = NameTranslator_GetKeyNameWithoutSubStation(otherInputs.at(j).srcSignal);
				QString otherDestSig = NameTranslator_GetKeyNameWithoutSubStation(otherInputs.at(j).destSignal);
				QString otherVLink = otherSrcSig + otherDestSig;
				QSignalInfo otherDestSignalInfo, otherSrcSignalInfo;
				SCLModel->GetInfoOfInSignal(otherInputs.at(j).destSignal, &otherDestSignalInfo);
				SCLModel->GetInfoOfOutSignal(otherInputs.at(j).srcSignal, &otherSrcSignalInfo);

				if (baseVLink == otherVLink)
				{
					strExtRef.ID = baseDestSig;
					strExtRef.type = baseDestSignalInfo.type;
					strExtRef.srcSignalID = baseSrcSig;
					strExtRef.destSignalID = baseDestSig;
					if (SetValue(IED_INPUTS_EXTREF_DESC, strExtRef.desc, baseDestSignalInfo.desc, otherDestSignalInfo.desc, DIFFSTATE_NORMAL))
					{
						strExtRef.state = DIFFSTATE_EDITED;
					}			
					if (SetValue(IED_INPUTS_EXTREF_INTADDR, strExtRef.IntAddr, baseDestSignalInfo.path, otherDestSignalInfo.path, DIFFSTATE_NORMAL))
					{
						strExtRef.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_INPUTS_EXTREF_OUTADDR, strExtRef.OutAddr, baseSrcSignalInfo.path, otherSrcSignalInfo.path, DIFFSTATE_NORMAL))
					{
						strExtRef.state = DIFFSTATE_EDITED;
					}
					QString baseDestPortString = m_NameTranslator.GetSignalPortQStringFromList(baseDestSignalInfo.portIndexList);
					QString OtherDestPortString = m_NameTranslator.GetSignalPortQStringFromList(otherDestSignalInfo.portIndexList);
					if (SetValue(IED_INPUTS_EXTREF_PORTNAME, strExtRef.PortName, baseDestPortString, OtherDestPortString, DIFFSTATE_NORMAL))
					{
						strExtRef.state = DIFFSTATE_EDITED;
					}
					

					if (SetValue(IED_INPUTS_EXTREF_PORTNAME, strExtRef.InSigInfo.PortIndex, baseDestPortString, OtherDestPortString, DIFFSTATE_NORMAL))
					{
						strExtRef.InSigInfo.state = DIFFSTATE_EDITED;
						strExtRef.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_INPUTS_DESTSIG_DOIDU, strExtRef.InSigInfo.DOIdU, baseDestSignalInfo.dUVal, otherDestSignalInfo.dUVal, DIFFSTATE_NORMAL))
					{
						strExtRef.InSigInfo.state = DIFFSTATE_EDITED;
						strExtRef.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_INPUTS_DESTSIG_DODESC, strExtRef.InSigInfo.DODesc, baseDestSignalInfo.DOIDesc, otherDestSignalInfo.DOIDesc, DIFFSTATE_NORMAL))
					{
						strExtRef.InSigInfo.state = DIFFSTATE_EDITED;
						strExtRef.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_INPUTS_DESTSIG_LNDESC, strExtRef.InSigInfo.LNDesc, baseDestSignalInfo.LnDesc, otherDestSignalInfo.LnDesc, DIFFSTATE_NORMAL))
					{
						strExtRef.InSigInfo.state = DIFFSTATE_EDITED;
						strExtRef.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_INPUTS_DESTSIG_FC, strExtRef.InSigInfo.FC, baseDestSignalInfo.fc, otherDestSignalInfo.fc, DIFFSTATE_NORMAL))
					{
						strExtRef.InSigInfo.state = DIFFSTATE_EDITED;
						strExtRef.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_INPUTS_DESTSIG_BDATYPE, strExtRef.InSigInfo.BDAType, baseDestSignalInfo.BDAType, otherDestSignalInfo.BDAType, DIFFSTATE_NORMAL))
					{
						strExtRef.InSigInfo.state = DIFFSTATE_EDITED;
						strExtRef.state = DIFFSTATE_EDITED;
					}
					if (SetValue(IED_INPUTS_DESTSIG_SIGDESC, strExtRef.InSigInfo.Desc, baseDestSignalInfo.desc, otherDestSignalInfo.desc, DIFFSTATE_NORMAL))
					{
						strExtRef.InSigInfo.state = DIFFSTATE_EDITED;
						strExtRef.state = DIFFSTATE_EDITED;
					}
					strExtRef.InSigInfo.type = baseDestSignalInfo.type;
					strExtRef.InSigInfo.ctrlBlockID = NameTranslator_GetKeyNameWithoutSubStation(baseDestSignalInfo.ctrlBlockID);
					strExtRef.InSigInfo.ID = baseDestSig;

					otherInputs.remove(j);
					if (strExtRef.state == DIFFSTATE_EDITED)
					{
						strInput.state = DIFFSTATE_EDITED;
						isEdit = true;
					}
					isFound = true;
					break;
				}
			}
			if (!isFound)
			{
				if (bInputsSelected)
				{
					strExtRef.state = DIFFSTATE_REMOVED;
				}
				
				strExtRef.ID = baseDestSig;
				strExtRef.type = baseDestSignalInfo.type;
				strExtRef.srcSignalID = baseSrcSig;
				strExtRef.destSignalID = baseDestSig;
				SetValue(IED_INPUTS_EXTREF_DESC, strExtRef.desc, baseDestSignalInfo.desc, "", DIFFSTATE_REMOVED);
				SetValue(IED_INPUTS_EXTREF_INTADDR, strExtRef.IntAddr, baseDestSignalInfo.path, "", DIFFSTATE_REMOVED);
				SetValue(IED_INPUTS_EXTREF_OUTADDR, strExtRef.OutAddr, baseSrcSignalInfo.path, "", DIFFSTATE_REMOVED);
				QString baseDestPortString = m_NameTranslator.GetSignalPortQStringFromList(baseDestSignalInfo.portIndexList);

				SetValue(IED_INPUTS_EXTREF_PORTNAME, strExtRef.PortName, baseDestPortString, "", DIFFSTATE_REMOVED);
				
                QVector<QString> votherInSignal;
                QString otherIEDKey = m_OtherSubStationKey + "." + strIED.ID;
                SCLModel->GetInSignalsListOfIED(otherIEDKey, typeAll_, &votherInSignal);
                bool iFoundInSignal = false;
                for (int ii = 0; ii < votherInSignal.size(); ii++)
                {
                    QString otherDestSig = NameTranslator_GetKeyNameWithoutSubStation(votherInSignal.at(ii));
                    if (baseDestSig == otherDestSig)
                    {
                        iFoundInSignal = true;
                        QSignalInfo otherDestSignalInfo;
                        SCLModel->GetInfoOfInSignal(votherInSignal.at(ii), &otherDestSignalInfo);
                        QString OtherDestPortString = m_NameTranslator.GetSignalPortQStringFromList(otherDestSignalInfo.portIndexList);
                        if (SetValue(IED_INPUTS_EXTREF_PORTNAME, strExtRef.InSigInfo.PortIndex, baseDestPortString, OtherDestPortString, DIFFSTATE_NORMAL))
                        {
                            strExtRef.InSigInfo.state = DIFFSTATE_EDITED;
                            strExtRef.state = DIFFSTATE_EDITED;
                        }
                        if (SetValue(IED_INPUTS_DESTSIG_DOIDU, strExtRef.InSigInfo.DOIdU, baseDestSignalInfo.dUVal, otherDestSignalInfo.dUVal, DIFFSTATE_NORMAL))
                        {
                            strExtRef.InSigInfo.state = DIFFSTATE_EDITED;
                            strExtRef.state = DIFFSTATE_EDITED;
                        }
                        if (SetValue(IED_INPUTS_DESTSIG_DODESC, strExtRef.InSigInfo.DODesc, baseDestSignalInfo.DOIDesc, otherDestSignalInfo.DOIDesc, DIFFSTATE_NORMAL))
                        {
                            strExtRef.InSigInfo.state = DIFFSTATE_EDITED;
                            strExtRef.state = DIFFSTATE_EDITED;
                        }
                        if (SetValue(IED_INPUTS_DESTSIG_LNDESC, strExtRef.InSigInfo.LNDesc, baseDestSignalInfo.LnDesc, otherDestSignalInfo.LnDesc, DIFFSTATE_NORMAL))
                        {
                            strExtRef.InSigInfo.state = DIFFSTATE_EDITED;
                            strExtRef.state = DIFFSTATE_EDITED;
                        }
                        if (SetValue(IED_INPUTS_DESTSIG_FC, strExtRef.InSigInfo.FC, baseDestSignalInfo.fc, otherDestSignalInfo.fc, DIFFSTATE_NORMAL))
                        {
                            strExtRef.InSigInfo.state = DIFFSTATE_EDITED;
                            strExtRef.state = DIFFSTATE_EDITED;
                        }
                        if (SetValue(IED_INPUTS_DESTSIG_BDATYPE, strExtRef.InSigInfo.BDAType, baseDestSignalInfo.BDAType, otherDestSignalInfo.BDAType, DIFFSTATE_NORMAL))
                        {
                            strExtRef.InSigInfo.state = DIFFSTATE_EDITED;
                            strExtRef.state = DIFFSTATE_EDITED;
                        }
                        if (SetValue(IED_INPUTS_DESTSIG_SIGDESC, strExtRef.InSigInfo.Desc, baseDestSignalInfo.desc, otherDestSignalInfo.desc, DIFFSTATE_NORMAL))
                        {
                            strExtRef.InSigInfo.state = DIFFSTATE_EDITED;
                            strExtRef.state = DIFFSTATE_EDITED;
                        }
                        strExtRef.InSigInfo.type = baseDestSignalInfo.type;
                        strExtRef.InSigInfo.ctrlBlockID = NameTranslator_GetKeyNameWithoutSubStation(baseDestSignalInfo.ctrlBlockID);
                        strExtRef.InSigInfo.ID = baseDestSig;
                        break;
                    }
                }
                if (!iFoundInSignal)
                {
                    SetValue(IED_INPUTS_EXTREF_PORTNAME, strExtRef.InSigInfo.PortIndex, baseDestPortString, "", DIFFSTATE_REMOVED);
                    SetValue(IED_INPUTS_DESTSIG_DOIDU, strExtRef.InSigInfo.DOIdU, baseDestSignalInfo.dUVal, "", DIFFSTATE_REMOVED);
                    SetValue(IED_INPUTS_DESTSIG_DODESC, strExtRef.InSigInfo.DODesc, baseDestSignalInfo.DOIDesc, "", DIFFSTATE_REMOVED);
                    SetValue(IED_INPUTS_DESTSIG_LNDESC, strExtRef.InSigInfo.LNDesc, baseDestSignalInfo.LnDesc, "", DIFFSTATE_REMOVED);
                    SetValue(IED_INPUTS_DESTSIG_FC, strExtRef.InSigInfo.FC, baseDestSignalInfo.fc, "", DIFFSTATE_REMOVED);
                    SetValue(IED_INPUTS_DESTSIG_BDATYPE, strExtRef.InSigInfo.BDAType, baseDestSignalInfo.BDAType, "", DIFFSTATE_REMOVED);
                    SetValue(IED_INPUTS_DESTSIG_SIGDESC, strExtRef.InSigInfo.Desc, baseDestSignalInfo.desc, "", DIFFSTATE_REMOVED);
                    strExtRef.InSigInfo.type = baseDestSignalInfo.type;
                    strExtRef.InSigInfo.ID = baseDestSig;
                    strExtRef.InSigInfo.ctrlBlockID = NameTranslator_GetKeyNameWithoutSubStation(baseDestSignalInfo.ctrlBlockID);
                    strExtRef.InSigInfo.state = DIFFSTATE_REMOVED;
                }

				strInput.state = DIFFSTATE_EDITED;
			}
			strInput.vExtRef.push_back(strExtRef);
		}	
	}

	for (int j = 0; j < otherInputs.size(); j++)
	{
		ExtRefStru strExtRef;
		QSignalInfo otherDestSignalInfo, otherSrcSignalInfo;
		SCLModel->GetInfoOfInSignal(otherInputs.at(j).destSignal, &otherDestSignalInfo);
		//chenxin modify 2014.10.31 --->
		SCLModel->GetInfoOfOutSignal(otherInputs.at(j).srcSignal, &otherSrcSignalInfo);
		//SCLModel->GetInfoOfInSignal(otherInputs.at(j).srcSignal, &otherSrcSignalInfo);
		//chenxin modify 2014.10.31 --->
		QString otherDestSig = NameTranslator_GetKeyNameWithoutSubStation(otherInputs.at(j).destSignal);
		QString otherSrcSig = NameTranslator_GetKeyNameWithoutSubStation(otherInputs.at(j).srcSignal);

		if (NameTranslator_GetIEDKeyFromSCDDiffResult(otherDestSig) == strIED.ID)
		{
			if (bInputsSelected)
			{
				strExtRef.state = DIFFSTATE_ADDED;
			}
			
			strExtRef.ID = otherDestSig;
			strExtRef.type = otherDestSignalInfo.type;
			strExtRef.srcSignalID = otherSrcSig;
			strExtRef.destSignalID = otherDestSig;
			SetValue(IED_INPUTS_EXTREF_DESC, strExtRef.desc, "", otherDestSignalInfo.desc, DIFFSTATE_ADDED);
			SetValue(IED_INPUTS_EXTREF_INTADDR, strExtRef.IntAddr, "", otherDestSignalInfo.path, DIFFSTATE_ADDED);
			SetValue(IED_INPUTS_EXTREF_OUTADDR, strExtRef.OutAddr, "", otherSrcSignalInfo.path, DIFFSTATE_ADDED);
			QString OtherDestPortString = m_NameTranslator.GetSignalPortQStringFromList(otherDestSignalInfo.portIndexList);

			SetValue(IED_INPUTS_EXTREF_PORTNAME, strExtRef.PortName, "", OtherDestPortString, DIFFSTATE_ADDED);
			
            QVector<QString> vbaseInSignal;
            QString baseIEDKey = m_BaseSubStationKey + "." + strIED.ID;
            SCLModel->GetInSignalsListOfIED(baseIEDKey, typeAll_, &vbaseInSignal);
            bool iFoundInSignal = false;
            for (int ii = 0; ii < vbaseInSignal.size(); ii++)
            {
                QString baseDestSig = NameTranslator_GetKeyNameWithoutSubStation(vbaseInSignal.at(ii));
                if (baseDestSig == otherDestSig)
                {
                    iFoundInSignal = true;
                    QSignalInfo baseDestSignalInfo;
                    SCLModel->GetInfoOfInSignal(vbaseInSignal.at(ii), &baseDestSignalInfo);
                    QString baseDestPortString = m_NameTranslator.GetSignalPortQStringFromList(baseDestSignalInfo.portIndexList);
                    if (SetValue(IED_INPUTS_EXTREF_PORTNAME, strExtRef.InSigInfo.PortIndex, baseDestPortString, OtherDestPortString, DIFFSTATE_NORMAL))
                    {
                        strExtRef.InSigInfo.state = DIFFSTATE_EDITED;
                        strExtRef.state = DIFFSTATE_EDITED;
                    }
                    if (SetValue(IED_INPUTS_DESTSIG_DOIDU, strExtRef.InSigInfo.DOIdU, baseDestSignalInfo.dUVal, otherDestSignalInfo.dUVal, DIFFSTATE_NORMAL))
                    {
                        strExtRef.InSigInfo.state = DIFFSTATE_EDITED;
                        strExtRef.state = DIFFSTATE_EDITED;
                    }
                    if (SetValue(IED_INPUTS_DESTSIG_DODESC, strExtRef.InSigInfo.DODesc, baseDestSignalInfo.DOIDesc, otherDestSignalInfo.DOIDesc, DIFFSTATE_NORMAL))
                    {
                        strExtRef.InSigInfo.state = DIFFSTATE_EDITED;
                        strExtRef.state = DIFFSTATE_EDITED;
                    }
                    if (SetValue(IED_INPUTS_DESTSIG_LNDESC, strExtRef.InSigInfo.LNDesc, baseDestSignalInfo.LnDesc, otherDestSignalInfo.LnDesc, DIFFSTATE_NORMAL))
                    {
                        strExtRef.InSigInfo.state = DIFFSTATE_EDITED;
                        strExtRef.state = DIFFSTATE_EDITED;
                    }
                    if (SetValue(IED_INPUTS_DESTSIG_FC, strExtRef.InSigInfo.FC, baseDestSignalInfo.fc, otherDestSignalInfo.fc, DIFFSTATE_NORMAL))
                    {
                        strExtRef.InSigInfo.state = DIFFSTATE_EDITED;
                        strExtRef.state = DIFFSTATE_EDITED;
                    }
                    if (SetValue(IED_INPUTS_DESTSIG_BDATYPE, strExtRef.InSigInfo.BDAType, baseDestSignalInfo.BDAType, otherDestSignalInfo.BDAType, DIFFSTATE_NORMAL))
                    {
                        strExtRef.InSigInfo.state = DIFFSTATE_EDITED;
                        strExtRef.state = DIFFSTATE_EDITED;
                    }
                    if (SetValue(IED_INPUTS_DESTSIG_SIGDESC, strExtRef.InSigInfo.Desc, baseDestSignalInfo.desc, otherDestSignalInfo.desc, DIFFSTATE_NORMAL))
                    {
                        strExtRef.InSigInfo.state = DIFFSTATE_EDITED;
                        strExtRef.state = DIFFSTATE_EDITED;
                    }
                    strExtRef.InSigInfo.type = baseDestSignalInfo.type;
                    strExtRef.InSigInfo.ctrlBlockID = NameTranslator_GetKeyNameWithoutSubStation(baseDestSignalInfo.ctrlBlockID);
                    strExtRef.InSigInfo.ID = baseDestSig;
                    break;
                }
            }
            if (!iFoundInSignal)
            {
                SetValue(IED_INPUTS_EXTREF_PORTNAME, strExtRef.InSigInfo.PortIndex, "", OtherDestPortString, DIFFSTATE_ADDED);
                SetValue(IED_INPUTS_DESTSIG_DOIDU, strExtRef.InSigInfo.DOIdU, "", otherDestSignalInfo.dUVal, DIFFSTATE_ADDED);
                SetValue(IED_INPUTS_DESTSIG_DODESC, strExtRef.InSigInfo.DODesc, "", otherDestSignalInfo.DOIDesc, DIFFSTATE_ADDED);
                SetValue(IED_INPUTS_DESTSIG_LNDESC, strExtRef.InSigInfo.LNDesc, "", otherDestSignalInfo.LnDesc, DIFFSTATE_ADDED);
                SetValue(IED_INPUTS_DESTSIG_FC, strExtRef.InSigInfo.FC, "", otherDestSignalInfo.fc, DIFFSTATE_ADDED);
                SetValue(IED_INPUTS_DESTSIG_BDATYPE, strExtRef.InSigInfo.BDAType, "", otherDestSignalInfo.BDAType, DIFFSTATE_ADDED);
                SetValue(IED_INPUTS_DESTSIG_SIGDESC, strExtRef.InSigInfo.Desc, "", otherDestSignalInfo.desc, DIFFSTATE_ADDED);
                strExtRef.InSigInfo.type = otherDestSignalInfo.type;
                strExtRef.InSigInfo.ID = otherDestSig;
                strExtRef.InSigInfo.ctrlBlockID = NameTranslator_GetKeyNameWithoutSubStation(otherDestSignalInfo.ctrlBlockID);
                strExtRef.InSigInfo.state = DIFFSTATE_ADDED;
            }

			strInput.vExtRef.push_back(strExtRef);
			strInput.state = DIFFSTATE_EDITED;
		}
	}
	strIED.vInput.push_back(strInput);
	if (strInput.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	return false;
}

bool SCDDiffCompare::GetPhyPortDiff(QVector<QString> &basePhyPortList, QVector<QString> &otherPhyPortList, IEDStru& strIED, DiffStates parentState)
{
	bool isEdit = false;
	for (int i = 0; i < basePhyPortList.size(); i++)
	{
		PhysicsPortStru strPhyPort;
		QPhyPortInfo basePhyPortInfo, otherPhyPortInfo;
		bool isFound = false;
		SCLModel->GetPhyPortInfo(basePhyPortList.at(i), basePhyPortInfo);
		string  baseKey = m_NameTranslator.GetKeyNameWithoutSubStation(basePhyPortList.at(i).toStdString());
		for (int j = 0; j < otherPhyPortList.size(); j++)
		{
			SCLModel->GetPhyPortInfo(otherPhyPortList.at(j), otherPhyPortInfo);
			string  otherKey = m_NameTranslator.GetKeyNameWithoutSubStation(otherPhyPortList.at(j).toStdString());
			if (baseKey == otherKey)
			{
				strPhyPort.ID = QString::fromStdString(baseKey);

				if (SetValue(IED_PHY_PORTINDEX ,strPhyPort.pIndex, basePhyPortInfo.pIndex, otherPhyPortInfo.pIndex, DIFFSTATE_NORMAL))
				{
					strPhyPort.state = DIFFSTATE_EDITED;
				}
				if (SetValue(IED_PHY_PLUG ,strPhyPort.plug, basePhyPortInfo.plug, otherPhyPortInfo.plug, DIFFSTATE_NORMAL))
				{
					strPhyPort.state = DIFFSTATE_EDITED;
				}
				if (SetValue(IED_PHY_TYPE ,strPhyPort.type, basePhyPortInfo.type, otherPhyPortInfo.type, DIFFSTATE_NORMAL))
				{
					strPhyPort.state = DIFFSTATE_EDITED;
				}
				if (SetValue(IED_PHY_TRANSMEDIA ,strPhyPort.transMedia, basePhyPortInfo.transMedia, otherPhyPortInfo.transMedia, DIFFSTATE_NORMAL))
				{
					strPhyPort.state = DIFFSTATE_EDITED;
				}
				if (SetValue(IED_PHY_CABLE ,strPhyPort.cable, basePhyPortInfo.cable, otherPhyPortInfo.cable, DIFFSTATE_NORMAL))
				{
					strPhyPort.state = DIFFSTATE_EDITED;
				}
				otherPhyPortList.remove(j);
				if (strPhyPort.state == DIFFSTATE_EDITED)
				{
					isEdit =true;
				}
				isFound = true;
				break;
			}

		}
		if (!isFound)
		{
			if (bPhyPortSelected || strIED.state == DIFFSTATE_REMOVED)
			{
				strPhyPort.state = DIFFSTATE_REMOVED;
			}
						
			strPhyPort.ID = QString::fromStdString(baseKey);
			SetValue(IED_PHY_TRANSMEDIA ,strPhyPort.transMedia, basePhyPortInfo.transMedia, "", DIFFSTATE_REMOVED);
			SetValue(IED_PHY_PORTINDEX ,strPhyPort.pIndex, basePhyPortInfo.pIndex, "", DIFFSTATE_REMOVED);
			SetValue(IED_PHY_PLUG ,strPhyPort.plug, basePhyPortInfo.plug, "", DIFFSTATE_REMOVED);
			SetValue(IED_PHY_CABLE ,strPhyPort.cable, basePhyPortInfo.cable, "", DIFFSTATE_REMOVED);
			SetValue(IED_PHY_TYPE ,strPhyPort.type, basePhyPortInfo.type, "", DIFFSTATE_REMOVED);
		}
		strIED.vPortStru.push_back(strPhyPort);
	}
	for (int j = 0; j < otherPhyPortList.size(); j++)
	{
		PhysicsPortStru strPhyPort;
		QPhyPortInfo basePhyPortInfo, otherPhyPortInfo;
		if (bPhyPortSelected || strIED.state == DIFFSTATE_ADDED)
		{
			strPhyPort.state = DIFFSTATE_ADDED;
		}

		SCLModel->GetPhyPortInfo(otherPhyPortList.at(j), otherPhyPortInfo);
		string  otherKey = m_NameTranslator.GetKeyNameWithoutSubStation(otherPhyPortList.at(j).toStdString());

		strPhyPort.ID = QString::fromStdString(otherKey);
		SetValue(IED_PHY_TRANSMEDIA ,strPhyPort.transMedia, "", otherPhyPortInfo.transMedia, DIFFSTATE_ADDED);
		SetValue(IED_PHY_PORTINDEX ,strPhyPort.pIndex, "", otherPhyPortInfo.pIndex, DIFFSTATE_ADDED);
		SetValue(IED_PHY_PLUG ,strPhyPort.plug, "", otherPhyPortInfo.plug, DIFFSTATE_ADDED);
		SetValue(IED_PHY_CABLE ,strPhyPort.cable, "", otherPhyPortInfo.cable, DIFFSTATE_ADDED);
		SetValue(IED_PHY_TYPE ,strPhyPort.type, "", otherPhyPortInfo.type, DIFFSTATE_ADDED);
		strIED.vPortStru.push_back(strPhyPort);
	}
	return isEdit;
}

bool SCDDiffCompare::SetValue(ConfInfo conf,  BasicStru &basicStru, QString baseAtt, QString otherAtt, DiffStates parentState)
{
	bool isEdit = false;
	basicStru.AttributeDesc = m_ConfPair[conf].confChDesc;

	if (ConfigureInfo[conf])
	{
		if (parentState == DIFFSTATE_NORMAL)
		{
			if (baseAtt != otherAtt)
			{
				basicStru.state = DIFFSTATE_EDITED;
				isEdit = true;
			}
			else 
			{
				basicStru.state = DIFFSTATE_NORMAL;
			}
		} 
		else 
		{
			if (baseAtt != otherAtt)
			{
				basicStru.state = parentState;
			}
			else 
			{
				basicStru.state = DIFFSTATE_NORMAL;
			}			
		}

		basicStru.baseAttributeName = baseAtt;
		basicStru.otherAttributeName = otherAtt;
	}
	else 
	{
		if (parentState == DIFFSTATE_NORMAL)
		{
			if (baseAtt.isEmpty())
			{
				basicStru.baseAttributeName = otherAtt;
				basicStru.otherAttributeName = otherAtt;
			}else 
			{
				basicStru.baseAttributeName = baseAtt;
				basicStru.otherAttributeName = baseAtt;
			}
			basicStru.state = DIFFSTATE_NORMAL;
		} 
		else 
		{
			if (baseAtt != otherAtt)
			{
				basicStru.state = parentState;
			}
			else 
			{
				basicStru.state = DIFFSTATE_NORMAL;
			}
			basicStru.baseAttributeName = baseAtt;
			basicStru.otherAttributeName = otherAtt;
		}
	}
	return isEdit;	
}

bool SCDDiffCompare::SetValue(ConfInfo conf,  BasicIntStruct &basicStru, int baseAtt, int otherAtt, DiffStates parentState)
{
	bool isEdit = false;
	basicStru.AttributeDesc = m_ConfPair[conf].confChDesc;
	if (ConfigureInfo[conf])
	{
		if (parentState == DIFFSTATE_NORMAL)
		{
			if (baseAtt != otherAtt)
			{
				basicStru.state = DIFFSTATE_EDITED;
				isEdit = true;
			}
			else 
			{
				basicStru.state = DIFFSTATE_NORMAL;
			}
		} 
		else 
		{
			if (baseAtt != otherAtt)
			{
				basicStru.state = parentState;
			}
			else 
			{
				basicStru.state = DIFFSTATE_NORMAL;
			}	
		}
		basicStru.baseAttributeName = baseAtt;
		basicStru.otherAttributeName = otherAtt;
	}
	else
	{
		if (parentState == DIFFSTATE_NORMAL)
		{
			if (baseAtt == 0)
			{
				basicStru.baseAttributeName = otherAtt;
				basicStru.otherAttributeName = otherAtt;
			}else 
			{
				basicStru.baseAttributeName = baseAtt;
				basicStru.otherAttributeName = baseAtt;
			}
			basicStru.state = DIFFSTATE_NORMAL;
		}
		else
		{
			if (baseAtt != otherAtt)
			{
				basicStru.state = parentState;
			}
			else 
			{
				basicStru.state = DIFFSTATE_NORMAL;
			}
			basicStru.baseAttributeName = baseAtt;
			basicStru.otherAttributeName = otherAtt;
		}
	}
	return isEdit;
}


/** 
  * @brief    isIEDSelect
  * 
  * �ж�IED�е��������Ƿ�ѡ��
  * 
  * @class    SCDDiffCompare
  * @return   bool
  * @author   zhang-xch
  * @date     2014/5/8
  * @remarks  
  */ 
bool SCDDiffCompare::IsIEDSelect()
{
	for (int i = IED_DESC; i < ITEMCOUNT; i++)
	{
		if (ConfigureInfo[i])
		{
			return true;
		}
	}
	return false;
}

bool SCDDiffCompare::IsSVCBSelected()
{
	for (int i = IED_SV_APPID; i < IED_GOOSE_APPID; i++)
	{
		if (ConfigureInfo[i])
		{
			return true;
		}
	}
	return false;
}

bool SCDDiffCompare::IsSVDataSetSelected()
{
	for (int i = IED_SV_DATASET_ORD; i < IED_GOOSE_APPID; i++)
	{
		if (ConfigureInfo[i])
		{
			return true;
		}
	}
	return false;
}

bool SCDDiffCompare::IsGOOSECBSelected()
{
	for (int i = IED_GOOSE_APPID; i < IED_INPUTS_EXTREFCOUNT; i++)
	{
		if (ConfigureInfo[i])
		{
			return true;
		}
	}
	return false;
}

bool SCDDiffCompare::IsGOOSEDataSetSelected()
{
	for (int i = IED_GOOSE_DATASET_ORD; i < IED_INPUTS_EXTREFCOUNT; i++)
	{
		if (ConfigureInfo[i])
		{
			return true;
		}
	}
	return false;
}

bool SCDDiffCompare::IsInputsSelected()
{
	for (int i = IED_INPUTS_EXTREFCOUNT; i < IED_PHY_TRANSMEDIA; i++)
	{
		if (ConfigureInfo[i])
		{
			return true;
		}
	}
	return false;
}

bool SCDDiffCompare::IsPhyPortSelected()
{
	for (int i = IED_PHY_TRANSMEDIA; i < ITEMCOUNT; i++)
	{
		if (ConfigureInfo[i])
		{
			return true;
		}
	}
	return false;
}

/** 
  * @brief    NameTranslator_GetKeyNameWithoutSubStation
  * 
  * ͨ��NameTranslator����ʵʱ���е�Keyת��Ϊ���컯�Ƚ����õ�Key
  * 
  * @class    SCDDiffCompare
  * @param    QString qString : ʵʱ���е�Key
  * @return   QString
  * @author   zhang-xch
  * @date     2014/5/15
  * @remarks  
  */ 
QString SCDDiffCompare::NameTranslator_GetKeyNameWithoutSubStation(QString qString)
{
	return QString::fromStdString(m_NameTranslator.GetKeyNameWithoutSubStation(qString.toStdString()));
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
QString SCDDiffCompare::NameTranslator_GetIEDKeyFromSCDDiffResult(QString qString)
{
	return QString::fromStdString(m_NameTranslator.GetIEDKeyFromSCDDiffResult(qString.toStdString()));
}


/** 
  * @brief    SetConfigureInfo
  * 
  * ����������Ϣ
  * 
  * @class    SCDDiffCompare
  * @return   void
  * @author   zhang-xch
  * @date     2014/5/18
  * @remarks  
  */ 
void SCDDiffCompare::SetConfigureInfo()
{
	SetConfigureInfoPair(FILEINFO_VERSION, "FILEINFO_VERSION", "�汾��Ϣ");
	SetConfigureInfoPair(FILEINFO_REVERSION, "FILEINFO_REVERSION", "�޶��汾��Ϣ");
	SetConfigureInfoPair(FILEINFO_CRC, "FILEINFO_CRC", "�ļ�CRC��Ϣ");

	SetConfigureInfoPair(IED_DESC, "IED_DESC", "IED����");
	SetConfigureInfoPair(IED_TYPE, "IED_TYPE", "IED����");
	SetConfigureInfoPair(IED_MANU, "IED_MANU", "IED����");
	SetConfigureInfoPair(IED_CRC, "IED_CRC", "IED CRC��Ϣ");

	SetConfigureInfoPair(IED_SV_APPID, "IED_SV_APPID", "AppID");
	SetConfigureInfoPair(IED_SV_VLANID, "IED_SV_VLANID", "VLanID");
	SetConfigureInfoPair(IED_SV_VLANPRIORITY, "IED_SV_VLANPRIORITY", "VLan���ȼ�");
	SetConfigureInfoPair(IED_SV_MACADDR, "IED_SV_MACADDR", "Mac��ַ");
	SetConfigureInfoPair(IED_SV_CONFVERSION, "IED_SV_CONFVERSION", "���ð汾��Ϣ");
	//SetConfigureInfoPair(IED_SV_ASDUCOUNT, "IED_SV_ASDUCOUNT", "ASDU��Ŀ");
	SetConfigureInfoPair(IED_SV_SMPRATE, "IED_SV_SMPRATE", "������");
	SetConfigureInfoPair(IED_SV_CBNAME, "IED_SV_CBNAME", "���ƿ�����");
	// SetConfigureInfoPair(IED_SV_CBDESC, "IED_SV_CBDESC", "���ƿ�����");
	SetConfigureInfoPair(IED_SV_DATASETNAME, "IED_SV_DATASETNAME", "���ݼ�����");
	SetConfigureInfoPair(IED_SV_DATASETDESC, "IED_SV_DATASETDESC", "���ݼ�����");
	SetConfigureInfoPair(IED_SV_DATASETCOUNT, "IED_SV_DATASETCOUNT", "��Ϣ����");
	SetConfigureInfoPair(IED_SV_DATASETADDR, "IED_SV_DATASETADDR", "���ݼ���ַ");
	SetConfigureInfoPair(IED_SV_CBAPAPPID, "IED_SV_CBAPAPPID", "���ƿ�Ap��ַ");
	SetConfigureInfoPair(IED_SV_DATASET_ORD, "IED_SV_DATASET_ORD", "���");
	SetConfigureInfoPair(IED_SV_DATASET_DOIDU, "IED_SV_DATASET_DOIDU", "dUֵ");
	SetConfigureInfoPair(IED_SV_DATASET_DODESC, "IED_SV_DATASET_DODESC", "DO����");
	SetConfigureInfoPair(IED_SV_DATASET_LNDESC, "IED_SV_DATASET_LNDESC", "LN����");
	SetConfigureInfoPair(IED_SV_DATASET_FC, "IED_SV_DATASET_FC", "����Լ��");
	SetConfigureInfoPair(IED_SV_DATASET_BDATYPE, "IED_SV_DATASET_BDATYPE", "BDA����");
	SetConfigureInfoPair(IED_SV_DATASET_TRIPNAME, "IED_SV_DATASET_TRIPNAME", "ѹ������");
	SetConfigureInfoPair(IED_SV_DATASET_SIGDESC, "IED_SV_DATASET_SIGDESC", "�ź�����");
	SetConfigureInfoPair(IED_SV_DATASET_PORTINDEX, "IED_SV_DATASET_PORTINDEX", "����˿�����");
	// SetConfigureInfoPair(IED_SV_VLINK, "IED_SV_VLINK", "������");
	// GOOSE
	SetConfigureInfoPair(IED_GOOSE_APPID, "IED_GOOSE_APPID", "AppID");
	SetConfigureInfoPair(IED_GOOSE_VLANID, "IED_GOOSE_VLANID", "VLanID");
	SetConfigureInfoPair(IED_GOOSE_VLANPRIORITY, "IED_GOOSE_VLANPRIORITY", "VLan���ȼ�");
	SetConfigureInfoPair(IED_GOOSE_MACADDR, "IED_GOOSE_MACADDR", "Mac��ַ");
	SetConfigureInfoPair(IED_GOOSE_CONFVERSION, "IED_GOOSE_CONFVERSION", "���ð汾��Ϣ");
	SetConfigureInfoPair(IED_GOOSE_MAXTIME, "IED_GOOSE_MAXTIME", "MaxTime");
	SetConfigureInfoPair(IED_GOOSE_MINTIME, "IED_GOOSE_MINTIME", "MinTime");
	SetConfigureInfoPair(IED_GOOSE_CBNAME, "IED_GOOSE_CBNAME", "���ƿ�����");
	// SetConfigureInfoPair(IED_GOOSE_CBDESC, "IED_GOOSE_CBDESC", "���ƿ�����");
	SetConfigureInfoPair(IED_GOOSE_DATASETNAME, "IED_GOOSE_DATASETNAME", "���ݼ�����");
	SetConfigureInfoPair(IED_GOOSE_DATASETDESC, "IED_GOOSE_DATASETDESC", "���ݼ�����");
	SetConfigureInfoPair(IED_GOOSE_DATASETCOUNT, "IED_GOOSE_DATASETCOUNT", "��Ϣ����");
	SetConfigureInfoPair(IED_GOOSE_GOCBREF, "IED_GOOSE_GOCBREF", "goCBRef");
	SetConfigureInfoPair(IED_GOOSE_DATASETADDR, "IED_GOOSE_DATASETADDR", "���ݼ���ַ");
	SetConfigureInfoPair(IED_GOOSE_CBAPAPPID, "IED_GOOSE_CBAPAPPID", "���ƿ�Ap��ַ");
	SetConfigureInfoPair(IED_GOOSE_DATASET_ORD, "IED_GOOSE_DATASET_ORD", "���");
	SetConfigureInfoPair(IED_GOOSE_DATASET_DOIDU, "IED_GOOSE_DATASET_DOIDU", "dUֵ");
	SetConfigureInfoPair(IED_GOOSE_DATASET_DODESC, "IED_GOOSE_DATASET_DODESC", "DO����");
	SetConfigureInfoPair(IED_GOOSE_DATASET_LNDESC, "IED_GOOSE_DATASET_LNDESC", "LN����");
	SetConfigureInfoPair(IED_GOOSE_DATASET_FC, "IED_GOOSE_DATASET_FC", "����Լ��");
	SetConfigureInfoPair(IED_GOOSE_DATASET_BDATYPE, "IED_GOOSE_DATASET_BDATYPE", "BDA����");
	SetConfigureInfoPair(IED_GOOSE_DATASET_TRIPNAME, "IED_GOOSE_DATASET_TRIPNAME", "ѹ������");
	SetConfigureInfoPair(IED_GOOSE_DATASET_SIGDESC, "IED_GOOSE_DATASET_SIGDESC", "�ź�����");
	SetConfigureInfoPair(IED_GOOSE_DATASET_PORTINDEX, "IED_GOOSE_DATASET_PORTINDEX", "����˿�����");
	// SetConfigureInfoPair(IED_GOOSE_VLINK, "IED_GOOSE_VLINK", "������");

	SetConfigureInfoPair(IED_INPUTS_EXTREFCOUNT, "IED_INPUTS_EXTREFCOUNT", "��������Ŀ");
	//SetConfigureInfoPair(IED_INPUTS_EXTREF_ORD, "IED_INPUTS_EXTREF_ORD", "���");
	SetConfigureInfoPair(IED_INPUTS_EXTREF_DESC, "IED_INPUTS_EXTREF_DESC", "����");
	//	SetConfigureInfoPair(IED_INPUTS_EXTREF_TYPE, "IED_INPUTS_EXTREF_TYPE", "����");
	SetConfigureInfoPair(IED_INPUTS_EXTREF_INTADDR, "IED_INPUTS_EXTREF_INTADDR", "�ڲ���ַ");
	SetConfigureInfoPair(IED_INPUTS_EXTREF_OUTADDR, "IED_INPUTS_EXTREF_OUTADDR", "�ⲿ��ַ");
	SetConfigureInfoPair(IED_INPUTS_EXTREF_PORTNAME, "IED_INPUTS_EXTREF_PORTNAME", "����˿�����");
	SetConfigureInfoPair(IED_INPUTS_DESTSIG_DOIDU, "IED_INPUTS_DESTSIG_DOIDU", "dUֵ");
	SetConfigureInfoPair(IED_INPUTS_DESTSIG_DODESC, "IED_INPUTS_DESTSIG_DODESC", "DO����");
	SetConfigureInfoPair(IED_INPUTS_DESTSIG_LNDESC, "IED_INPUTS_DESTSIG_LNDESC", "LN����");
	SetConfigureInfoPair(IED_INPUTS_DESTSIG_FC, "IED_INPUTS_DESTSIG_FC", "����Լ��");
	SetConfigureInfoPair(IED_INPUTS_DESTSIG_BDATYPE, "IED_INPUTS_DESTSIG_BDATYPE", "BDA����");
	SetConfigureInfoPair(IED_INPUTS_DESTSIG_TRIPNAME, "IED_INPUTS_DESTSIG_TRIPNAME", "ѹ������");
	SetConfigureInfoPair(IED_INPUTS_DESTSIG_SIGDESC, "IED_INPUTS_DESTSIG_SIGDESC", "�ź�����");
	SetConfigureInfoPair(IED_INPUTS_DESTSIG_PORTINDEX, "IED_INPUTS_DESTSIG_PORTINDEX", "����˿�����");

	SetConfigureInfoPair(IED_PHY_TRANSMEDIA, "IED_PHY_TRANSMEDIA", "����˿�����");
	SetConfigureInfoPair(IED_PHY_PORTINDEX, "IED_PHY_PINDEX", "����˿����");
	SetConfigureInfoPair(IED_PHY_PLUG, "IED_PHY_PLUG", "����˿ڲ�ͷ����");
	SetConfigureInfoPair(IED_PHY_TYPE, "IED_PHY_TYPE", "����˿ڽӿ�����");
	SetConfigureInfoPair(IED_PHY_CABLE, "IED_PHY_CABLE", "�����������");
}

/** 
  * @brief    SetConfigureInfoPair
  * 
  * ����������Ϣ������
  * 
  * @class    SCDDiffCompare
  * @param    ConfInfo ord    : ���
  * @param    QString EnDesc  : Ӣ������
  * @param    QString ChDesc  : ��������
  * @return   void
  * @author   zhang-xch
  * @date     2014/5/18
  * @remarks  
  */ 
void SCDDiffCompare::SetConfigureInfoPair(ConfInfo ord, QString EnDesc, QString ChDesc)
{
	m_ConfPair[ord].confEnDesc = EnDesc;
	m_ConfPair[ord].confChDesc = ChDesc;
}
