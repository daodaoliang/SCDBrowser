#ifndef SCL_OBJECT_INFO_DEFINE_BY_CLIENT_H
#define SCL_OBJECT_INFO_DEFINE_BY_CLIENT_H

/*
1�����ļ�������Ӧ�ò�ģ���еĶ���ṹ��
2�����ļ���SCLModelAdapterForClient������ʹ�ã�ΪӦ�ò��ṩ��Ҫ�Ķ�������
*/

#include <string>
#include <vector>
#include <map>


enum SignalTypeEnum
{
	typeAll,
	typeGoose,
	typeSV,
	typeRpt, // ��Ϣ���
	typeSetting // ��ֵ��Ŀ
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
	::std::string filePath;//����---����
	::std::string toolID;//����---head��toolID
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
	int GooseInSigCount;//���� ----goose�����ź�
	int GooseOutSigCount;//����----goose����ź�
	int SVInSigCount;//����-----sv�����ź�
	int SVOutSigCount;//����----sv����ź�
	int ReportCBCount; //����----ReportCB����
	int DsSettingCount; // ����---��ֵ���ݼ���Ŀ
};

// ��ֵģ�����Ϣ
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

	::std::string iedName;//����

	::std::vector<::std::string>  portNames; // zxc 12.03

	::std::string BDAType;
	::std::string plugType;
	::std::string LnName;//����
	::std::string LnDesc;
	::std::string DOIDesc;
	::std::string dUVal;
	::std::string ctrlBlockID;
};

// ��ֵ��Ŀ��Ϣ
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

// report���ƿ��TrgOps
struct CtrlBlockTrgOps
{
    std::string dchg;
    std::string qchg;
    std::string dupd;
    std::string period;
};
// report���ƿ��OptFields
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
// report���ƿ��RptEnabled
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
	std::string intgPd; // report���ƿ�����������
	std::string Buffered; // �涨�����Ƿ񻺴�
	std::string bufTime;  // ����ʱ��

    // add by zxc 
    std::string nofASDU; // sv���ƿ�����
    CtrlBlockTrgOps trgOps;
    CtrlBlockOptFields OptFields;
    CtrlBlockRptEnabled RptEnabled;
    std::string logName;
    std::string logEna;
    std::string reasonCode;
    std::string cbtype; // goose���ƿ�type����
};

// zxc �޸� 12.03
struct PhyPortInfo  
{
	::std::string IEDName;    
	
	::std::string portType; // PhysConn type="Connection/RedConn"
	::std::string portName; 
	::std::string plug;   
	::std::string transMedia;   
	::std::string cable; 
};

// ����ģ�嶨��
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

// ��¼xmlԪ��˳�����������(name ��Ϊ��ʾ)
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

// -----------------------------------------------------------------------------SSD�ṹ---------------------------------------------------------------
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

//// �ӹ���
//struct SubFunctionCache{
//    ::std::string name;
//    ::std::string desc;
//
//    LNodeCacheMap lnMap;
//    CommonEquipmentCacheMap generalEquipmentMap;
//};

//// ����
//struct FunctionCache{
//    ::std::string name;
//    ::std::string desc;
//
//    LNodeCacheMap lnMap;
//    SubFunctionCache subFuncMap;
//    CommonEquipmentCacheMap generalEquipmentMap;
//};
//typedef std::map<::std::string, FunctionCache> FunctionCacheMap;

// �����豸
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

// ��ѹ������
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
// ��ѹ���豸
struct PowerTransformerCache{
    ::std::string key;

    ::std::string name;
    ::std::string desc;
    ::std::string vir;
    ::std::string type;

    TransformerWindingCacheMap tfWindingMap; // ����
    LNodeCacheMap lnodeMap; 
};
typedef std::map<::std::string, PowerTransformerCache> PowerTransformerCacheMap;

struct BayCache{
    ::std::string key;

    ::std::string name;
    ::std::string desc;
    ::std::string dtype; // �������

    PowerTransformerCacheMap ptMap; //��ѹ���б�
    ConductingEquipmentCacheMap ceMap; //�����豸�б�
    ConnectivityNodeCacheMap cnMap; // ����ڵ㣻
    LNodeCacheMap lnMap; // LNode �б�
    // FunctionCacheMap funcMap; // �������е�����
};
typedef std::map<::std::string, BayCache> BayCacheMap;

struct VoltageCache{
    ::std::string multiplier;
    ::std::string unit;
    ::std::string text;
};

// ��ѹ�ȼ�
struct VoltageLevelCache{
    ::std::string key;

    ::std::string name;
    ::std::string desc;

    VoltageCache voltage;
    BayCacheMap bayMap;
};
typedef std::vector<VoltageLevelCache> VoltageLevelVector;

// Substation �ṹ
struct SubstationCache{
    ::std::string key;

    ::std::string name;
    ::std::string desc;

    // private �ڵ�
    ::std::string dtype; // ���վ���� CIME-type
    ::std::string areaName; // ���վ������д CIME-area
    ::std::string areaDesc; // ���վ�������� CIME-area

    VoltageLevelVector vVoltageLevel; // �˴���vector�洢 ���ں��ڱȽ�˳��
};


#endif


