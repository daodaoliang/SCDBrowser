#ifndef IEDGRAPHICMANGER_H
#define IEDGRAPHICMANGER_H

#include "qdatastream.h"
#include "qfile.h"
#include "SecuMeasStructDefine.h"
#include "SCLModelSvrAdapter.h"
#include "GraphicNameGenerator.h"
#include "GraphicSvrAdapter.h"
#include "HisGraphicSvrSliceAdapter.h"

class IEDGraphicsInfo
{
public:
	QString name;
	QString type;
	IEDInfo maintanceIED;
	IEDInfo relateIED;
    QByteArray graphicData;
};

class IEDGraphicsItem
{
public:
    IEDGraphicsInfo  graphicInfo;
	bool             checkedFlag;
};

class IEDItem
{
public:
	IEDInfo maintanceIED;
	QVector<IEDGraphicsItem> graphicItems;
};

//图形管理器
class IEDGraphicsManger
{
public:
	static IEDGraphicsManger * GetInstance()  
	{  
		if (m_pInstance == NULL)
			m_pInstance = new IEDGraphicsManger();  
		return m_pInstance; 
	};

	QString GetCurPrimIEDKey()
	{
		QString substationKey = sclModelSvrAdapter->GetChildKeyFromParentKeyAndChildName(voltageLevelName,substationName);
		QString panelKey      = sclModelSvrAdapter->GetChildKeyFromParentKeyAndChildName(substationKey,
			IEDs[IEDIndex].graphicItems[graphicIndex].graphicInfo.maintanceIED.panel);
		QString IEDKey        = sclModelSvrAdapter->GetChildKeyFromParentKeyAndChildName(panelKey,
			IEDs[IEDIndex].graphicItems[graphicIndex].graphicInfo.maintanceIED.IED);
		return IEDKey;
	};

	QString GetCurSlaveIEDkey()
	{
		QString substationKey = sclModelSvrAdapter->GetChildKeyFromParentKeyAndChildName(voltageLevelName,substationName);
		QString relatePanelKey      = sclModelSvrAdapter->GetChildKeyFromParentKeyAndChildName(substationKey,
			IEDs[IEDIndex].graphicItems[graphicIndex].graphicInfo.relateIED.panel);
		QString relateIEDKey        = sclModelSvrAdapter->GetChildKeyFromParentKeyAndChildName(relatePanelKey,
			IEDs[IEDIndex].graphicItems[graphicIndex].graphicInfo.relateIED.IED);
		return relateIEDKey;
	};

	bool ReLoadGraphicInfos(QString voltLeveName,QString substName,QVector<IEDInfo> *graphicIEDs)
	{
		IEDs.clear();
		voltageLevelName = voltLeveName;
		substationName   = substName;
		IEDIndex         = 0;
		graphicIndex     = 0;

		for(int i=0;i<graphicIEDs->size();i++)
		{
			IEDItem tempIEDItem;
			tempIEDItem.maintanceIED.panel = graphicIEDs->at(i).panel;
			tempIEDItem.maintanceIED.IED = graphicIEDs->at(i).IED;
			IEDs.push_back(tempIEDItem);
		}
		AnalysisGraphicsName();
		return true;
	};

    //分析检修设备对应哪些图
    bool AnalysisGraphicsName()
	{
		GraphicNameGenerator translator;
		QVector<QString> relateIEDs;
		for(int i=0;i<IEDs.size();i++)
		{
			QString maintanceIEDKey = voltageLevelName + "." + substationName + "."
			                 + IEDs[i].maintanceIED.panel + "." + IEDs[i].maintanceIED.IED;
			//综合图
			IEDGraphicsItem mainGraphicItem;
			QString mainGraphicName = translator.FromMaintanceIEDInfoToGraphicName(IEDs[i].maintanceIED,IEDs[i].maintanceIED,GRAPHIC_TYPE_NO_INFOMATION_FLOW);
			mainGraphicItem.checkedFlag = true;
			mainGraphicItem.graphicInfo.maintanceIED.panel = IEDs[i].maintanceIED.panel;
			mainGraphicItem.graphicInfo.maintanceIED.IED = IEDs[i].maintanceIED.IED;
			mainGraphicItem.graphicInfo.type = translator.FromGraphicTypeNoToGraphicTypeName(GRAPHIC_TYPE_NO_COMPREHENSIVE);
			mainGraphicItem.graphicInfo.relateIED.panel = "";
			mainGraphicItem.graphicInfo.relateIED.IED = "";
			mainGraphicItem.graphicInfo.name = mainGraphicName;
			IEDs[i].graphicItems.push_back(mainGraphicItem);

			//通信图
			IEDGraphicsItem signalGraphicItem;
			QString signalGraphicName = translator.FromMaintanceIEDInfoToGraphicName(IEDs[i].maintanceIED,IEDs[i].maintanceIED,GRAPHIC_TYPE_NO_SIGNAL);
			signalGraphicItem.checkedFlag = true;
			signalGraphicItem.graphicInfo.maintanceIED.panel = IEDs[i].maintanceIED.panel;
			signalGraphicItem.graphicInfo.maintanceIED.IED = IEDs[i].maintanceIED.IED;
			signalGraphicItem.graphicInfo.type = translator.FromGraphicTypeNoToGraphicTypeName(GRAPHIC_TYPE_NO_SIGNAL);
			signalGraphicItem.graphicInfo.relateIED.panel = "";
			signalGraphicItem.graphicInfo.relateIED.IED = "";
			signalGraphicItem.graphicInfo.name = signalGraphicName;
			IEDs[i].graphicItems.push_back(signalGraphicItem);

			relateIEDs.clear();
			sclModelSvrAdapter->GetIEDListByLogicalRealitionWithCurrentIED(maintanceIEDKey,&relateIEDs);
			for(int j=0;j<relateIEDs.size();j++)
			{
				IEDGraphicsItem linkGraphicItem;
				linkGraphicItem.checkedFlag = true;
				linkGraphicItem.graphicInfo.maintanceIED.panel = IEDs[i].maintanceIED.panel;
				linkGraphicItem.graphicInfo.maintanceIED.IED = IEDs[i].maintanceIED.IED;
				linkGraphicItem.graphicInfo.type = translator.FromGraphicTypeNoToGraphicTypeName(GRAPHIC_TYPE_NO_COMMUNICATION);
				QString panelKey = sclModelSvrAdapter->GetParentKey(relateIEDs[j]);
				linkGraphicItem.graphicInfo.relateIED.panel = sclModelSvrAdapter->GetNameFromKey(panelKey);
				linkGraphicItem.graphicInfo.relateIED.IED = sclModelSvrAdapter->GetNameFromKey(relateIEDs[j]);
				QString linkGraphicName = translator.FromMaintanceIEDInfoToGraphicName(IEDs[i].maintanceIED,linkGraphicItem.graphicInfo.relateIED,GRAPHIC_TYPE_NO_COMMUNICATION);
				linkGraphicItem.graphicInfo.name = linkGraphicName;
				IEDs[i].graphicItems.push_back(linkGraphicItem);
			}

			analysisProgress = int((float)i/IEDs.size());
		}
		return true;
	};

   int GetCurIEDIndex()
   {
	   return IEDIndex;
   };

   int GetCurGraphicIndex()
   {
	   return graphicIndex;
   };

   void SetCurIEDIndex(int index)
   {
	   if(index<IEDs.size()&&
		   index>=0)
	   {
		   IEDIndex = index;
	   }
   };

   void SetCurGraphicIndex(int index)
   {
	   if(index<IEDs[IEDIndex].graphicItems.size()&&
		   index>=0)
	   {
		   graphicIndex = index;
	   }
   };

   int GetAnalysisProgress()
   {
	   return analysisProgress;
   }

   bool SaveGraphicData(int nIEDIndex,int nGraphicIndex)
   {
	   if(graphicSvrAdapter)
	   {
		   QString subStationKey =  SCLModelAdapter::GetInstance()->GetChildKeyFromParentKeyAndChildName(voltageLevelName,substationName);
		   QString version = SCLModelAdapter::GetInstance()->GetSCDVersion(subStationKey);

		   QString graphicName = IEDs[nIEDIndex].graphicItems[nGraphicIndex].graphicInfo.name;
		   QString graphicTypeDesc = IEDs[nIEDIndex].graphicItems[nGraphicIndex].graphicInfo.type;
		   GraphicNameTranslator translator;
		   int typeNo = translator.FromGraphicTypeNameToGraphicTypeNo(graphicTypeDesc);
		   if(!hisGraphicSvrAdapter->AddGraphic(graphicName,version,graphicTypeDesc,
                                    IEDs[nIEDIndex].graphicItems[nGraphicIndex].graphicInfo.graphicData))
			{
                 return false;
			}
		   SetChecked(nIEDIndex,nGraphicIndex,true);
	   }
       return true;
   };

   bool SaveCurGraphicData()
  {
      SaveGraphicData(IEDIndex,graphicIndex);
  };

   bool LoadGraphicDataFromGraphicSvr(int nIEDIndex,int nGraphicIndex)
   {
	   QByteArray tempByteArray;
	   if(graphicSvrAdapter)
	   {
		   QString substationKey = sclModelSvrAdapter->GetChildKeyFromParentKeyAndChildName(voltageLevelName,substationName);
		   QString panelKey      = sclModelSvrAdapter->GetChildKeyFromParentKeyAndChildName(substationKey,
			   IEDs[nIEDIndex].graphicItems[nGraphicIndex].graphicInfo.maintanceIED.panel);
		   QString IEDKey        = sclModelSvrAdapter->GetChildKeyFromParentKeyAndChildName(panelKey,
			   IEDs[nIEDIndex].graphicItems[nGraphicIndex].graphicInfo.maintanceIED.IED);
		   QString relatePanelKey      = sclModelSvrAdapter->GetChildKeyFromParentKeyAndChildName(substationKey,
			   IEDs[nIEDIndex].graphicItems[nGraphicIndex].graphicInfo.relateIED.panel);
		   QString relateIEDKey        = sclModelSvrAdapter->GetChildKeyFromParentKeyAndChildName(panelKey,
			   IEDs[nIEDIndex].graphicItems[nGraphicIndex].graphicInfo.relateIED.IED);


		   QString graphicTypeDesc = IEDs[nIEDIndex].graphicItems[nGraphicIndex].graphicInfo.type;
		   GraphicNameTranslator translator;
		   int typeNo = translator.FromGraphicTypeNameToGraphicTypeNo(graphicTypeDesc);
		   switch (typeNo)
		   {
		   case GRAPHIC_TYPE_NO_COMPREHENSIVE:
			    graphicSvrAdapter->GetIEDIndexGraphic(IEDKey,tempByteArray);
		    	break;
		   case GRAPHIC_TYPE_NO_COMMUNICATION:
			   graphicSvrAdapter->GetTwoIEDGraphic(IEDKey,relateIEDKey,tempByteArray);
			    break;
		   case GRAPHIC_TYPE_NO_SIGNAL:
			   graphicSvrAdapter->GetSignalIEDGraphic(IEDKey,tempByteArray);
			    break;
		   default:
			   break;
		   }
		   SetChecked(nIEDIndex,nGraphicIndex,true);
           IEDs[nIEDIndex].graphicItems[nGraphicIndex].graphicInfo.graphicData = tempByteArray;
	   }
	   return true;
   };

   bool LoadGraphicDataFromHisGraphicSvr(int nIEDIndex,int nGraphicIndex)
   {
	   QByteArray tempByteArray;
	   if(graphicSvrAdapter)
	   {
		   QString subStationKey =  SCLModelAdapter::GetInstance()->GetChildKeyFromParentKeyAndChildName(voltageLevelName,substationName);
		   QString version = SCLModelAdapter::GetInstance()->GetSCDVersion(subStationKey);

		   hisGraphicSvrAdapter->GetGraphicData(IEDs[nIEDIndex].graphicItems[nGraphicIndex].graphicInfo.name,version,
                                    IEDs[nIEDIndex].graphicItems[nGraphicIndex].graphicInfo.graphicData);           
		   SetChecked(nIEDIndex,nGraphicIndex,true);
	   }
	   return true;
   };

   bool LoadGraphicData(int nIEDIndex,int nGraphicIndex)
   {
        if(IsGraphicExist(nIEDIndex,nGraphicIndex))
		{
             return LoadGraphicDataFromHisGraphicSvr(nIEDIndex,nGraphicIndex);
		}
		else
		{
             return LoadGraphicDataFromGraphicSvr(nIEDIndex,nGraphicIndex);
		}
   };

   bool LoadCurGraphicData()
   {
        return LoadGraphicData(IEDIndex,graphicIndex);
   };

	void SetAllCheckedState(bool useFlag)
	{
		for(int i=0;i<IEDs.size();i++)
		{
			for(int j=0;j<IEDs[i].graphicItems.size();j++)
			{
				IEDs[i].graphicItems[j].checkedFlag = useFlag;
			}
		}
	};

	QVector<IEDItem> *GetIEDItems()
	{
		return &IEDs;
	};

	//判断图是否存在
    bool IsGraphicExist(int nIEDIndex,int nGraphicIndex)
	{
        if(hisGraphicSvrAdapter)
		{
		   QString graphicName = IEDs[nIEDIndex].graphicItems[nGraphicIndex].graphicInfo.name;
		   QString graphicTypeDesc = IEDs[nIEDIndex].graphicItems[nGraphicIndex].graphicInfo.type;

		   QString subStationKey =  SCLModelAdapter::GetInstance()->GetChildKeyFromParentKeyAndChildName(voltageLevelName,substationName);
		   QString version = SCLModelAdapter::GetInstance()->GetSCDVersion(subStationKey);
		   return hisGraphicSvrAdapter->IsGraphicExist(graphicName,version);
		}
        return false;
	};

	//存储图
    bool SaveGraphic()
	{
		for(int i=0;i<IEDs.size();i++)
		{
			for(int j=0;j<IEDs[i].graphicItems.size();j++)
			{
				if(IEDs[i].graphicItems[j].checkedFlag)
				{
                    if(IEDs[i].graphicItems[j].graphicInfo.graphicData.size()<=0)
					{
						//生成图的数据
						LoadGraphicData(i,j);
					}
                    //保存
                    SaveGraphicData(i,j);
				}
			}
		}
		return true;
	};


	void SetChecked(int IEDIndex,int graphicIndex,bool flag)
	{
		if(IEDIndex<IEDs.size())
		{
			if(graphicIndex<IEDs[IEDIndex].graphicItems.size())
			{
				IEDs[IEDIndex].graphicItems[graphicIndex].checkedFlag = flag;
			}
		}
	};

	bool GetChecked(int IEDIndex,int graphicIndex)
	{
		if(IEDIndex<IEDs.size())
		{
			if(graphicIndex<IEDs[IEDIndex].graphicItems.size())
			{
				return IEDs[IEDIndex].graphicItems[graphicIndex].checkedFlag;
			}
		}
	};

	bool ReLoadGraphicNames()
	{
		return true;
	};

public:
	IEDGraphicsManger()
	{
		sclModelSvrAdapter = SCLModelAdapter::GetInstance();
		graphicSvrAdapter  = GraphicSvrAdapter::GetInstance();
        hisGraphicSvrAdapter = HisGraphicSvrSliceAdapter::GetInstance();
		IEDIndex = 0;
		graphicIndex = 0;
		analysisProgress = 0;
	};

	~IEDGraphicsManger(){};

private:
	static IEDGraphicsManger  *m_pInstance;

	class CGarbo
	{  
	public:  ~CGarbo()  
			 {  
				 if (IEDGraphicsManger::m_pInstance) 
				 {
					 delete IEDGraphicsManger::m_pInstance;  
				 }
			 }
	};
	static CGarbo Garbo;

	QString voltageLevelName;
	QString substationName;
	QVector<IEDItem> IEDs;

	int IEDIndex;
	int graphicIndex;

	int analysisProgress;

    SCLModelSvrAdapterInterface *sclModelSvrAdapter;
	GraphicSvrAdapterInterface  *graphicSvrAdapter;
    HisGraphicSvrSliceAdapter *hisGraphicSvrAdapter;
};

#endif