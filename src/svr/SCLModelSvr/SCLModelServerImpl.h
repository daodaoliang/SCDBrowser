#pragma once

// #include "db_cxx.h"
// #include <db.h>
// #include "RDB\RDBenv.h"
#include <vector>
// #include "SCDLoader.h"
// #include "SCDUnLoader.h"
#include "SCLObjectInfoDefineByClient.h"

using namespace std;

/*
enum SignalTypeEnum
{
	typeAll,
	typeGoose,
	typeSV
};

struct PhysLinkInfo
{
	::std::string srcPort;
	::std::string destPort;
};

struct VirtualLinkInfo
{
	int iOrd;
	::std::string srcSignal;
	::std::string destSignal;
	::std::string inSigDesc;
};

struct SubStationInfo
{
	::std::string Name;
	::std::string crc;
	::std::string desc;     
	::std::string version;
	::std::string reversion;
	::std::string fileName;
	::std::string filePath;
	::std::string toolID;
};

struct SubNetInfo
{
	::std::string name;
	::std::string type;
	::std::string desc;      
};

struct HItemInfo
{
	::std::string substationName;
	::std::string version;
	::std::string reversion;
	::std::string who;
	::std::string when;
	::std::string what;
	::std::string why;        
};

struct DEVInfo
{
	::std::string name;
	::std::string crc;
	::std::string desc;      
	::std::string type;
	::std::string manu;
	::std::string funVer;
	::std::string configVer;
	int GooseInSigCount;
	int GooseOutSigCount;
	int SVInSigCount;
	int SVOutSigCount;
};

struct SignalInfo
{
	int iOrd;
	int iOutSigOrd;
	::std::string signalName;
	::std::string signalPath;
	::std::string signalDesc;
	int signalType;

	::std::string dataSet;
	::std::string fc;
	int state;
	::std::string softTripKey;

	::std::string iedName;
	::std::string portIndex;

	::std::string BDAType;
	::std::string plugType;
	::std::string LnName;
	::std::string LnDesc;
	::std::string DOIDesc;
	::std::string dUVal;
	::std::string ctrlBlockID;
};

struct APInfo
{
	::std::string name;
	::std::string desc;
	::std::string IEDName;
	::std::string subNetname;
	::std::string ipAddr;
	::std::string macAddr;      
};

struct CtrlBlockInfo 
{
	::std::string name;
	::std::string type;
	::std::string APPID;
	::std::string apName;
	::std::string apMAC;
	::std::string apVLAN_ID;
	::std::string apVLAN_PRIORITY;
	::std::string apAPPID;
	::std::string dataSetName;
	::std::string dataSetAddr;

	::std::string confRev;
	int ASDU;
	::std::string smpRate;
	::std::string gocbRef;

	::std::string maxTime;
	::std::string minTime;
};

struct PhyPortInfo  
{
	::std::string desc;
	::std::string ID;
	::std::string pIndex;
	::std::string plug;
	::std::string type;
	::std::string rtType;	
};*/

class SCLModelServerImpl
{
public:
	//模型通用接口
	QString GetSubstationImportTime();
	virtual ::std::string GetParentKey(const ::std::string& childKey); //根据子节点Key获得父节点Key
	virtual ::std::string GetParentName(const ::std::string& childKey);//根据子节点Key获得父节点的Name
	virtual ::std::string GetNameFromKey(const ::std::string& key);    //根据Key获得名称
	virtual ::std::string GetChildKeyFromParentKeyAndChildName(const ::std::string& parentKey, const ::std::string& childName);//根据父节点的Key和子节点Name获得子节点的Key 

    //模型基本访问接口
	virtual bool GetVoltageLevelList(::std::vector<::std::string>& nameList);   //获得电压等级名列表（同Key）
	virtual bool GetSubstationList(const ::std::string& voltageLevelKey, std::vector<std::string>& subStationKeyList);//获得变电站Key列表
	virtual bool GetSubstationInfo(const ::std::string& subStationKey, SubStationInfo& subStationInfo);//获得变电站（特定SCD版本）信息	
	virtual bool GetSCDHItem(const ::std::string& subStationKey, std::vector<HItemInfo>& hItemList);//获得SCD历史修订记录
	virtual bool GetSubNetList(const ::std::string& subStationKey, std::vector<std::string>& subNetKeyList);//获得子网Key列表
	virtual bool GetSubNetInfo(const ::std::string& subNetKey, SubNetInfo& subNetInfo);//获得子网信息
	virtual bool GetApInfo(const ::std::string& subNetName,const ::std::string& IEDName, APInfo& apInfo);//获得访问点信息
	virtual bool GetPanelList(const ::std::string& subStationKey,std::vector<std::string>& subPanelKeyList);//获得屏柜Key列表
	virtual bool GetIEDList(const ::std::string& panelKey,std::vector<std::string>& subIEDKeyList);//获得装置Key列表
	virtual bool GetIEDListBySubNet(const ::std::string& subNetKey, std::vector<std::string>& subIEDKeyList);//获得装置Key列表(根据子网)
	virtual bool GetIEDInfo(const ::std::string& IEDKey, DEVInfo& IEDInfo);//获得装置信息	
	virtual bool GetIEDType(const ::std::string& IEDKey,  int& type);//获得装置类型
	virtual ::std::string GetSCDVersion(const ::std::string& subStationKey);//获得当前模型的SCD文件版本号
	virtual bool IsVoltageLevelExist(const ::std::string& voltageLevelName);//是否存在电压等级
	virtual bool IsSubstationExist(const ::std::string& substationKey);//是否存在变电站
	virtual bool GetPortsList(const ::std::string& IEDKey,std::vector<std::string>& portKeys);//获得装置的物理端口列表
	virtual ::std::string GetPortDescByPortKey(const ::std::string& portKey);//获得物理端口的描述
	virtual bool GetLinksListOfIED(const ::std::string& IEDKey, std::vector<PhysLinkInfo>& links);//获得装置的物理链接列表
	virtual bool GetInSignalsListOfIED(const ::std::string& IEDKey, SignalTypeEnum signalType,std::vector<std::string>& inSignalKeys);//获得装置的输入信号列表
	virtual bool GetOutSignalsListOfIED(const ::std::string& IEDKey,  SignalTypeEnum signalType,std::vector<std::string>& outSignalKeys);//获得装置的输出信号列表
	virtual bool GetVritualLinksOfIED(const ::std::string& IEDKey,  SignalTypeEnum signalType, std::vector<VirtualLinkInfo>& links);//获得装置的虚拟链接列表（装置和装置之间）
	virtual bool GetInternalVritualLinksOfIED(const ::std::string& IEDKey,  SignalTypeEnum signalType, std::vector<VirtualLinkInfo>& links);//获得装置内部的虚拟链接列表（装置内部）
	virtual bool GetFunctionsList(const ::std::string& IEDKey,std::vector<std::string>& protectFuncKeys);//获得装置的功能列表
	virtual bool GetVirtualLinksListOfPhysicsLink(const PhysLinkInfo& phyLink,  SignalTypeEnum signalType, std::vector<VirtualLinkInfo>& links);//获得物理链接关联的虚拟链接列表
	virtual bool GetTripsList(const ::std::string& IEDKey,std::vector<std::string>& tripKeys);//获得装置的压板列表  
	virtual bool GetInfoOfInSignal(const ::std::string& signalKey, SignalInfo& signalInfo);//获得输入信号的信息  
	virtual bool GetInfoOfOutSignal(const ::std::string& signalKey, SignalInfo& signalInfo);//获得输出信号的信息

	virtual bool GetCtrlBlockListByIED(const ::std::string& IEDKey, std::vector<std::string>& ctrlBlockKeys);//获取当前IED的数据集列表
	virtual bool GetCtrlBlockInfo(const ::std::string& ctrlBlockKey, CtrlBlockInfo& cbInfo);//获取数据集信息
	virtual bool GetDataSetSignalsByCtrlBlockID(const ::std::string& cbKey, std::vector<std::string>& dsSignalList);//获取数据集虚信号
	virtual bool GetSlaveCtrlBlockByIEDKeys(const ::std::string& masterKey, const ::std::string& slaveKey, std::vector<std::string>& ctrlBlockList);//根据主从IED的Key获取从IED用到的控制块
	virtual bool GetOutSignalsByCBKey(const ::std::string& masterIEDKey, const ::std::string& cbKey, std::vector<std::string>& outSignals);		//通过控制块获取虚链接;
	virtual bool GetAssoIEDListByCBKey(const ::std::string& cbKey, std::vector<std::string>& assoIEDKeys);		//通过控制块获取对端IED;

	virtual bool GetPhyPortInfo(const ::std::string& pPortKey, PhyPortInfo& pPortInfo);				//获取物理端口信息;
	virtual bool GetPhyPortListByIED(const ::std::string& IEDKey, std::vector<std::string>& pPortList);		//根据IED获取物理端口;
	virtual bool GetPhyPortKeyByInSignalID(const ::std::string& signalKey, ::std::string& pPortKey);			//根据信号获取端口;
	virtual bool GetPhyPortKeyByOutSignalID(const ::std::string& signalKey, ::std::string& pPortKey);			//根据信号获取端口;

	//高级访问接口
	virtual bool GetIEDListByLogicalRealitionWithCurrentIED(const ::std::string& IEDKey,std::vector<std::string>& associatedIEDKeys);//获得IED的关联IED列表（逻辑）
	virtual bool GetRealitionTypeOfSrcIEDAndDestIED(const ::std::string& srcIEDKey, const ::std::string& destIEDKey, int& type);//获得IED之间的关联类型（0：源IED为输入）																																												//1: 源IED为输出																																											//2: 源IED既是输入又是输																																												//-1:源IED和目的IED没有关系）
	virtual bool GetIEDListByPhysicalRealitionWithCurrentIED(const ::std::string& IEDKey,std::vector<std::string>& associatedIEDKeys);//获得IED的关联IED列表（物理）  
	virtual bool GetPhysicsLinkAssociatedIED(const ::std::string& srcIEDKey, const ::std::string& destIEDKey, std::vector<PhysLinkInfo>& links);//获得IED与IED之间物理链接（多个链接表示多个物理通道，0个链接表示无直接连接）
	virtual bool GetLinksBetweenPort(const ::std::string& srcPortKey, const ::std::string& destPortKey, std::vector<PhysLinkInfo>& links);//获得原port与目的port之间的物理链接（多个链接表示之间有多个设备）
	virtual bool GetVirtualSignalsAssociatedIED(const ::std::string& srcIEDKey, const ::std::string& destIEDKey,  SignalTypeEnum signalType, std::vector<VirtualLinkInfo>& links);//获得IED与IED之间虚信号 
	virtual bool GetPortsListOfPort(const ::std::string& portKey,std::vector<std::string>& portKeys);//获得与物理端口关联的其它物理端口
	virtual bool GetPhysicsLinkeByPort(const ::std::string& srcPortKey, const ::std::string& destPortKey, PhysLinkInfo& link);//根据两个端口获得物理链接 
	virtual bool GetInSignalsListOfOutSignal(const ::std::string& outSignalKey,std::vector<std::string>& inSignalKeys);//获得输出信号关联的输入信号列表（两个装置之间）
	virtual bool GetOutSignalsListOfInSignal(const ::std::string& inSignalKey,std::vector<std::string>& outSignalKeys);//获得输入信号关联的输出信号列表（两个装置之间）
	virtual bool GetInternalOutSignalsListOfInSignal(const ::std::string& inSignalKey,std::vector<std::string>& outSignalKeys);//获得输入信号关联的输出信号列表（装置内部）
	virtual bool GetInternalInSignalsListOfOutSignal(const ::std::string& outSignalKey,std::vector<std::string>& inSignalKeys);//获得输出信号关联的输入信号列表（装置内部）
	virtual bool GetVirtualLinkBySignal(const ::std::string& srcSignalKey, const ::std::string& destSignalKey, VirtualLinkInfo& link);//根据两个信号获得虚拟链接 
	virtual bool GetFunctionListOfInSignal(const ::std::string& inSignalKey,std::vector<std::string>& funcKeys);//获得输入信号关联的功能列表（本装置功能）
	virtual bool GetFunctionsListOfOutSignal(const ::std::string& outSignalKey,std::vector<std::string>& funcKeys);//获得输出信号关联的功能列表 （其它装置功能）
	virtual bool GetFunctionsListOfOutSignalWithDestIED(const ::std::string& outSignalKey, const ::std::string& destIEDKey,std::vector<std::string>& funcKeys);//获得输出信号相对于某装置的功能列表（某装置功能）
	virtual bool GetFunctionsListOfPort(const ::std::string& portKey,std::vector<std::string>& FuncKeys);//获得物理端口功能列表
	virtual bool GetFunctionsListOfTrip(const ::std::string& tripKey,std::vector<std::string>& FuncKeys);//获得压板对应的功能列表
	virtual ::std::string GetFunctionDesc(const ::std::string& functionKey);//获得功能的功能描述
	virtual ::std::string GetFunctionTripKey(const ::std::string& functionKey);//获得功能的功能压板（可能不存在）
	virtual ::std::string GetIEDTripKey(const ::std::string& IEDKey) ;//获得设备的压板（表示断电一类的操作）
	virtual ::std::string GetPortTripKey(const ::std::string& portKey) ;//获得端口的压板（表示拨光纤一类的操作）
	virtual ::std::string GetSignalTripKey(const ::std::string& signalKey);//获得信号的压板（软压板）


    //模型编辑接口
	virtual bool AddVoltageLevel(const ::std::string& voltageLevelName);//增加电压等级
	virtual bool DeleteVoltageLevel(const ::std::string& voltageLevelName);//删除电压等级
	virtual bool AddSubstation(const ::std::string& voltageLevelKey, const ::std::string& subName/*, const std::vector<char>& fileData*/);//增加变电站模型
	virtual int  GetSubstationAddedProgress();//获得当前导入进度
	virtual bool DeleteSubstation(const ::std::string& substationKey);//删除变电站模型
	virtual bool UpdateSubstationModel(const ::std::string& voltageLevelKey, const ::std::string& subName, const ::std::string& SCDFilePath);//更新变电站模型
	virtual bool AddPanelForSubstation(const ::std::string& subKey, const ::std::string& paneName);//为变电站增加屏柜
	virtual bool DeletePanelForSubstation(const ::std::string& PanelKey);//为变电站删除屏柜
	virtual bool AddIEDForPanel(const ::std::string& panelKey, const ::std::string& IEDName);//为屏柜增加装置
	virtual bool DeleteIEDForPanel(const ::std::string& IEDKey);//为屏柜删除装置
	virtual bool AddPortForIED(const ::std::string& IEDKey, const ::std::string& portName);//为装置增加物理端口
	virtual bool DeletePortForIED(const ::std::string& portKey);//为装置删除物理端口
	virtual bool AddInSignalForIED(const ::std::string& IEDKey, const ::std::string& inSignalName);//为装置增加输入信号
	virtual bool DeleteInSignalForIED(const ::std::string& inSignalKey);//为装置删除输入信号
	virtual bool AddOutSignalForIED(const ::std::string& IEDKey, const ::std::string& outSignalName); //为装置增加输出信号
	virtual bool DeleteOutSignalForIED(const ::std::string& outSignalKey);//为装置删除输出信号
	virtual bool AddFunction(const ::std::string& IEDKey, const ::std::string& funName, const ::std::string& funcDesc);//为装置增加功能
	virtual bool ModifyFunction(const ::std::string& funKey, const ::std::string& funcDesc);//为装置修改功能描述  
	virtual bool DeleteFunction(const ::std::string& funKey);//为装置删除功能 
	virtual bool SetDescForPort(const ::std::string& portKey, const ::std::string& desc);//设置物理端口的描述  
	virtual bool SetInSignalsListOfOutSignal(const ::std::string& outSignalKey, const std::vector<std::string>& inSignalKeys);//设置输出信号关联的输入信号列表（两个装置之间）
	virtual bool SetOutSignalsListOfInSignal(const ::std::string& inSignalKey, const std::vector<std::string>& outSignalKeys);//设置输入信号关联的输出信号列表（两个装置之间）
	virtual bool SetInternalOutSignalsListOfInSignal(const ::std::string& inSignalKey, const std::vector<std::string>& outSignalKeys);//设置输入信号关联的输出信号列表（装置内部）
	virtual bool SetInternalInSignalsListOfOutSignal(const ::std::string& outSignalKey, const std::vector<std::string>& inSignalKeys);//设置输出信号关联的输入信号列表（装置内部）
	virtual bool AddFunctionOfInSignal(const ::std::string& inSignalKey, const ::std::string& funKey);//为输入信号增加功能关联
	virtual bool DeleteFunctionOfInSignal(const ::std::string& inSignalKey,const std::vector<std::string>& funcKeys);//为输入信号删除功能关联 
	virtual bool AddTripForIED(const ::std::string& IEDKey, const ::std::string& tripName);//增加压板
	virtual bool DeleteTripForIED(const ::std::string& tripKey);//删除压板 
	virtual bool SetFunctionTripKey(const ::std::string& functionKey, const ::std::string& tripKey);//设置功能的功能压板（可能不存在） 
	virtual bool SetIEDTripKey(const ::std::string& IEDKey, const ::std::string& tripKey);//设置设备的压板（表示断电一类的操作） 
	virtual bool SetPortTripKey(const ::std::string& portKey, const ::std::string& tripKey);//设置端口的压板（表示拨光纤一类的操作）
	virtual bool SetSignalTripKey(const ::std::string& signalKey, const ::std::string& tripKey);//设置信号的压板（软压板）  

	virtual bool GetSoftTripState(const ::std::string& tripKey, bool& state);//获得压板状态
	virtual bool SetSoftTripState(const ::std::string& tripKey, bool state);//设置压板状态

	SCLModelServerImpl(void);
	~SCLModelServerImpl(void);
protected:
	bool        GetSignalKeyOfTrip(const ::std::string& tripKey,std::string &signalKey);

	void		OpenVoltageLevelLib();
	void		CloseVoltageLevelLib();

	void		OpenSubstationLib();
	void		CloseSubstationLib();

	void		OpenPanelLib();
	void		ClosePanelLib();

	void		OpenIedLib();
	void		CloseIedLib();

	void		OpenNetSwitchLib();
	void		CloseNetSwitchLib();

	void		OpenPhysicsPortLib();
	void		ClosePhysicsPortLib();

	void		OpenPhysicsLinkLib();
	void		ClosePhysicsLinkLib();

	void		OpenInSignalLib();
	void		CloseInSignalLib();

	void		OpenOutSignalLib();
	void		CloseOutSignalLib();

	void		OpenSoftTripLib();
	void		CloseSoftTripLib();

	void		OpenFunctionTripLib();
	void		CloseFunctionTripLib();

	void		OpenVirtualLinkLib();
	void		CloseVirtualLinkLib();

	void		OpenAllBdbLib();
	void		CloseAllBdbLib();

	//根据键值去相应实时库中取记录，输入为键值和库指针，输出为记录data
	// bool			GetRdbRecByKey(const ::std::string& keystring,Db *db,Dbt &rec);
	std::string			GetIedKeyFromSignal(const std::string& keystring);

	//根据OutSignal的键值取得所属IED名称:Vol.Sub.Panel.Ied.Port.OutSignal
	// SCDLoader scdLoader;
	// SCDUnLoader scdUnLoader;
	int  curProgress;
	// CRDBEnvironment *env;

private:
	bool IsRecv(const std::string& masterKey, const std::string& slaveKey);
};

