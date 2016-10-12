/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      GraphicLoader.h
  * Author:         zhang-xch
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   变电站加载管理
  */ 
#ifndef IEDLoader_H
#define IEDLoader_H

#include <qstring>
#include <QVector>
#include "SCLModelSvrAdapterInterface.h"
#include "SCLValid.h"
#include "checkthread.h"

struct IEDItem
{
public:
	QString IEDName;
	QString IEDKey;
};

struct SubNetItem 
{
	QString  subNetName;
	QString  subNetKey;
};

class IEDLoader
{
public:
	void LoadVoltageLevelNameList();
	void LoadSubstationNameList();
	QSubStationInfo *GetSubstationInfo(QString substationName);
	QString GetSubstationImportTime(QString substationKey);
	QVector<QString> *GetVoltageLevelNameList();
	QVector<QString> *GetSubstationNameList();

	int     GetCurrentVoltageLevelIndex();
	void    SetCurrentVoltageLevelIndex(int index);

	void    SetCurrentSubstationID(QString substationID);//chenxin add 2014.10.26
	void    UnloadScdFile(QString SubstationID);
	QString    GetCurrentSubstationID();//chenxin add 2014.10.26


	SubNetItem GetSubNetItemFromSubNetName(QString name);
	IEDItem    GetIEDItemFromIEDName(QString name);

	void LoadSubNetList();
	QVector<SubNetItem> *GetSubNetList();

	void LoadIEDList();
	QVector<IEDItem> *GetIEDList();

	void LoadHItemList();
	QVector<QHItemInfo> *GetHItemList();

	void LoadIEDInfoList();
	QVector<QIEDInfo> *GetIEDInfoList();

	void setShowStationOnStart(bool bStart){m_bShowStationOnStart = bStart;}

	IEDLoader();
	~IEDLoader();
private:
	QVector<QString>           voltageLevelNameLists;
	QVector<QString>           subStationNameLists;
	int                        voltageLevelIndex;

	int                        substationIndex;
	QString                    curSubstationID;//chenxin add 2014.10.26

	QVector<SubNetItem>        subNets;   
	QVector<IEDItem>           IEDs;
	QVector<QHItemInfo>        HItems;
	QVector<QIEDInfo>          IEDInfos;
	QSubStationInfo			   substationInfo;
	SCLModelSvrAdapterInterface   *SCLModel;

	bool                    m_bShowStationOnStart;

public:
    // 在导入时 进行SSD检测
    CheckThread  	*checkThread;
    CSCLValid		*m_SCLValid;
    void StartSSDVaildate(QString substationID);
};

#endif