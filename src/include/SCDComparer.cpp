#include "SCDComparer.h"
#include "SCLModelSvrAdapter.h"
#include "SCLNameTranslator.h"

SCDComparer::SCDComparer()
{
	SCLModel = SCLModelAdapter::GetInstance();
}

SCDComparer::~SCDComparer()
{

}

void SCDComparer::setSubstationKeys(const QString baseKey, const QString otherKey)
{
	m_BaseSubKey = baseKey + ".屏柜";
	m_OtherSubKey = otherKey + ".屏柜";
}

void SCDComparer::LoadDiffVector()
{
	QVector<QString> baseIEDKeys;
	QVector<QString> otherIEDKeys;
	QVector<QString> baseIEDKeysInclude;
	QVector<QString> otherIEDKeysInclude;
	
	SCLModel->GetIEDList(m_BaseSubKey, &baseIEDKeys);
	SCLModel->GetIEDList(m_OtherSubKey, &otherIEDKeys);
	
	iedDiffVec.clear();

	for (int i = 0; i != baseIEDKeys.size(); i++)
	{
		QString baseIEDName = SCLModel->GetNameFromKey(baseIEDKeys.at(i));
		
		QIEDInfo baseIEDInfo;
		SCLModel->GetIEDInfo(baseIEDKeys.at(i),baseIEDInfo);
		
		for (int j = 0; j != otherIEDKeys.size(); j++)
		{
			QString otherIEDName = SCLModel->GetNameFromKey(otherIEDKeys.at(j));
			if (baseIEDName.compare(otherIEDName) == 0)
			{
				QIEDInfo otherIEDInfo;
				SCLModel->GetIEDInfo(otherIEDKeys.at(j),otherIEDInfo);
				
				bool isUpdated = false;

				QVector<QString> baseRealiteIEDs, otherRealiteIEDs;
				SCLModel->GetIEDListByLogicalRealitionWithCurrentIED(baseIEDKeys.at(i), &baseRealiteIEDs);
				SCLModel->GetIEDListByLogicalRealitionWithCurrentIED(otherIEDKeys.at(j), &otherRealiteIEDs);

				if (baseRealiteIEDs.size() != otherRealiteIEDs.size())
				{
					isUpdated = true;
					IEDDiffStru diffStru;
					diffStru.IEDKey = baseIEDKeys.at(i);
					diffStru.otherIEDKey = otherIEDKeys.at(j);
					diffStru.IEDName = baseIEDName;
					diffStru.IEDDesc = baseIEDInfo.desc;
					diffStru.diffType = IED_update;
					iedDiffVec.append(diffStru);
					baseIEDKeysInclude.append(baseIEDKeys.at(i));
					otherIEDKeysInclude.append(otherIEDKeys.at(j));
					break;	//break	  for(otherIEDKeys)
				}
				for (int k = 0; k != baseRealiteIEDs.size(); k++)
				{
					bool isIncluded = false;
					bool isVLinkUpdate = true;
					QString baseRealiteIEDKey = baseRealiteIEDs.at(k);
					QString baseRealiteIEDName = SCLModel->GetNameFromKey(baseRealiteIEDKey);
					for (int l = 0; l != otherRealiteIEDs.size(); l++)
					{
						QCoreApplication::processEvents(QEventLoop::AllEvents,1);

						QString otherRealiteIEDKey = otherRealiteIEDs.at(l);
						QString otherRealiteIEDName = SCLModel->GetNameFromKey(otherRealiteIEDKey);
						if (baseRealiteIEDName == otherRealiteIEDName)
						{
							isIncluded = true;
							QVector<QVirtualLink> baseIEDVLinks, otherIEDVLinks;
							SCLModel->GetVirtualSignalsAssociatedIED(baseIEDKeys.at(i), baseRealiteIEDKey, typeAll_, &baseIEDVLinks);
							SCLModel->GetVirtualSignalsAssociatedIED(otherIEDKeys.at(j), otherRealiteIEDKey, typeAll_, &otherIEDVLinks);
							if (baseIEDVLinks.size() == otherIEDVLinks.size())
							{
								int vl_equal_num = 0;
								for (int vi = 0; vi != baseIEDVLinks.size(); vi++)
								{
									QVirtualLink vl_base = baseIEDVLinks.at(vi);
									for (int vj = 0; vj != otherIEDVLinks.size(); vj++)
									{
										QCoreApplication::processEvents(QEventLoop::AllEvents,1);
										QVirtualLink vl_other = otherIEDVLinks.at(vj);
										QString base_src_name = SCLModel->GetNameFromKey(vl_base.srcSignal);
										QString base_dest_name = SCLModel->GetNameFromKey(vl_base.destSignal);
										QString other_src_name = SCLModel->GetNameFromKey(vl_other.srcSignal);
										QString other_dest_name = SCLModel->GetNameFromKey(vl_other.destSignal);
										if (base_src_name == other_src_name && base_dest_name == other_dest_name)
										{
											vl_equal_num++;
											break;
										}
									}
								}
								if (vl_equal_num == baseIEDVLinks.size())
								{
									isVLinkUpdate = false;
									break;	//break	  for(otherIEDVLinks)
								}
							}
							//break;	//break   for(otherIEDVLinks)
						}
					}	//for (otherIEDVLinks)
					if (isVLinkUpdate)
					{
						isUpdated = true;
						IEDDiffStru diffStru;
						diffStru.IEDKey = baseIEDKeys.at(i);
						diffStru.otherIEDKey = otherIEDKeys.at(j);
						diffStru.IEDName = baseIEDName;
						diffStru.IEDDesc = baseIEDInfo.desc;
						diffStru.diffType = IED_update;
						iedDiffVec.append(diffStru);
						break;	//break  for(baseIEDVLinks)
					}
					if (!isIncluded)
					{
						isUpdated = true;
						IEDDiffStru diffStru;
						diffStru.IEDKey = baseIEDKeys.at(i);
						diffStru.otherIEDKey = otherIEDKeys.at(j);
						diffStru.IEDName = baseIEDName;
						diffStru.IEDDesc = baseIEDInfo.desc;
						diffStru.diffType = IED_update;
						iedDiffVec.append(diffStru);
						break;	//break  for(baseIEDVLinks)
					}
				} //for baseIEDVLinks
				
				if (!isUpdated)
				{
					IEDDiffStru diffStru;
					diffStru.IEDKey = baseIEDKeys.at(i);
					diffStru.otherIEDKey = otherIEDKeys.at(j);
					diffStru.IEDName = baseIEDName;
					diffStru.IEDDesc = baseIEDInfo.desc;
					diffStru.diffType = IED_Equal;
					iedDiffVec.append(diffStru);
					//break;	//break   for(otherIEDKeys)
				}
				baseIEDKeysInclude.append(baseIEDKeys.at(i));
				otherIEDKeysInclude.append(otherIEDKeys.at(j));
				break;	//break	  for(otherIEDKeys)
			}	//if(baseIEDName.compare(otherIEDName) == 0)
		}	//for(otherIEDKeys)
	}	//for(baseIEDKeys)
	//比较结束;

	//遍历减少的IED
	if (baseIEDKeys.size() != baseIEDKeysInclude.size())
	{
		for (int i = 0; i != baseIEDKeys.size(); i++)
		{
			bool isIncluded = false;
			QString iedKey1 = baseIEDKeys.at(i);
			for (int j = 0; j != baseIEDKeysInclude.size(); j++)
			{
				QString iedKey2 = baseIEDKeysInclude.at(j);
				if (iedKey1 == iedKey2)
				{
					isIncluded = true;
					break;
				}
			}
			if (!isIncluded)
			{
				QIEDInfo iedInfo;
				SCLModel->GetIEDInfo(iedKey1, iedInfo);
				IEDDiffStru diffStru;
				diffStru.IEDKey = iedKey1;
				diffStru.otherIEDKey = "";
				diffStru.IEDName = iedInfo.name;
				diffStru.IEDDesc = iedInfo.desc;
				diffStru.diffType = IED_delete;
				iedDiffVec.append(diffStru);
			}
		}
	}

	//遍历增加的IED
	if (otherIEDKeys.size() != otherIEDKeysInclude.size())
	{
		for (int i = 0; i != otherIEDKeys.size(); i++)
		{
			bool isIncluded = false;
			QString iedKey1 = otherIEDKeys.at(i);
			for (int j = 0; j != otherIEDKeysInclude.size(); j++)
			{
				QString iedKey2 = otherIEDKeysInclude.at(j);
				if(iedKey1 == iedKey2)
				{
					isIncluded = true;
					break;
				}
			}
			if (!isIncluded)
			{
				QIEDInfo iedInfo;
				SCLModel->GetIEDInfo(iedKey1, iedInfo);
				IEDDiffStru diffStru;
				diffStru.IEDKey = "";
				diffStru.otherIEDKey = iedKey1;
				diffStru.IEDName = iedInfo.name;
				diffStru.IEDDesc = iedInfo.desc;
				diffStru.diffType = IED_add;
				iedDiffVec.append(diffStru);
			}
		}
	}
}

QVector<IEDDiffStru> SCDComparer::GetDiffVector()
{
	return iedDiffVec;
}

QString SCDComparer::GetBaseSubKey()
{
	return m_BaseSubKey;
}

QString SCDComparer::GetOtherSubKey()
{
	return m_OtherSubKey;
}

QVector<QString> SCDComparer::GetAddedOtherIEDs(QVector<QString> *baseOtherIEDs, QVector<QString> *otherOtherIEDs)
{
	QVector<QString> addedOtherIEDs;

	for (int i = 0; i != otherOtherIEDs->size(); i++)
	{
		QString otherIEDKey = otherOtherIEDs->at(i);
 		QString otherIEDName = SCLModel->GetNameFromKey(otherIEDKey);
		bool isIncluded = false;
		for (int j = 0; j != baseOtherIEDs->size(); j++)
		{
			QString baseIEDKey = baseOtherIEDs->at(j);
			QString baseIEDName = SCLModel->GetNameFromKey(baseIEDKey);
			if (otherIEDName == baseIEDName)
			{
				isIncluded = true;
				break;
			}
		}
		if (!isIncluded)
		{
			addedOtherIEDs.append(otherIEDKey);
		}
	}
	return addedOtherIEDs;
}

bool SCDComparer::isVLinksModified(const QVector<QVirtualLink> &baseVLinks, const QVector<QVirtualLink> &otherVLinks)
{
	int equalNum = 0;
	for (int gIndex = 0; gIndex != baseVLinks.size(); gIndex++)
	{
		QVirtualLink baseIEDVLink = baseVLinks.at(gIndex);
		for (int gOIndex = 0; gOIndex != otherVLinks.size(); gOIndex++)
		{
			QVirtualLink otherIEDVLink = otherVLinks.at(gOIndex);
			QString baseSrc = SCLModel->GetNameFromKey(baseIEDVLink.srcSignal);
			QString baseDest = SCLModel->GetNameFromKey(baseIEDVLink.destSignal);
			QString otherSrc = SCLModel->GetNameFromKey(otherIEDVLink.srcSignal);
			QString otherDest = SCLModel->GetNameFromKey(otherIEDVLink.destSignal);
			if (baseSrc == otherSrc && baseDest == otherDest)
			{
				equalNum++;
				break;
			}
		}
	}
	if (equalNum != baseVLinks.size())
	{
		return true;
	}
	return false;
}

QList<int> SCDComparer::GetDiffVTList(const QList<VirtualTerminal> &baseMasterList, const QList<VirtualTerminal> &baseSlaveList,
									const QList<VirtualTerminal> &otherMasterList, const QList<VirtualTerminal> &otherSlaveList,
									QList<VirtualTerminal> *allVTMasterList, QList<VirtualTerminal> *allVTSlaveLis)
{
	QList<VirtualTerminal> bothIncludeMasterList_Base;
	QList<VirtualTerminal> bothIncludeSlaveList_Base;
	QList<VirtualTerminal> bothIncludeMasterList_Other;
	QList<VirtualTerminal> bothIncludeSlaveList_Other;
	QList<int> diffTypeList;
	for (int i = 0; i != baseMasterList.size(); i++)
	{
		VirtualTerminal baseMasterVT = baseMasterList.at(i);
		VirtualTerminal baseSlaveVT  = baseSlaveList.at(i);
		for (int j = 0; j != otherMasterList.size(); j++)
		{
			VirtualTerminal otherMasterVT = otherMasterList.at(j);
			VirtualTerminal otherSlaveVT  = otherSlaveList.at(j);
			if (baseMasterVT.strName == otherMasterVT.strName)
			{
				if (baseSlaveVT.strName == otherSlaveVT.strName)
				{
					bothIncludeMasterList_Base.append(baseMasterVT);
					bothIncludeSlaveList_Base.append(baseSlaveVT);
					bothIncludeMasterList_Other.append(otherMasterVT);
					bothIncludeSlaveList_Other.append(otherSlaveVT);
					allVTMasterList->append(baseMasterVT);
					allVTSlaveLis->append(baseSlaveVT);
					diffTypeList.append(0);
				}
			}
		}
	}

	for (int baseI = 0; baseI != baseMasterList.size(); baseI++)
	{
		bool isIncluded = false;
		VirtualTerminal vt1 = baseMasterList.at(baseI);
		VirtualTerminal vtS = baseSlaveList.at(baseI);
		for (int baseJ = 0; baseJ != bothIncludeMasterList_Base.size(); baseJ++)
		{
			VirtualTerminal vt2m = bothIncludeMasterList_Base.at(baseJ);
			VirtualTerminal vt2s = bothIncludeSlaveList_Base.at(baseJ);
			if (vt1.strName == vt2m.strName && vtS.strName == vt2s.strName)
			{
				isIncluded = true;
				break;
			}
		}
		if (!isIncluded)
		{
			allVTMasterList->append(vt1);
			allVTSlaveLis->append(vtS);
			diffTypeList.append(2);
		}
	}

	for (int otherI = 0; otherI != otherMasterList.size(); otherI++)
	{
		bool isIncluded = false;
		VirtualTerminal vt1 = otherMasterList.at(otherI);
		VirtualTerminal vtS = otherSlaveList.at(otherI);
		for (int otherJ = 0; otherJ != bothIncludeMasterList_Other.size(); otherJ++)
		{
			VirtualTerminal vt2 = bothIncludeMasterList_Other.at(otherJ);
			VirtualTerminal vt2s = bothIncludeSlaveList_Other.at(otherJ);
			if (vt1.strName == vt2.strName && vtS.strName == vt2s.strName)
			{
				isIncluded = true;
				break;
			}
		}
		if (!isIncluded)
		{
			allVTMasterList->append(vt1);
			allVTSlaveLis->append(vtS);
			diffTypeList.append(3);
		}
	}
	return diffTypeList;
}

SCDComparer *SCDComparer::m_pInstance = NULL;