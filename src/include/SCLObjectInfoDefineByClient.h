#ifndef SCL_OBJECT_INFO_DEFINE_BY_CLIENT_H
#define SCL_OBJECT_INFO_DEFINE_BY_CLIENT_H

/*
1、本文件定义了应用层模型中的对象结构；
2、本文件被SCLModelAdapterForClient适配器使用，为应用层提供需要的对象结果；
*/

#include <string>
#include <vector>
#include <map>


enum SignalTypeEnum
{
	typeAll,
	typeGoose,
	typeSV,
	typeRpt, // 信息点表
	typeSetting // 定值条目
};

struct PhysLinkInfo
{
	std::string srcPort;
	std::string destPort;
};

struct VirtualLinkInfo
{
	int iOrd;
	std::string srcSignal;
	std::string destSignal;
	std::string inSigDesc;
};

struct SubStationInfo
{
	std::string Name;
	std::string crc;
	std::string desc;     
	std::string version;
	std::string reversion;
	std::string fileName;
	::std::string filePath;//新增---已有
	::std::string toolID;//新增---head下toolID
};

struct SubNetInfo
{
	std::string name;
	std::string type;
	std::string desc;      
};

struct HItemInfo
{
	std::string substationName;
	std::string version;
	std::string reversion;
	std::string who;
	std::string when;
	std::string what;
	std::string why;        
};

struct DEVInfo
{
	std::string name;
	std::string crc;
	std::string desc;      
	std::string type;
	std::string manu;
	std::string funVer;
	std::string configVer;  
	int GooseInSigCount;//新增 ----goose输入信号
	int GooseOutSigCount;//新增----goose输出信号
	int SVInSigCount;//新增-----sv输入信号
	int SVOutSigCount;//新增----sv输出信号
	int ReportCBCount; //新增----ReportCB数量
	int DsSettingCount; // 新增---定值数据集数目
};

// 定值模块的信息
struct DataSetInfo
{
	std::string  IEDName;
	std::string  ApName;
	std::string  LDInst;
	std::string  dataSetName;
	std::string  dataSetDesc;
    int iOutSignalCount;
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

	::std::string iedName;//新增

	::std::vector<::std::string>  portNames; // zxc 12.03

	::std::string BDAType;
	::std::string plugType;
	::std::string LnName;//新增
	::std::string LnDesc;
	::std::string DOIDesc;
	::std::string dUVal;
	::std::string ctrlBlockID;
};

// 定值条目信息
struct DSSettingSignalInfo
{
	::std::string cdc;
	::std::string minVal;
	::std::string maxVal;
	::std::string stepSize;
	::std::string unit;
};

struct APInfo
{
	std::string name;
	std::string desc;
	std::string IEDName;
	std::string subNetname;
	std::string ipAddr;
	std::string macAddr;   

	std::vector<std::string> phyPorts; //zxc 12.03 
};

// report控制块的TrgOps
struct CtrlBlockTrgOps
{
    std::string dchg;
    std::string qchg;
    std::string dupd;
    std::string period;
};
// report控制块的OptFields
struct CtrlBlockOptFields
{
    std::string bufOvfl;
    std::string configRef;
    std::string dataRef;
    std::string dataSet;
    std::string entryID;
    std::string reasonCode;
    std::string segmentation;
    std::string seqNum;
    std::string timeStamp;
};
// report控制块的RptEnabled
struct CtrlBlockRptEnabled
{
    std::string max;
    std::string desc;
};

struct CtrlBlockInfo 
{
	std::string name;
	std::string type;
	std::string APPID;
	std::string apName;
	std::string apMAC;
	std::string apVLAN_ID;
	std::string apVLAN_PRIORITY;
	std::string apAPPID;
	std::string dataSetName;
	std::string dataSetAddr;

	std::string confRev;
	int ASDU;
	std::string smpRate;
	std::string gocbRef;

	std::string maxTime;
	std::string minTime;

	// add by zxc for ReportCB
	std::string LDInst;
	std::string desc;
	std::string intgPd; // report控制块完整性周期
	std::string Buffered; // 规定报告是否缓存
	std::string bufTime;  // 缓存时间

    // add by zxc 
    std::string nofASDU; // sv控制块属性
    CtrlBlockTrgOps trgOps;
    CtrlBlockOptFields OptFields;
    CtrlBlockRptEnabled RptEnabled;
    std::string logName;
    std::string logEna;
    std::string reasonCode;
    std::string cbtype; // goose控制块type属性
};

// zxc 修改 12.03
struct PhyPortInfo  
{
	::std::string IEDName;    
	
	::std::string portType; // PhysConn type="Connection/RedConn"
	::std::string portName; 
	::std::string plug;   
	::std::string transMedia;   
	::std::string cable; 
};

// 数据模板定义
struct DACache 
{
    unsigned int ord;
    ::std::string name;
    ::std::string bType;
    ::std::string type;
    ::std::string sAddr;
    ::std::string valKind;
    ::std::string count;
    ::std::string dchg;
    ::std::string fc;
    ::std::string qchg;
    ::std::string dupd;
};

typedef std::map<::std::string, DACache> DAMap;

struct DOCache 
{
    unsigned int ord;
    ::std::string name;
    ::std::string type;
    ::std::string transient;
    ::std::string accessControl;
};

typedef std::map<::std::string, DOCache> DOMap;

// 记录xml元素顺序的数据类型(name 作为标示)
typedef std::vector<::std::string> Order;

struct DOTypeCache
{
    ::std::string id;
    ::std::string cdc;
    ::std::string iedType;

    DOMap mapSDO;
    DAMap mapDaType;
};

typedef std::map<::std::string, DOTypeCache> DOTypeMap;

typedef std::map<::std::string, DACache> BDAMap;

struct DATypeCache
{
    ::std::string id;
    ::std::string iedType;

    BDAMap mapBDA;
};

typedef std::map<::std::string, DATypeCache> DATypeMap;

struct LNodeTypeCache 
{
    ::std::string id;
    ::std::string lnClass;
    ::std::string iedType;

    DOMap mapDO;
};

typedef std::map<::std::string, LNodeTypeCache> LNodeTypeMap;

struct DataTypeTemplateType
{
    LNodeTypeMap mapLN;
    DOTypeMap mapDO;
    DATypeMap mapDA;
};

// -----------------------------------------------------------------------------SSD结构---------------------------------------------------------------
struct TerminalCache{
    ::std::string key;

    ::std::string name;
    ::std::string desc;
    ::std::string connectivityNode;
    ::std::string substationName;
    ::std::string voltageLevelName;
    ::std::string bayName;
    ::std::string cnodeName;
};
typedef std::map<::std::string, TerminalCache> TerminalCacheMap;

struct LNodeCache{
    ::std::string key;

    ::std::string lnInst;
    ::std::string lnClass;
    ::std::string iedName;
    ::std::string ldInst;
    ::std::string prefix;
    ::std::string lnType;
};
typedef std::map<::std::string, LNodeCache> LNodeCacheMap;

struct ConnectivityNodeCache{
    ::std::string key;

    ::std::string name;
    ::std::string desc;
    ::std::string pathname;
};
typedef std::map<::std::string, ConnectivityNodeCache> ConnectivityNodeCacheMap;

//struct CommonEquipmentCache{
//    ::std::string name;
//    ::std::string desc;
//    ::std::string vir;
//    ::std::string type;
//};
//typedef std::map<::std::string, CommonEquipmentCache> CommonEquipmentCacheMap;

//// 子功能
//struct SubFunctionCache{
//    ::std::string name;
//    ::std::string desc;
//
//    LNodeCacheMap lnMap;
//    CommonEquipmentCacheMap generalEquipmentMap;
//};

//// 功能
//struct FunctionCache{
//    ::std::string name;
//    ::std::string desc;
//
//    LNodeCacheMap lnMap;
//    SubFunctionCache subFuncMap;
//    CommonEquipmentCacheMap generalEquipmentMap;
//};
//typedef std::map<::std::string, FunctionCache> FunctionCacheMap;

// 导电设备
struct ConductingEquipmentCache{
    ::std::string key;

    ::std::string name;
    ::std::string desc;
    ::std::string vir;
    ::std::string type;

    TerminalCacheMap terminalMap;
    LNodeCacheMap lnMap;
    // CommonEquipmentCacheMap subEquipmentMap;
};
typedef std::map<::std::string, ConductingEquipmentCache> ConductingEquipmentCacheMap;

// 变压器绕组
struct TransformerWindingCache{
    ::std::string key;

    ::std::string name;
    ::std::string desc;
    ::std::string vir;
    ::std::string type;

    TerminalCacheMap terminalMap;
    //CommonEquipmentCacheMap subEquipmentMap;
    //CommonEquipmentCacheMap tapChangerMap;
    LNodeCacheMap lnMap;
};
typedef std::map<::std::string, TransformerWindingCache> TransformerWindingCacheMap;
// 变压器设备
struct PowerTransformerCache{
    ::std::string key;

    ::std::string name;
    ::std::string desc;
    ::std::string vir;
    ::std::string type;

    TransformerWindingCacheMap tfWindingMap; // 绕组
    LNodeCacheMap lnodeMap; 
};
typedef std::map<::std::string, PowerTransformerCache> PowerTransformerCacheMap;

struct BayCache{
    ::std::string key;

    ::std::string name;
    ::std::string desc;
    ::std::string dtype; // 间隔类型

    PowerTransformerCacheMap ptMap; //变压器列表
    ConductingEquipmentCacheMap ceMap; //导电设备列表
    ConnectivityNodeCacheMap cnMap; // 导电节点；
    LNodeCacheMap lnMap; // LNode 列表
    // FunctionCacheMap funcMap; // 虚拟间隔中的内容
};
typedef std::map<::std::string, BayCache> BayCacheMap;

struct VoltageCache{
    ::std::string multiplier;
    ::std::string unit;
    ::std::string text;
};

// 电压等级
struct VoltageLevelCache{
    ::std::string key;

    ::std::string name;
    ::std::string desc;

    VoltageCache voltage;
    BayCacheMap bayMap;
};
typedef std::vector<VoltageLevelCache> VoltageLevelVector;

// Substation 结构
struct SubstationCache{
    ::std::string key;

    ::std::string name;
    ::std::string desc;

    // private 节点
    ::std::string dtype; // 变电站类型 CIME-type
    ::std::string areaName; // 变电站区域缩写 CIME-area
    ::std::string areaDesc; // 变电站区域描述 CIME-area

    VoltageLevelVector vVoltageLevel; // 此处用vector存储 便于后期比较顺序
};


#endif


