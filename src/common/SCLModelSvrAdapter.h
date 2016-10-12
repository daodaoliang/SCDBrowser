#ifndef SCLModelAdapter_H
#define SCLModelAdapter_H

#include "SCLModelSvrAdapterInterface.h"
#include "QtCore/qcoreapplication.h"
#include "../../Svr/SCLModelSvr/SCLModelServerImpl.h"
//#include "SCLModelAdapterForClient.h"
#include "SCLModelAdapterForClient.h"//chenxin add 2014.10.17
#include "InterfaceInvokeStat.hxx"//chenxin add 2014.10.23
#include "QMap.h"
using namespace std;

//类名：SCLModelAdapter
//功能：将SCLModelServerImpl提供的SCL模型服务转换成本地接口
//特点：单例模式、适配器模式

class SCLModelAdapter : public SCLModelSvrAdapterInterface
{
public:

	virtual QString GetSubstationImportTime(QString subStationKey);
	//模型通用接口
	virtual QString GetParentKey(QString childKey);                                           //根据子节点Key获得父节点Key
	virtual QString GetParentName(QString childKey);                                          //根据子节点Key获得父节点的Name
	virtual QString GetNameFromKey(QString key);                                              //根据Key获得名称
	virtual QString GetChildKeyFromParentKeyAndChildName(QString parentKey,QString childName);//根据父节点的Key和子节点Name获得子节点的Key 

	//模型基本访问接口
	virtual bool   GetVoltageLevelList(QVector<QString>* nameList);                                          //获得电压等级名列表（同Key）
	virtual bool   GetSubstationList(QString voltageLevelKey,QVector<QString>* subStationKeyList);           //获得变电站Key列表
	virtual bool   GetSubstationInfo(QString subStationKey, QSubStationInfo& subStationInfo);//获得变电站（特定SCD版本）信息	
	virtual QString GetSubstationPrivateXMLInfo(const QString& subStationKey);//获得变电站private标签XML原文
	virtual QString GetSubstationHeaderXMLInfo(const QString& subStationKey);//获得变电站Header标签XML原文
	virtual bool   GetSCDHItem(QString subStationKey, QVector<QHItemInfo>& hItemList);//获得SCD历史修订记录
	virtual bool   GetSubNetList(QString subStationKey, QVector<QString>& subNetKeyList);//获得子网Key列表
	virtual bool   GetSubNetInfo(QString subNetKey, QSubNetInfo& subNetInfo);//获得子网信息
	virtual QString GetSubNetXMLInfo(const QString& subNetKey);//获得子网XML片段;
	virtual bool   GetAPInfo(QString subNetKey, QString IEDName, QString APName, QAPInfo& apInfo);//获得访问点信息
	virtual bool   GetPanelList(QString subStationKey,QVector<QString>* subPanelKeyList);                    //获得屏柜Key列表
	virtual bool   GetIEDList(QString panelKey,QVector<QString>* subIEDKeyList);                             //获得装置Key列表
	virtual bool   GetIEDListBySubNet(QString subNetKey, QVector<QString>& subIEDKeyList);//获得装置Key列表(根据子网)
	virtual bool   GetIEDInfo(QString IEDKey, QIEDInfo& IEDInfo);//获得装置信息	 
    virtual QString GetIEDXMLInfo(QString& IEDKey);//获得IED的XML信息
	virtual bool   GetIEDType(QString IEDKey,int* type);                                                     //获得装置类型
	virtual QString GetSCDVersion(QString subStationKey);                                                    //获得当前模型的SCD文件版本号
	virtual bool   IsVoltageLevelExist(QString voltageLevelName);                                            //是否存在电压等级
	virtual bool   IsSubstationExist(QString substationKey);                                                 //是否存在变电站		
	virtual bool   GetPortsList(QString IEDKey,QVector<QString>* portKeys);                                  //获得装置的物理端口列表
	virtual QString GetPortDescByPortKey(QString portKey);                                                   //获得物理端口的描述
	virtual bool   GetLinksListOfIED(QString IEDKey,QVector<QPhysLink>* links);                              //获得装置的物理链接列表  
	virtual bool   GetInSignalsListOfIED(QString IEDKey,QSignalTypeEnum signalType,QVector<QString>* inSignalKeys);                     //获得装置的输入信号列表
	virtual bool   GetOutSignalsListOfIED(QString IEDKey,QSignalTypeEnum signalType,QVector<QString>* outSignalKeys);                   //获得装置的输出信号列表            
	virtual bool   GetVritualLinksOfIED(QString IEDKey,QSignalTypeEnum signalType,QVector<QVirtualLink>* links);                        //获得装置的虚拟链接列表（装置和装置之间）
	virtual bool   GetInternalVritualLinksOfIED(QString IEDKey,QSignalTypeEnum signalType,QVector<QVirtualLink>* links);                //获得装置内部的虚拟链接列表（装置内部）     
	virtual bool   GetFunctionsList(QString IEDKey,QVector<QString>* protectFuncKeys);                       //获得装置的功能列表
	virtual bool   GetVirtualLinksListOfPhysicsLink(QPhysLink phyLink,QSignalTypeEnum signalType,QVector<QVirtualLink>* links);         //获得物理链接关联的虚拟链接列表
	virtual bool   GetTripsList(QString IEDKey,QVector<QString>* tripKeys);                                  //获得装置的压板列表
	virtual bool   GetInfoOfInSignal(QString signalKey,QSignalInfo *signalInfo);                             //获得输入信号的信息  
	virtual bool   GetInfoOfOutSignal(QString signalKey,QSignalInfo *signalInfo);                            //获得输出信号的信息
	virtual bool   GetInfoOfDSSettingOutSignal(QString signalKey,QDSSettingSignalInfo *signalInfo);

	virtual bool   GetSignalInfosByIEDKeyAndCBKey(const QString masterIEDKey, const QString slaveIEDKey, const QString CBkey, QList<QSignalInfo> *mSigInfos, QList<QSignalInfo> *sSigInfos);
	virtual bool   GetSignalInfosByIEDKey(const QString IEDKey, QList<QSignalInfo> *mSigInfos, QList<QSignalInfo> *sSigInfos);

	virtual bool   GetCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList);						 //获取当前IED的数据集列表
	virtual bool   GetDsSettingListByIED(QString IEDKey, QVector<QDataSetInfo>* dsSettingList);
	virtual bool   GetCtrlBlockInfo(QString ctrlBlockKey, QCtrlBlockInfo& cbInfo);								 //获取数据集信息
	virtual bool   GetDataSetSignalsByCtrlBlockID(QString cbKey, QVector<QString>* dsSignalList);//获取数据集虚信号
	virtual bool   GetSlaveCtrlBlockByIEDKeys(QString masterKey, QString slaveKey, QVector<QString>* ctrlBlockList);	//根据主从IED的Key获取从IED用到的控制块
	virtual bool   GetOutSignalsByCBKey(QString masterIEDKey, QString cbKey, QVector<QString>* outSignals);		//通过控制块获取虚链接;
	virtual bool   GetOutSignalsByDataSetKey(QString masterIEDKey, QString dataSetKey, QVector<QString>* outSignals);
	virtual bool   GetAssoIEDListByCBKey(QString cbKey, QVector<QString>* assoIEDKeys);		//通过控制块获取对端IED;

	virtual int    GetItemChangeType(QString ItemName);											//获取差异类型
	virtual int    GetVLinkChangeType(QString masterIEDName, QString slaveIEDName, QString cbName);//获取虚回路差异类型(信息流图);

	virtual bool   GetPhyPortInfo(QString pPortKey, QPhyPortInfo& pPortInfo);				//获取物理端口信息;
	virtual bool   GetPhyPortListByIED(QString IEDKey, QVector<QString>* pPortList);		//根据IED获取物理端口;
	virtual bool   GetPhyPortListByInSignalID(QString signalKey, QVector<QString> &pPortList);			//根据信号获取端口;
	virtual bool   GetPhyPortKeyByOutSignalID(QString signalKey, QString* pPortKey);		//根据输出信号获取端口;

	//高级访问接口
	virtual bool   GetIEDListByLogicalRealitionWithCurrentIED(QString IEDKey,QVector<QString>* associatedIEDKeys);    //获得IED的关联IED列表（逻辑）
	virtual bool   GetRealitionTypeOfSrcIEDAndDestIED(QString srcIEDKey,QString destIEDKey,int *type);        //获得IED之间的关联类型（0：源IED为输入）
																										//1: 源IED为输出
																										//2: 源IED既是输入又是输出
																										//-1:源IED和目的IED没有关系
	virtual bool   GetIEDListByPhysicalRealitionWithCurrentIED(QString IEDKey,QVector<QString>* associatedIEDKeys);   //获得IED的关联IED列表（物理）  
	virtual bool   GetPhysicsLinkAssociatedIED(QString srcIEDKey,QString destIEDKey,QVector<QPhysLink>* links);       //获得IED与IED之间物理链接（多个链接表示多个物理通道，0个链接表示无直接连接）
	virtual bool   GetLinksBetweenPort(QString srcPortKey,QString destPortKey,QVector<QPhysLink>* links);             //获得原port与目的port之间的物理链接（多个链接表示之间有多个设备）
	virtual bool   GetVirtualSignalsAssociatedIED(QString srcIEDKey,QString destIEDKey,QSignalTypeEnum signalType,QVector<QVirtualLink>* links); //获得IED与IED之间虚信号   
	virtual bool   GetPortsListOfPort(QString portKey,QVector<QString>* portKeys);                                    //获得与物理端口关联的其它物理端口
	virtual bool   GetPhysicsLinkeByPort(QString srcPortKey,QString destPortKey,QPhysLink* link);                      //根据两个端口获得物理链接   
	virtual bool   GetInSignalsListOfOutSignal(QString outSignalKey,QVector<QString>* inSignalKeys);                  //获得输出信号关联的输入信号列表（两个装置之间）   
	virtual bool   GetOutSignalsListOfInSignal(QString inSignalKey,QVector<QString>* outSignalKeys);                  //获得输入信号关联的输出信号列表（两个装置之间） 
	virtual bool   GetInternalOutSignalsListOfInSignal(QString inSignalKey,QVector<QString>* outSignalKeys);          //获得输入信号关联的输出信号列表（装置内部）
	virtual bool   GetInternalInSignalsListOfOutSignal(QString outSignalKey,QVector<QString>* inSignalKeys);          //获得输出信号关联的输入信号列表（装置内部）
	virtual bool   GetVirtualLinkBySignal(QString srcSignalKey,QString destSignalKey,QVirtualLink* link);              //根据两个信号获得虚拟链接        
	virtual bool   GetFunctionListOfInSignal(QString inSignalKey,QVector<QString>* funcKeys);                         //获得输入信号关联的功能列表（本装置功能）
	virtual bool   GetFunctionsListOfOutSignal(QString outSignalKey,QVector<QString>* funcKeys);                      //获得输出信号关联的功能列表 （其它装置功能）
	virtual bool   GetFunctionsListOfOutSignalWithDestIED(QString outSignalKey,QString destIEDKey,QVector<QString>* funcKeys); //获得输出信号相对于某装置的功能列表（某装置功能）  
	virtual bool   GetFunctionsListOfPort(QString portKey,QVector<QString>* FuncKeys);                                //获得物理端口功能列表     
	virtual bool   GetFunctionsListOfTrip(QString tripKey,QVector<QString>* FuncKeys);                                //获得压板对应的功能列表     
	virtual QString GetFunctionDesc(QString functionKey);                                                             //获得功能的功能描述
	virtual QString GetFunctionTripKey(QString functionKey);                                                          //获得功能的功能压板（可能不存在） 
	virtual QString GetIEDTripKey(QString IEDKey);                                                                    //获得设备的压板（表示断电一类的操作） 
	virtual QString GetPortTripKey(QString portKey);                                                                  //获得端口的压板（表示拨光纤一类的操作） 
	virtual QString GetSignalTripKey(QString signalKey);                                                              //获得信号的压板（软压板）   


	//模型编辑接口
	virtual bool   AddVoltageLevel(QString voltageLevelName);                                         //增加电压等级
	virtual bool   DeleteVoltageLevel(QString voltageLevelName);                                      //删除电压等级
	virtual bool   AddSubstation(QString voltageLevelKey,QString subName/*,QByteArray& fileData*/);        //增加变电站模型
	virtual int    GetSubstationAddedProgress();                                                      //获得SCD导入的进度
	virtual bool   DeleteSubstation(QString substationKey);                                           //删除变电站模型
	virtual bool   UpdateSubstationModel(QString voltageLevelKey,QString subName,QString SCDFilePath);//更新变电站模型
	virtual bool   AddPanelForSubstation(QString subKey,QString paneName);                            //为变电站增加屏柜
	virtual bool   DeletePanelForSubstation(QString PanelKey);                                        //为变电站删除屏柜   		
	virtual bool   AddIEDForPanel(QString panelKey,QString IEDName);                                  //为屏柜增加装置
	virtual bool   DeleteIEDForPanel(QString IEDKey);                                                 //为屏柜删除装置  
	virtual bool   AddPortForIED(QString IEDKey,QString portName);                                    //为装置增加物理端口
	virtual bool   DeletePortForIED(QString portKey);                                                 //为装置删除物理端口
	virtual bool   AddInSignalForIED(QString IEDKey,QString inSignalName);                            //为装置增加输入信号
	virtual bool   DeleteInSignalForIED(QString inSignalKey);                                         //为装置删除输入信号 
	virtual bool   AddOutSignalForIED(QString IEDKey,QString outSignalName);                          //为装置增加输出信号
	virtual bool   DeleteOutSignalForIED(QString outSignalKey);                                       //为装置删除输出信号
	virtual bool   AddFunction(QString IEDKey,QString funName,QString funcDesc);                      //为装置增加功能 
	virtual bool   ModifyFunction(QString funKey,QString funcDesc);                                   //为装置修改功能描述    
	virtual bool   DeleteFunction(QString funKey);                                                    //为装置删除功能 
	virtual bool   SetDescForPort(QString portKey,QString desc);                                      //设置物理端口的描述     
	virtual bool   SetInSignalsListOfOutSignal(QString outSignalKey,QVector<QString>* inSignalKeys);          //设置输出信号关联的输入信号列表（两个装置之间）   
	virtual bool   SetOutSignalsListOfInSignal(QString inSignalKey,QVector<QString>* outSignalKeys);          //设置输入信号关联的输出信号列表（两个装置之间） 
	virtual bool   SetInternalOutSignalsListOfInSignal(QString inSignalKey,QVector<QString>* outSignalKeys);  //设置输入信号关联的输出信号列表（装置内部）
	virtual bool   SetInternalInSignalsListOfOutSignal(QString outSignalKey,QVector<QString>* inSignalKeys);  //设置输出信号关联的输入信号列表（装置内部）
	virtual bool   AddFunctionOfInSignal(QString inSignalKey,QString funKey);                         //为输入信号增加功能关联  
	virtual bool   DeleteFunctionOfInSignal(QString inSignalKey,QVector<QString>* funcKeys);          //为输入信号删除功能关联            	
	virtual bool   AddTripForIED(QString IEDKey,QString tripName);                                     //增加压板  
	virtual bool   DeleteTripForIED(QString tripKey);                                                  //删除压板  
	virtual bool   SetFunctionTripKey(QString functionKey,QString tripKey);                            //设置功能的功能压板（可能不存在） 
	virtual bool   SetIEDTripKey(QString IEDKey,QString tripKey);                                      //设置设备的压板（表示断电一类的操作） 
	virtual bool   SetPortTripKey(QString portKey,QString tripKey);                                    //设置端口的压板（表示拨光纤一类的操作） 
	virtual bool   SetSignalTripKey(QString signalKey,QString tripKey);                                //设置信号的压板（软压板）   
	//压板状态
	virtual bool   GetSoftTripState(QString tripKey,bool &state);                //获得压板状态
	virtual bool   SetSoftTripState(QString tripKey,bool state);     //设置压板状态
	
	// 取得子网下IED的AP列表
	virtual bool GetAPListOfIEDFromSubnet(const QString& subNetKey, const QString IEDName, QVector<QString> &APList);
	// 取得子网下IED某个AP包含的物理端口列表
	virtual bool GetPortListOfIEDAPFromSubNet(const QString& subNetKey, const QString IEDName, const QString APName, QVector<QString> &PortList);
	// 取得IED下AP的列表
	virtual bool GetAPListFromIED(const QString& IEDKey, QVector<QString> &APList);
	// 取得IED下某个AP对应的物理端口列表
	virtual bool GetAPPortListFormIED(const QString& IEDKey, const QString &APName, QVector<QString> &PortList);

public:
    virtual bool GetSSDInfoFromSCD(const QString subStationKey, SubstationCache &ssdInfo);

  static SCLModelAdapter * GetInstance()  
  {  
	  if (m_pInstance == NULL)
		  m_pInstance = new SCLModelAdapter();  
	  return m_pInstance; 
  };

private:

	inline void CreatCDCDescMap()
	{
		m_CDCDescMap.insert("SPS", "单点状态");
		m_CDCDescMap.insert("DPS", "双点状态");
		m_CDCDescMap.insert("INS", "整数状态");
		m_CDCDescMap.insert("SIG", "状态信号组");
		m_CDCDescMap.insert("ACT", "继电保护激活信息");
		m_CDCDescMap.insert("ACD", "方向保护激活信息");
		m_CDCDescMap.insert("SEC", "安全违规计数");
		m_CDCDescMap.insert("BCR", "二进制计数读数");

		m_CDCDescMap.insert("MV", "测量值");
		m_CDCDescMap.insert("CMV", "复数测量值");
		m_CDCDescMap.insert("SAV", "采样值");
		m_CDCDescMap.insert("WYE Class", "Y接法类");
		m_CDCDescMap.insert("Delta DEL", "△接法类");
		m_CDCDescMap.insert("SEQ", "相序");
		m_CDCDescMap.insert("HVWYE", "Y接法谐波值类");
		m_CDCDescMap.insert("HDEL", "△接法谐波值类");

		m_CDCDescMap.insert("SPC", "可控单点");
		m_CDCDescMap.insert("DPC", "可控双点");
		m_CDCDescMap.insert("INC", "可控整数状态");
		m_CDCDescMap.insert("BSC", "二进制可控步位置信息");
		m_CDCDescMap.insert("ISC", "整数可控步位置信息");

		m_CDCDescMap.insert("APC", "可控模拟设点信息");

		m_CDCDescMap.insert("SPG", "单点状态设置");
		m_CDCDescMap.insert("ING", "整数状态设置");

		m_CDCDescMap.insert("ASG", "模拟定值");
		m_CDCDescMap.insert("CURVE", "曲线定值");

		m_CDCDescMap.insert("DPL", "设备铭牌");
		m_CDCDescMap.insert("LPL", "逻辑节点铭牌");
		m_CDCDescMap.insert("CSD", "曲线形状描述");
	}

	SCLModelAdapter()
	{
		try 
		{
			SCLServerManger    = new SclDacLib; //chenxin add 2014.10.17
			QString path;  
			path = QCoreApplication::applicationDirPath();
			path = path + "/../data";
			SCLServerManger->Startup(path.toLocal8Bit().constData()); //chenxin add 2014.10.17
			SCLServerManger->GetImportedFileMarks(); //chenxin add 2014.10.17
			// SiisEnableStat(true); // 统计各接口的访问次数和时间
			SCLModelSvrAdapter = new SCLModelAdapterForClient;
			CreatCDCDescMap();
		}
		catch (...) 
		{
			SCLModelSvrAdapter = NULL; 
			SCLServerManger    = NULL; //chenxin add 2014.10.17
		}
	};

	static SCLModelAdapter    *m_pInstance;
	SCLModelAdapterForClient        *SCLModelSvrAdapter;
	SclDacLib *SCLServerManger; //chenxin add 2014.10.17
    // 区分输入输出信号，防止一个信号既是输入又是输出
	QMap<QString,QSignalInfo>  inSignalInfosMap;//chenxin add 2014.10.24
    QMap<QString,QSignalInfo>  outSignalInfosMap;//chenxin add 2014.10.24
	QMap<QString,QCtrlBlockInfo>  ctrlBlockInfosMap;//chenxin add 2014.10.24

	class CGarbo
	{  
	public:  ~CGarbo()  
			 {  
				 if (SCLModelAdapter::m_pInstance) 
				 {
					 //SCLModelAdapter::SCLServerManger->Shutdown(); //chenxin add 2014.10.17
					 delete SCLModelAdapter::m_pInstance;  
				 }
			 }
	};
	static CGarbo Garbo;

	QString GetCDCDesc(QString cdc);
	QMap<QString, QString> m_CDCDescMap;
};

#endif