#include "GraphicLoader.h"
#include "SCLModelSvrAdapter.h"
#include "SCLNameTranslator.h"
#include "SCLModelAdapterForClient.h"//chenxin add 2014.10.17
#include "QMessageBox.h"//chenxin add 2014.10.24
#include <QFile>
#include <QApplication>

/** 
  * @brief    LoadVoltageLevelNameList
  * 
  * ���ص�ѹ�ȼ��б�
  * 
  * @class    IEDLoader
  * @return   void
  * @author   zhang-xch
  * @date     2014/11/10
  * @remarks  
  */ 
void IEDLoader::LoadVoltageLevelNameList()
{
	if(SCLModel!=NULL)
	{
		voltageLevelNameLists.clear();
		SCLModel->GetVoltageLevelList(&voltageLevelNameLists);
	}
}

/** 
  * @brief    LoadSubstationNameList
  * 
  * ���ص�ѹ�ȼ��±��վ�б�
  * 
  * @class    IEDLoader
  * @return   void
  * @author   zhang-xch
  * @date     2014/11/10
  * @remarks  
  */ 
void IEDLoader::LoadSubstationNameList()
{
	if(SCLModel!=NULL)
	{
		subStationNameLists.clear();
		QString tempSubStationImportTime;
		QMap<QString, QString> SubstationInfoMap;
		if(voltageLevelNameLists.size()>0)
		{
			SCLModel->GetSubstationList(voltageLevelNameLists[voltageLevelIndex],&subStationNameLists);
			for(int i=0;i<subStationNameLists.size();i++)
			{
				tempSubStationImportTime = SCLModel->GetSubstationImportTime(subStationNameLists[i]);
				SubstationInfoMap.insert(tempSubStationImportTime, subStationNameLists[i]);
			}
			int j = 0;
			for (QMap<QString, QString>::iterator it = SubstationInfoMap.begin(); it != SubstationInfoMap.end(); it++)
			{
				subStationNameLists[j] = SCLModel->GetNameFromKey(it.value());
				j++;
			}			
		}
	}
}

/** 
  * @brief    GetSubstationInfo
  * 
  * ��ȡ���վ��Ϣ
  * 
  * @class    *IEDLoader
  * @param    QString substationName :
  * @return   QSubStationInfo
  * @author   zhang-xch
  * @date     2014/11/10
  * @remarks  
  */ 
QSubStationInfo *IEDLoader::GetSubstationInfo(QString substationName)
{
	SCLModel->GetSubstationInfo(substationName,substationInfo);
	return &substationInfo;
}

/** 
  * @brief    GetSubstationImportTime
  * 
  * ��ȡ���վ����ʱ��
  * 
  * @class    IEDLoader
  * @param    QString substationKey :
  * @return   QString
  * @author   zhang-xch
  * @date     2014/11/10
  * @remarks  
  */ 
QString IEDLoader::GetSubstationImportTime(QString substationKey)
{
	QString imptTime = SCLModel->GetSubstationImportTime(substationKey);
	if (imptTime.compare("") == 0)
	{
		return "";
	}
	QList<QString> strList = imptTime.split(".");
	return strList.at(0);
}

/** 
  * @brief    GetVoltageLevelNameList
  * 
  * ��ȡ��ѹ�ȼ��б�
  * 
  * @class    *IEDLoader
  * @return   QVector<QString>
  * @author   zhang-xch
  * @date     2014/11/10
  * @remarks  
  */ 
QVector<QString> *IEDLoader::GetVoltageLevelNameList()
{
	return &voltageLevelNameLists;
}

/** 
  * @brief    GetSubstationNameList
  * 
  * ��ȡ���վ�����б�
  * 
  * @class    *IEDLoader
  * @return   QVector<QString>
  * @author   zhang-xch
  * @date     2014/11/10
  * @remarks  
  */ 
QVector<QString> *IEDLoader::GetSubstationNameList()
{
	return &subStationNameLists;
}

/** 
  * @brief    GetCurrentVoltageLevelIndex
  * 
  * ��ȡ��ǰ��ѹ�ȼ�����
  * 
  * @class    IEDLoader
  * @return   int
  * @author   zhang-xch
  * @date     2014/11/10
  * @remarks  
  */ 
int IEDLoader::GetCurrentVoltageLevelIndex()
{
	return voltageLevelIndex;
}

/** 
  * @brief    SetCurrentVoltageLevelIndex
  * 
  * ���õ�ǰ��ѹ�ȼ�����
  * 
  * @class    IEDLoader
  * @param    int index :
  * @return   void
  * @author   zhang-xch
  * @date     2014/11/10
  * @remarks  
  */ 
void IEDLoader::SetCurrentVoltageLevelIndex(int index)
{
	voltageLevelIndex = index;
}

void IEDLoader::UnloadScdFile(QString SubstationID)
{
	int oldSubstationIndex = subStationNameLists.indexOf(SubstationID,0);
	if(oldSubstationIndex>=0)
	{
		if(SclDacLib::UnloadScdFile(SubstationID.toLocal8Bit().constData()))
		{
			// QMessageBox::warning(NULL,"ж��ʧ��","ж���ڴ�ʧ��!");
		}
	}
	LoadSubstationNameList();
}


/** 
  * @brief    SetCurrentSubstationID
  * 
  * ���õ�ǰ���վ
  * 
  * @class    IEDLoader
  * @param    QString substationID :
  * @return   void
  * @author   chenxin 
  * @date     2014.10.26
  * @remarks  
  */ 
void IEDLoader::SetCurrentSubstationID(QString substationID)
{
	UnloadScdFile(curSubstationID);
	int newSubstationIndex = subStationNameLists.indexOf(substationID,0);
	if(newSubstationIndex>=0)
	{
		if(SclDacLib::LoadScdFile(substationID.toLocal8Bit().constData()))
		{
            StartSSDVaildate(substationID);
			 // QMessageBox::warning(NULL,subStationNameLists[substationIndex],"װ���ڴ�ʧ��!");
		}
		curSubstationID = substationID;
		substationIndex = newSubstationIndex;
	}
	else
	{
		if(subStationNameLists.size()>0)
		{
			if(m_bShowStationOnStart)
			{
				// ��List��δ�ҵ�ʱ����������Ϊ0���������ڴ�
				substationIndex = 0;
				curSubstationID = subStationNameLists.at(substationIndex);
				if(SclDacLib::LoadScdFile(curSubstationID.toLocal8Bit().constData()))
				{
                    StartSSDVaildate(substationID);
					//QMessageBox::warning(NULL,subStationNameLists[substationIndex],"װ���ڴ�ʧ��!");
				}
			}
			else
			{
				curSubstationID = "";
			}
						
		}
		else
		{
			curSubstationID = "";
		}
	}
}


void IEDLoader::StartSSDVaildate(QString substationID)
{
    QString appPath = QApplication::applicationDirPath();
    QString filePath = appPath + "/../data/" + substationID + "/stored.rfscd";
    if (!QFile::exists(filePath))
    {
        QMessageBox::warning(NULL, "SCL���", "��Ӧ��SCD�ļ������ڣ�");
        return;
    }

    // ��ȡ�����ļ�
    QString configFilePath = appPath + "/../cfg/private/SSD���.xml";
    if (!QFile::exists(configFilePath))
    {
        QMessageBox::warning(NULL, "SCL���", "��Ӧ�������ļ������ڣ�");
        return;
    }

    // ���exe�ļ�
    QString scdCheckExe = appPath + "/SCLCheck/ScdChecker.exe"; 

    
    m_SCLValid->SetFilePath(filePath, scdCheckExe, configFilePath);
    checkThread->start();
}
/** 
  * @brief    GetCurrentSubstationID
  * 
  * ��ȡ��ǰ���վ����
  * 
  * @class    IEDLoader
  * @return   QString
  * @author   zhang-xch
  * @date     2014/11/10
  * @remarks  
  */ 
QString IEDLoader::GetCurrentSubstationID()
{
	return curSubstationID;
}

QVector<QHItemInfo> *IEDLoader::GetHItemList()
{
	return &HItems;
}


IEDItem IEDLoader::GetIEDItemFromIEDName(QString name)
{
	IEDItem rtnItem;
	for(int i=0;i<IEDs.size();i++)
	{
		if(IEDs[i].IEDName==name)
		{
			rtnItem = IEDs[i];
			break;
		}
	}
    return rtnItem;
}

SubNetItem IEDLoader::GetSubNetItemFromSubNetName(QString name)
{
	SubNetItem tempItem;
	for(int i=0;i<subNets.size();i++)
	{
		tempItem = subNets[i];
		if(name.compare(tempItem.subNetName) == 0)
		{
			return tempItem;
		}
	}
	return tempItem;
}

void IEDLoader::LoadSubNetList()
{
	if(SCLModel!=NULL)
	{
		subNets.clear();
		if(voltageLevelNameLists.size()>0&&subStationNameLists.size()>0)
		{
			QString subStationKey = SCLModel->GetChildKeyFromParentKeyAndChildName(voltageLevelNameLists[voltageLevelIndex],subStationNameLists[substationIndex]);
			QVector<QString> subNetList;
			SCLModel->GetSubNetList(subStationKey,subNetList);

			SCLNameTranslator translator;

			for(int i=0;i<subNetList.size();i++)
			{
				SubNetItem tempSubNetItem;
				tempSubNetItem.subNetKey  = subNetList[i];
				tempSubNetItem.subNetName = translator.GetNameFromKey(subNetList[i].toLocal8Bit().constData()).c_str();
				subNets.append(tempSubNetItem);
			}
		}
	}
}

void IEDLoader::LoadHItemList()
{
	if(SCLModel!=NULL)
	{
		HItems.clear();
		if(voltageLevelNameLists.size()>0&&subStationNameLists.size()>0)
		{
			QString subStationKey = SCLModel->GetChildKeyFromParentKeyAndChildName(voltageLevelNameLists[voltageLevelIndex],subStationNameLists[substationIndex]);
			SCLModel->GetSCDHItem(subStationKey,HItems);
		}
	}
}

void IEDLoader::LoadIEDList()
{
	if(SCLModel!=NULL)
	{
		IEDs.clear();
		if(voltageLevelNameLists.size()>0&&subStationNameLists.size()>0)
		{
			QString subStationKey = SCLModel->GetChildKeyFromParentKeyAndChildName(voltageLevelNameLists[voltageLevelIndex],subStationNameLists[substationIndex]);

			QVector<QString> panelKeyList;			
			if(SCLModel->GetPanelList(subStationKey,&panelKeyList))
			{
				for(int i=0;i<panelKeyList.size();i++)
				{
					QVector<QString> iedKeyList;
					if(SCLModel->GetIEDList(panelKeyList[i],&iedKeyList))
					{
						SCLNameTranslator translator;
						for(int j=0;j<iedKeyList.size();j++)
						{
							IEDItem  iedItem;
							iedItem.IEDKey   = iedKeyList[j];
							iedItem.IEDName  = translator.GetNameFromKey(iedKeyList[j].toLocal8Bit().constData()).c_str();
							IEDs.append(iedItem);
						}
					}
				}
			}
		}
	}
	//����;
	QList<IEDItem> iedList = IEDs.toList();
	int count  = iedList.size();
	int index = 0;
	for (int i = 0; i < count - 1; i++)
	{
		index = i;
		QString sName = iedList.at(i).IEDName;
		for (int j = i + 1; j < count; j++)
		{
			QString dName = iedList.at(j).IEDName;
			if (strncmp(sName.toLocal8Bit().constData(),dName.toLocal8Bit().constData(),256)>0)
			{
				index = j;
				sName = dName;
			}
		}
		if (index != i)
		{
			iedList.swap(i,index);
		}
	}
	IEDs.clear();
	IEDs = iedList.toVector();
	//�������;
}

void IEDLoader::LoadIEDInfoList()
{
	IEDInfos.clear();
	for (int i = 0; i < IEDs.size(); i++)
	{
		QIEDInfo iedInfo;
		SCLModel->GetIEDInfo(IEDs.at(i).IEDKey, iedInfo);
		IEDInfos.append(iedInfo);
	}
}

QVector<IEDItem> *IEDLoader::GetIEDList()
{
	return &IEDs;
}

QVector<SubNetItem> *IEDLoader::GetSubNetList()
{
	return &subNets;
}

QVector<QIEDInfo> *IEDLoader::GetIEDInfoList()
{
	return &IEDInfos;
}

IEDLoader::IEDLoader()
{
	SCLModel = SCLModelAdapter::GetInstance();
	voltageLevelIndex = 0;
	substationIndex   = 0;
	m_bShowStationOnStart = true;

    m_SCLValid = CSCLValid::GetInstance();
    checkThread = new CheckThread(m_SCLValid);
}

IEDLoader::~IEDLoader()
{
	;
}