#ifndef  _SCL_MODEL_ADAPTER
#define  _SCL_MODEL_ADAPTER

/*
1、文件中包含了两个类的声明:SCLModelAdapterForClient、SclDacLib
2、SCLModelAdapterForClient类按照应用层的接口要求返回信息，将模型层的模型对象信息返回
3、SclDacLib类封装了对SCD导入文件和文件缓存的管理
*/

#ifdef _MSC_VER
    #ifdef SCLDATAACCESSCOMPONENT_EXPORTS
        #define SCLDATAACCESSCOMPONENT_API __declspec(dllexport)
    #else
        #define SCLDATAACCESSCOMPONENT_API __declspec(dllimport)
    #endif
#else
    #define SCLDATAACCESSCOMPONENT_API
#endif

#include "SCLObjectInfoDefineByClient.h"
#include <string>
#include <vector>

class SCLDATAACCESSCOMPONENT_API SCLModelAdapterForClient
{
public:
	//模型通用接口
	std::string GetSubstationImportTime(const std::string& subStationKey);

    //模型基本访问接口
	virtual bool GetVoltageLevelList(std::vector<std::string>& nameList);   //获得电压等级名列表（同Key）
	virtual bool GetSubstationList(const std::string& voltageLevelKey, std::vector<std::string>& subStationKeyList);//获得变电站Key列表
	virtual bool GetSubstationInfo(const std::string& subStationKey, SubStationInfo& subStationInfo);//获得变电站（特定SCD版本）信息
	virtual std::string GetSubstationPrivateXMLInfo(const std::string& subStationKey);//获得变电站private标签XML原文
	virtual std::string GetSubstationHeaderXMLInfo(const std::string& subStationKey);//获得变电站Header标签XML原文
	virtual bool GetSCDHItem(const std::string& subStationKey, std::vector<HItemInfo>& hItemList);//获得SCD历史修订记录
	virtual bool GetSubNetList(const std::string& subStationKey, std::vector<std::string>& subNetKeyList);//获得子网Key列表
	virtual bool GetSubNetInfo(const std::string& subNetKey, SubNetInfo& subNetInfo);//获得子网信息
	virtual std::string GetSubNetXMLInfo(const ::std::string& subNetKey);//获得子网XML片段
	virtual bool GetApInfo(const std::string& subStationKey, const std::string& subNetName,const std::string& IEDName, const std::string APName, APInfo& apInfo);//获得访问点信息
	virtual bool GetPanelList(const std::string& subStationKey,std::vector<std::string>& subPanelKeyList);//获得屏柜Key列表
	virtual bool GetIEDList(const std::string& panelKey,std::vector<std::string>& subIEDKeyList);//获得装置Key列表
	virtual bool GetIEDListBySubNet(const std::string& subNetKey, std::vector<std::string>& subIEDKeyList);//获得装置Key列表(根据子网)
	virtual bool GetIEDInfo(const std::string& IEDKey, DEVInfo& IEDInfo);//获得装置信息	
	virtual std::string GetIEDXMLInfo(const ::std::string& IEDKey);//获得IED的XML信息

	virtual bool GetIEDType(const std::string& IEDKey,  int& type);//获得装置类型
	virtual std::string GetSCDVersion(const std::string& subStationKey);//获得当前模型的SCD文件版本号
	virtual bool IsVoltageLevelExist(const std::string& voltageLevelName);//是否存在电压等级
	virtual bool IsSubstationExist(const std::string& substationKey);//是否存在变电站
	virtual bool GetPortsList(const std::string& IEDKey,std::vector<std::string>& portKeys);//获得装置的物理端口列表
	virtual std::string GetPortDescByPortKey(const std::string& portKey);//获得物理端口的描述
	virtual bool GetLinksListOfIED(const std::string& IEDKey, std::vector<PhysLinkInfo>& links);//获得装置的物理链接列表
	virtual bool GetInSignalsListOfIED(const std::string& IEDKey, SignalTypeEnum signalType,std::vector<std::string>& inSignalKeys);//获得装置的输入信号列表
	virtual bool GetOutSignalsListOfIED(const std::string& IEDKey,  SignalTypeEnum signalType,std::vector<std::string>& outSignalKeys);//获得装置的输出信号列表
	virtual bool GetVritualLinksOfIED(const std::string& IEDKey,  SignalTypeEnum signalType, std::vector<VirtualLinkInfo>& links);//获得装置的虚拟链接列表（装置和装置之间）
	virtual bool GetInternalVritualLinksOfIED(const std::string& IEDKey,  SignalTypeEnum signalType, std::vector<VirtualLinkInfo>& links);//获得装置内部的虚拟链接列表（装置内部）
	virtual bool GetFunctionsList(const std::string& IEDKey,std::vector<std::string>& protectFuncKeys);//获得装置的功能列表
	virtual bool GetVirtualLinksListOfPhysicsLink(const PhysLinkInfo& phyLink,  SignalTypeEnum signalType, std::vector<VirtualLinkInfo>& links);//获得物理链接关联的虚拟链接列表
	virtual bool GetTripsList(const std::string& IEDKey,std::vector<std::string>& tripKeys);//获得装置的压板列表  
	virtual bool GetInfoOfInSignal(const std::string& signalKey, SignalInfo& signalInfo);//获得输入信号的信息  
	virtual bool GetInfoOfOutSignal(const std::string& signalKey, SignalInfo& signalInfo);//获得输出信号的信息
	virtual bool GetInfoOfDSSettingOutSignal(const std::string& signalKey, DSSettingSignalInfo& signalInfo);

	virtual bool GetCtrlBlockListByIED(const std::string& IEDKey, std::vector<std::string>& ctrlBlockKeys);//获取当前IED的数据集列表
	virtual bool GetCtrlBlockInfo(const std::string& ctrlBlockKey, CtrlBlockInfo& cbInfo);//获取数据集信息
	virtual bool GetDataSetSignalsByCtrlBlockID(const std::string& cbKey, std::vector<std::string>& dsSignalList);//获取数据集虚信号
	virtual bool GetSlaveCtrlBlockByIEDKeys(const std::string& masterKey, const std::string& slaveKey, std::vector<std::string>& ctrlBlockList);//根据主从IED的Key获取从IED用到的控制块
	virtual bool GetOutSignalsByCBKey(const std::string& masterIEDKey, const std::string& cbKey, std::vector<std::string>& outSignals);		//通过控制块获取虚链接;
	virtual bool GetAssoIEDListByCBKey(const std::string& cbKey, std::vector<std::string>& assoIEDKeys);		//通过控制块获取对端IED;

	virtual bool GetPhyPortInfo(const std::string& pPortKey, PhyPortInfo& pPortInfo);				//获取物理端口信息;
	virtual bool GetPhyPortListByIED(const std::string& IEDKey, std::vector<std::string>& pPortList);		//根据IED获取物理端口;
	virtual bool GetPhyPortListByInSignalID(const std::string& signalKey, std::vector<std::string>& pPortList);			//根据信号获取端口;
	virtual bool GetPhyPortKeyByOutSignalID(const std::string& signalKey, std::string& pPortKey);			//根据信号获取端口;

	//高级访问接口
	virtual bool GetIEDListByLogicalRealitionWithCurrentIED(const std::string& IEDKey,std::vector<std::string>& associatedIEDKeys);//获得IED的关联IED列表（逻辑）
	virtual bool GetRealitionTypeOfSrcIEDAndDestIED(const std::string& srcIEDKey, const std::string& destIEDKey, int& type);//获得IED之间的关联类型（0：源IED为输入）																																												//1: 源IED为输出																																											//2: 源IED既是输入又是输																																												//-1:源IED和目的IED没有关系）
	virtual bool GetIEDListByPhysicalRealitionWithCurrentIED(const std::string& IEDKey,std::vector<std::string>& associatedIEDKeys);//获得IED的关联IED列表（物理）  
	virtual bool GetPhysicsLinkAssociatedIED(const std::string& srcIEDKey, const std::string& destIEDKey, std::vector<PhysLinkInfo>& links);//获得IED与IED之间物理链接（多个链接表示多个物理通道，0个链接表示无直接连接）
	virtual bool GetLinksBetweenPort(const std::string& srcPortKey, const std::string& destPortKey, std::vector<PhysLinkInfo>& links);//获得原port与目的port之间的物理链接（多个链接表示之间有多个设备）
	virtual bool GetVirtualSignalsAssociatedIED(const std::string& srcIEDKey, const std::string& destIEDKey,  SignalTypeEnum signalType, std::vector<VirtualLinkInfo>& links);//获得IED与IED之间虚信号 
	virtual bool GetPortsListOfPort(const std::string& portKey,std::vector<std::string>& portKeys);//获得与物理端口关联的其它物理端口
	virtual bool GetPhysicsLinkeByPort(const std::string& srcPortKey, const std::string& destPortKey, PhysLinkInfo& link);//根据两个端口获得物理链接 
	virtual bool GetInSignalsListOfOutSignal(const std::string& outSignalKey,std::vector<std::string>& inSignalKeys);//获得输出信号关联的输入信号列表（两个装置之间）
	virtual bool GetOutSignalsListOfInSignal(const std::string& inSignalKey,std::vector<std::string>& outSignalKeys);//获得输入信号关联的输出信号列表（两个装置之间）
	virtual bool GetInternalOutSignalsListOfInSignal(const std::string& inSignalKey,std::vector<std::string>& outSignalKeys);//获得输入信号关联的输出信号列表（装置内部）
	virtual bool GetInternalInSignalsListOfOutSignal(const std::string& outSignalKey,std::vector<std::string>& inSignalKeys);//获得输出信号关联的输入信号列表（装置内部）
	virtual bool GetVirtualLinkBySignal(const std::string& srcSignalKey, const std::string& destSignalKey, VirtualLinkInfo& link);//根据两个信号获得虚拟链接 
	virtual bool GetFunctionListOfInSignal(const std::string& inSignalKey,std::vector<std::string>& funcKeys);//获得输入信号关联的功能列表（本装置功能）
	virtual bool GetFunctionsListOfOutSignal(const std::string& outSignalKey,std::vector<std::string>& funcKeys);//获得输出信号关联的功能列表 （其它装置功能）
	virtual bool GetFunctionsListOfOutSignalWithDestIED(const std::string& outSignalKey, const std::string& destIEDKey,std::vector<std::string>& funcKeys);//获得输出信号相对于某装置的功能列表（某装置功能）
	virtual bool GetFunctionsListOfPort(const std::string& portKey,std::vector<std::string>& FuncKeys);//获得物理端口功能列表
	virtual bool GetFunctionsListOfTrip(const std::string& tripKey,std::vector<std::string>& FuncKeys);//获得压板对应的功能列表
	virtual std::string GetFunctionDesc(const std::string& functionKey);//获得功能的功能描述
	virtual std::string GetFunctionTripKey(const std::string& functionKey);//获得功能的功能压板（可能不存在）
	virtual std::string GetIEDTripKey(const std::string& IEDKey) ;//获得设备的压板（表示断电一类的操作）
	virtual std::string GetPortTripKey(const std::string& portKey) ;//获得端口的压板（表示拨光纤一类的操作）
	virtual std::string GetSignalTripKey(const std::string& signalKey);//获得信号的压板（软压板）

    //模型编辑接口
	virtual bool AddVoltageLevel(const std::string& voltageLevelName);//增加电压等级
	virtual bool DeleteVoltageLevel(const std::string& voltageLevelName);//删除电压等级
	virtual bool AddSubstation(const std::string& voltageLevelKey, const std::string& subName/*, const std::vector<char>& fileData*/);//增加变电站模型
	virtual int  GetSubstationAddedProgress();//获得当前导入进度
	virtual bool DeleteSubstation(const std::string& substationKey);//删除变电站模型
	virtual bool UpdateSubstationModel(const std::string& voltageLevelKey, const std::string& subName, const std::string& SCDFilePath);//更新变电站模型
	virtual bool AddPanelForSubstation(const std::string& subKey, const std::string& paneName);//为变电站增加屏柜
	virtual bool DeletePanelForSubstation(const std::string& PanelKey);//为变电站删除屏柜
	virtual bool AddIEDForPanel(const std::string& panelKey, const std::string& IEDName);//为屏柜增加装置
	virtual bool DeleteIEDForPanel(const std::string& IEDKey);//为屏柜删除装置
	virtual bool AddPortForIED(const std::string& IEDKey, const std::string& portName);//为装置增加物理端口
	virtual bool DeletePortForIED(const std::string& portKey);//为装置删除物理端口
	virtual bool AddInSignalForIED(const std::string& IEDKey, const std::string& inSignalName);//为装置增加输入信号
	virtual bool DeleteInSignalForIED(const std::string& inSignalKey);//为装置删除输入信号
	virtual bool AddOutSignalForIED(const std::string& IEDKey, const std::string& outSignalName); //为装置增加输出信号
	virtual bool DeleteOutSignalForIED(const std::string& outSignalKey);//为装置删除输出信号
	virtual bool AddFunction(const std::string& IEDKey, const std::string& funName, const std::string& funcDesc);//为装置增加功能
	virtual bool ModifyFunction(const std::string& funKey, const std::string& funcDesc);//为装置修改功能描述  
	virtual bool DeleteFunction(const std::string& funKey);//为装置删除功能 
	virtual bool SetDescForPort(const std::string& portKey, const std::string& desc);//设置物理端口的描述  
	virtual bool SetInSignalsListOfOutSignal(const std::string& outSignalKey, const std::vector<std::string>& inSignalKeys);//设置输出信号关联的输入信号列表（两个装置之间）
	virtual bool SetOutSignalsListOfInSignal(const std::string& inSignalKey, const std::vector<std::string>& outSignalKeys);//设置输入信号关联的输出信号列表（两个装置之间）
	virtual bool SetInternalOutSignalsListOfInSignal(const std::string& inSignalKey, const std::vector<std::string>& outSignalKeys);//设置输入信号关联的输出信号列表（装置内部）
	virtual bool SetInternalInSignalsListOfOutSignal(const std::string& outSignalKey, const std::vector<std::string>& inSignalKeys);//设置输出信号关联的输入信号列表（装置内部）
	virtual bool AddFunctionOfInSignal(const std::string& inSignalKey, const std::string& funKey);//为输入信号增加功能关联
	virtual bool DeleteFunctionOfInSignal(const std::string& inSignalKey,const std::vector<std::string>& funcKeys);//为输入信号删除功能关联 
	virtual bool AddTripForIED(const std::string& IEDKey, const std::string& tripName);//增加压板
	virtual bool DeleteTripForIED(const std::string& tripKey);//删除压板 
	virtual bool SetFunctionTripKey(const std::string& functionKey, const std::string& tripKey);//设置功能的功能压板（可能不存在） 
	virtual bool SetIEDTripKey(const std::string& IEDKey, const std::string& tripKey);//设置设备的压板（表示断电一类的操作） 
	virtual bool SetPortTripKey(const std::string& portKey, const std::string& tripKey);//设置端口的压板（表示拨光纤一类的操作）
	virtual bool SetSignalTripKey(const std::string& signalKey, const std::string& tripKey);//设置信号的压板（软压板）  

	virtual bool GetSoftTripState(const std::string& tripKey, bool& state);//获得压板状态
	virtual bool SetSoftTripState(const std::string& tripKey, bool state);//设置压板状态

	// 取得子网下IED的AP列表
	virtual bool GetAPListOfIEDFromSubnet(const ::std::string& subStationID, const ::std::string& subNetName, const ::std::string IEDName, std::vector<std::string> &APList);
	// 取得子网下IED某个AP包含的物理端口列表
	virtual bool GetPortListOfIEDAPFromSubNet(const ::std::string& subStationID, const ::std::string& subNetName, const ::std::string IEDName, const ::std::string APName, std::vector<std::string> &PortList);
	// 取得IED下AP的列表
	virtual bool GetAPListFromIED(const ::std::string& subStationID, const ::std::string& IEDName, std::vector<std::string> &APList);
	// 取得IED下某个AP对应的物理端口列表
	virtual bool GetAPPortListFormIED(const ::std::string& subStationID, const ::std::string& IEDName, const ::std::string &APName, std::vector<std::string> &PortList);


	virtual bool GetProtectionSettingList(const ::std::string& masterIEDKey, std::vector<DataSetInfo>& dsSettingList);
	// 取得IED下的定值条目列表
	virtual bool GetProtectionSettingSignalList(const ::std::string& masterIEDKey, const ::std::string& DataSetName, ::std::vector<std::string>& outSignalKeys);
	// 取得定值条目所对应的列表

    virtual bool GetAllRemainingDataSetSignalList(const ::std::string& masterIEDKey, const ::std::string& ldInst, 
        const ::std::string& DataSetName, ::std::vector<std::string>& outSignalKeys);
    virtual bool GetAllRemainingDataSetList(const ::std::string& masterIEDKey, 
        std::vector<DataSetInfo>& dsSettingList);

    virtual bool GetDataTypeTemplateInfo(const ::std::string &subStationKey, DataTypeTemplateType& dtt);
    virtual bool GetIEDListByLNodeType(const ::std::string &subStationKey, const ::std::string &lnID, ::std::vector<std::string> &assoIEDList);

    virtual bool GetSSDInfo(const ::std::string &subStationKey, SubstationCache &ssdInfo);
};

// 库通用操作类，陆腾
class SCLDATAACCESSCOMPONENT_API SclDacLib
{
public:
    // 全局初始化函数
    // 如果传入空字符串，则使用默认路径（加载dll的exe所在路径）
    // 务必确认路径存在，否则可能无法被访问到
    static bool Startup(const std::string &strGlobalWorkPath = "");

    // 获取工作目录，自动附带结尾的路径分隔符
    // lpFileMark为NULL或空白字符串时，返回全局目录
    // lpFileMark为具体文件标识时，返回文件标识目录
    // 不自动创建目录
    // 若目录不存在，返回空白字符串
    static std::string GetWorkPath(const char *lpFileMark);

    // 全局销毁函数
    static void Shutdown();

    // 导入新scd文件，内部可能调用LoadScdFile来处理
    static bool ImportScdFile(const std::string &strFilePath, std::string &strFileMark);

    // 加载一个已导入过的scd文件
    static bool LoadScdFile(const std::string &strFileMark);

    // 从内存卸载一个已加载的scd文件
    static bool UnloadScdFile(const std::string &strFileMark);

    // 从外存删除一个已导入的scd文件，会自动调用UnloadScdFile
    static bool RemoveSavedData(const std::string &strFileMark);

    // 获取已导入的scd文件标识列表
    static std::vector<std::string> GetImportedFileMarks();

    // 获取已加载的scd文件标识列表
    static std::vector<std::string> GetLoadedFileMarks();

    // 从scd文件计算文件标识
    static std::string CalcFileMark(const std::string &strFilePath);

    // 将已导入的scd文件内容输出到sqlite
    static bool ExportScdFileInfomationToSqliteFile(const std::string &strFileMark, const std::string &strFilePath);

    // 触发底层开始对虚信号的缓存执行，并等待结束
    static bool BuildSignalCache(const std::string &strFileMark);
};

#endif