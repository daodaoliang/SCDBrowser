#ifndef SliceSCLModelSvrInterface_H
#define SliceSCLModelSvrInterface_H

#include <QString>
#include <QVector>
#include <QMap>
#include "SCLObjectInfoDefineByClient.h"

struct QPhysLink
{
    QString srcPort;
    QString destPort;
};

// 定值条目数据集信息
struct QDataSetInfo
{
	QString  IEDName;
	QString  ApName;
	QString  LDInst;
	QString  dataSetName;
	QString  dataSetDesc;
};

struct QVirtualLink
{
	int iOrd;
    QString srcSignal;
    QString destSignal;
	QString inSigInfo;	//增加输入信号描述;

	QString diffDesc; // 增加差异化描述，用于点击图形显示差异信息;

	int DiffState;	//差异类型;
};

enum QSignalTypeEnum {
	typeAll_,
	typeGoose_,
	typeSV_
};

struct QSignalInfo
{
	int		iOrd;
	int		iOutSigOrd;
	QString ID;
	QString name;
	QString path;
	QString desc;
	QString iedName;
	QVector<QString> portIndexList;
	QSignalTypeEnum type;

	QString dataSet;
	QString fc;
	int state;
	QString softTripKey;

	QString BDAType;
	QString plugType;
	QString LnName;
	QString LnDesc;
	QString DOIDesc;
	QString dUVal;

	QString ctrlBlockID;

	QString diffDesc; // 增加差异化描述，用于点击图形显示差异信息

	int DiffState;	//差异类型;
};

struct QDSSettingSignalInfo
{
	QString cdc;
	QString minVal;
	QString maxVal;
	QString stepSize;
	QString unit;
};

struct QSubStationInfo
{
	QString Name;
	QString crc;
	QString desc;     
	QString version;
	QString reversion;
	QString fileName;
	QString filePath;
	QString toolID;
};

struct QSubNetInfo
{
	QString name;
	QString type;
	QString desc;      
};

struct QHItemInfo
{
	QString substationName;
	QString version;
	QString reversion;
	QString who;
	QString when;
	QString what;
	QString why;
};

struct QIEDInfo
{
	QString name;
	QString crc;
	QString desc;      
	QString type;
	QString manu;
	QString funVer;
	QString configVer;

	int GooseInSigCount;
	int GooseOutSigCount;
	int SVInSigCount;
	int SVOutSigCount;
	int ReportCBCount;
	int DsSettingCount;

	QString diffDesc; // 增加差异化描述，用于点击图形显示差异信息

	int DiffState;	//差异类型;
};

struct QAPInfo
{
	QString name;
	QString desc;
	QString IEDName;
	QString subNetname;
	QString ipAddr;
	QString macAddr;
};

struct QCtrlBlockInfo
{
	QString name;
	QString type;
	QString APPID;
	QString apName;
	QString apMAC;
	QString apVLAN_ID;
	QString apVLAN_PRIORITY;
	QString apAPPID;
	QString dataSetName;
	QString dataSetAddr;

	QString confRev;
	int ASDU;
	QString smpRate;
	QString gocbRef;

	QString maxTime;
	QString minTime;

	// add by zxc for ReportCB 2014.11.25
	QString LDInst;
	QString desc;
	QString intgPd; // report控制块完整性周期
	QString Buffered; // 规定报告是否缓存
	QString bufTime;  // 缓存时间


	QString diffDesc; // 增加差异化描述，用于点击图形显示差异信息
	
	int DiffState;	//差异类型;
};

struct QPhyPortInfo 
{
	//QString	desc;
	QString	ID;
	QString IEDName;
	QString	pIndex;
	QString	plug;
	QString	type;
	QString transMedia;
	QString cable;
	//QString	rtType;

	QString diffDesc; // 增加差异化描述，用于点击图形显示差异信息

	int DiffState;	//差异类型;
};

// -----------------------------------------------------------------------------SSD结构---------------------------------------------------------------
struct QTerminalCache{
    QString key;

    QString name;
    QString desc;
    QString connectivityNode;
    QString substationName;
    QString voltageLevelName;
    QString bayName;
    QString cnodeName;
};
typedef QMap<QString, QTerminalCache> QTerminalCacheMap;
typedef QVector<QTerminalCache> QTerminalCacheVec;

struct QLNodeCache{
    QString key;

    QString lnInst;
    QString lnClass;
    QString iedName;
    QString ldInst;
    QString prefix;
    QString lnType;
};
typedef QMap<QString, QLNodeCache> QLNodeCacheMap;
typedef QVector<QLNodeCache> QLNodeCacheVec;

struct QConnectivityNodeCache{
    QString key;

    QString name;
    QString desc;
    QString pathname;
};
typedef QMap<QString, QConnectivityNodeCache> QConnectivityNodeCacheMap;
typedef QVector<QConnectivityNodeCache> QConnectivityNodeCacheVec;

// 导电设备
struct QConductingEquipmentCache{
    QString key;

    QString name;
    QString desc;
    QString vir;
    QString type;

    QTerminalCacheVec terminalVec;
    QLNodeCacheVec lnVec;
    // CommonEquipmentCacheMap subEquipmentMap;
};
typedef QMap<QString, QConductingEquipmentCache> QConductingEquipmentCacheMap;
typedef QVector<QConductingEquipmentCache> QConductingEquipmentCacheVec;

// 变压器绕组
struct QTransformerWindingCache{
    QString key;

    QString name;
    QString desc;
    QString vir;
    QString type;

    QTerminalCacheVec terminalVec;
    QLNodeCacheVec lnVec;
};
typedef QMap<QString, QTransformerWindingCache> QTransformerWindingCacheMap;
typedef QVector<QTransformerWindingCache> QTransformerWindingCacheVec;

// 变压器设备
struct QPowerTransformerCache{
    QString key;

    QString name;
    QString desc;
    QString vir;
    QString type;

    QTransformerWindingCacheVec tfWindingVec; // 绕组
    QLNodeCacheVec lnodeVec; 
};
typedef QMap<QString, QPowerTransformerCache> QPowerTransformerCacheMap;

struct QBayCache{
    QString key;

    QString name;
    QString desc;
    QString dtype; // 间隔类型

    //QPowerTransformerCacheMap ptMap; //变压器列表
    //QConductingEquipmentCacheMap ceMap; //导电设备列表
    //QConnectivityNodeCacheMap cnMap; // 导电节点；
    //QLNodeCacheMap lnMap; // LNode 列表
    // FunctionCacheMap funcMap; // 虚拟间隔中的内容
};
typedef QMap<QString, QBayCache> QBayCacheMap;

struct QVoltageCache{
    QString multiplier;
    QString unit;
    QString text;
};

// 电压等级
struct QVoltageLevelCache{
    QString key;

    QString name;
    QString desc;

    QVoltageCache voltage;
    //QBayCacheMap bayMap;
};
typedef QVector<QVoltageLevelCache> QVoltageLevelVector;

// Substation 结构
struct QSSDSubstationCache{
    QString key;

    QString name;
    QString desc;

    // private 节点
    QString dtype; // 变电站类型 CIME-type
    QString areaName; // 变电站区域缩写 CIME-area
    QString areaDesc; // 变电站区域描述 CIME-area

    //QVoltageLevelVector vVoltageLevel; // 此处用vector存储 便于后期比较顺序
};

class SCLModelSvrAdapterInterface
{
public:
  virtual QString GetSubstationImportTime(QString subStationKey) = 0;
  //模型通用接口;
  virtual QString GetParentKey(QString childKey) = 0;                                           //根据子节点Key获得父节点Key
  virtual QString GetParentName(QString childKey) = 0;                                          //根据子节点Key获得父节点的Name
  virtual QString GetNameFromKey(QString key) = 0;                                              //根据Key获得名称
  virtual QString GetChildKeyFromParentKeyAndChildName(QString parentKey,QString childName) = 0;//根据父节点的Key和子节点Name获得子节点的Key 

  //模型基本访问接口
  virtual bool   GetVoltageLevelList(QVector<QString>* nameList) = 0;                                          //获得电压等级名列表（同Key）
  virtual bool   GetSubstationList(QString voltageLevelKey,QVector<QString>* subStationKeyList) = 0;           //获得变电站Key列表
  virtual bool   GetSubstationInfo(QString subStationKey, QSubStationInfo& subStationInfo) = 0;//获得变电站（特定SCD版本）信息
  virtual QString GetSubstationPrivateXMLInfo(const QString& subStationKey) = 0;//获得变电站private标签XML原文
  virtual QString GetSubstationHeaderXMLInfo(const QString& subStationKey) = 0;//获得变电站Header标签XML原文
  virtual bool   GetSCDHItem(QString subStationKey, QVector<QHItemInfo>& hItemList) = 0;//获得SCD历史修订记录
  virtual bool   GetSubNetList(QString subStationKey, QVector<QString>& subNetKeyList) = 0;//获得子网Key列表
  virtual bool   GetSubNetInfo(QString subNetKey, QSubNetInfo& subNetInfo) = 0;//获得子网信息
  virtual QString GetSubNetXMLInfo(const QString& subNetKey) = 0;//获得子网XML片段;
  virtual bool   GetAPInfo(QString subNetName, QString IEDName, QString APName, QAPInfo& apInfo) = 0;//获得访问点信息
  virtual bool   GetPanelList(QString subStationKey,QVector<QString>* subPanelKeyList) = 0;                    //获得屏柜Key列表
  virtual bool   GetIEDList(QString panelKey,QVector<QString>* subIEDKeyList) = 0;                             //获得装置Key列表
  virtual bool   GetIEDListBySubNet(QString subNetKey, QVector<QString>& subIEDKeyList) = 0;//获得装置Key列表(根据子网)
  virtual bool   GetIEDInfo(QString IEDKey, QIEDInfo& IEDInfo) = 0;//获得装置信息	 
  virtual QString GetIEDXMLInfo(QString& IEDKey) = 0;//获得IED的XML信息
  virtual bool   GetIEDType(QString IEDKey,int* type) = 0;                                                     //获得装置类型
  virtual QString GetSCDVersion(QString subStationKey) = 0;                                                    //获得当前模型的SCD文件版本号
  virtual bool   IsVoltageLevelExist(QString voltageLevelName) = 0;                                            //是否存在电压等级
  virtual bool   IsSubstationExist(QString substationKey) = 0;                                                 //是否存在变电站		
  virtual bool   GetPortsList(QString IEDKey,QVector<QString>* portKeys) = 0;                                  //获得装置的物理端口列表
  virtual QString GetPortDescByPortKey(QString portKey) = 0;                                                   //获得物理端口的描述
  virtual bool   GetLinksListOfIED(QString IEDKey,QVector<QPhysLink>* links) = 0;                              //获得装置的物理链接列表  
  virtual bool   GetInSignalsListOfIED(QString IEDKey,QSignalTypeEnum signalType,QVector<QString>* inSignalKeys) = 0;                     //获得装置的输入信号列表
  virtual bool   GetOutSignalsListOfIED(QString IEDKey,QSignalTypeEnum signalType,QVector<QString>* outSignalKeys) = 0;                   //获得装置的输出信号列表            
  virtual bool   GetVritualLinksOfIED(QString IEDKey,QSignalTypeEnum signalType,QVector<QVirtualLink>* links) = 0;                        //获得装置的虚拟链接列表（装置和装置之间）
  virtual bool   GetInternalVritualLinksOfIED(QString IEDKey,QSignalTypeEnum signalType,QVector<QVirtualLink>* links) = 0;                //获得装置内部的虚拟链接列表（装置内部）     
  virtual bool   GetFunctionsList(QString IEDKey,QVector<QString>* protectFuncKeys) = 0;                       //获得装置的功能列表
  virtual bool   GetVirtualLinksListOfPhysicsLink(QPhysLink phyLink,QSignalTypeEnum signalType,QVector<QVirtualLink>* links) = 0;         //获得物理链接关联的虚拟链接列表
  virtual bool   GetTripsList(QString IEDKey,QVector<QString>* tripKeys) = 0;                                  //获得装置的压板列表
  virtual bool   GetInfoOfInSignal(QString signalKey,QSignalInfo *signalInfo) = 0;                             //获得输入信号的信息  
  virtual bool   GetInfoOfOutSignal(QString signalKey,QSignalInfo *signalInfo) = 0;                            //获得输出信号的信息
  virtual bool   GetInfoOfDSSettingOutSignal(QString signalKey,QDSSettingSignalInfo *signalInfo) = 0;
  virtual bool   GetSignalInfosByIEDKeyAndCBKey(const QString masterIEDKey, const QString slaveIEDKey, const QString CBkey, QList<QSignalInfo> *mSigInfos, QList<QSignalInfo> *sSigInfos) = 0;
  virtual bool   GetSignalInfosByIEDKey(const QString IEDKey, QList<QSignalInfo> *mSigInfos, QList<QSignalInfo> *sSigInfos) = 0;

  virtual bool   GetCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList) = 0;					   //获取当前IED的数据集列表
  virtual bool   GetDsSettingListByIED(QString IEDKey, QVector<QDataSetInfo>* dsSettingList) = 0;
  virtual bool   GetCtrlBlockInfo(QString ctrlBlockKey, QCtrlBlockInfo& cbInfo) = 0;							   //获取数据集信息
  virtual bool   GetDataSetSignalsByCtrlBlockID(QString cbKey, QVector<QString>* dsSignalList) = 0;//根据控制块ID获取数据集信号
  virtual bool   GetSlaveCtrlBlockByIEDKeys(QString masterKey, QString slaveKey, QVector<QString>* ctrlBlockList) = 0;	//获取发信号的IED的控制块;
  virtual bool   GetOutSignalsByCBKey(QString masterIEDKey, QString cbKey, QVector<QString>* outSignals) = 0;		//通过控制块获取虚链接;
  virtual bool   GetOutSignalsByDataSetKey(QString masterIEDKey, QString dataSetKey, QVector<QString>* outSignals) = 0;
  virtual bool   GetAssoIEDListByCBKey(QString cbKey, QVector<QString>* assoIEDKeys) = 0;		//通过控制块获取对端IED;

  virtual int    GetItemChangeType(QString ItemName) = 0;									//获取差异类型
  virtual int	 GetVLinkChangeType(QString masterIEDName, QString slaveIEDName, QString cbName) = 0;//获取虚回路差异类型(信息流图);

  virtual bool   GetPhyPortInfo(QString pPortKey, QPhyPortInfo& pPortInfo) = 0;				//获取物理端口信息;
  virtual bool   GetPhyPortListByIED(QString IEDKey, QVector<QString>* pPortList) = 0;		//根据IED获取物理端口;
  virtual bool   GetPhyPortListByInSignalID(QString signalKey, QVector<QString> &pPortList) = 0;			//根据输入信号获取端口;
  virtual bool   GetPhyPortKeyByOutSignalID(QString signalKey, QString* pPortKey) = 0;			//根据输出信号获取端口;

  //virtual bool   GetSlaveCtrlBlockCountByIEDKeys(QString);

  //高级访问接口
  virtual bool   GetIEDListByLogicalRealitionWithCurrentIED(QString IEDKey,QVector<QString>* associatedIEDKeys) = 0;    //获得IED的关联IED列表（逻辑）
  virtual bool   GetRealitionTypeOfSrcIEDAndDestIED(QString srcIEDKey,QString destIEDKey,int *type) = 0;        //获得IED之间的关联类型（0：源IED为输入）
																																  //1: 源IED为输出
																																  //2: 源IED既是输入又是输出
																																  //-1:源IED和目的IED没有关系）

  virtual bool   GetIEDListByPhysicalRealitionWithCurrentIED(QString IEDKey,QVector<QString>* associatedIEDKeys) = 0;   //获得IED的关联IED列表（物理）  
  virtual bool   GetPhysicsLinkAssociatedIED(QString srcIEDKey,QString destIEDKey,QVector<QPhysLink>* links) = 0;       //获得IED与IED之间物理链接（多个链接表示多个物理通道，0个链接表示无直接连接）
  virtual bool   GetLinksBetweenPort(QString srcPortKey,QString destPortKey,QVector<QPhysLink>* links) = 0;             //获得原port与目的port之间的物理链接（多个链接表示之间有多个设备）
  virtual bool   GetVirtualSignalsAssociatedIED(QString srcIEDKey,QString destIEDKey,QSignalTypeEnum signalType,QVector<QVirtualLink>* links) = 0; //获得IED与IED之间虚信号   
  virtual bool   GetPortsListOfPort(QString portKey,QVector<QString>* portKeys) = 0;                                    //获得与物理端口关联的其它物理端口
  virtual bool   GetPhysicsLinkeByPort(QString srcPortKey,QString destPortKey,QPhysLink* link) = 0;                     //根据两个端口获得物理链接   
  virtual bool   GetInSignalsListOfOutSignal(QString outSignalKey,QVector<QString>* inSignalKeys) = 0;                  //获得输出信号关联的输入信号列表（两个装置之间）   
  virtual bool   GetOutSignalsListOfInSignal(QString inSignalKey,QVector<QString>* outSignalKeys) = 0;                  //获得输入信号关联的输出信号列表（两个装置之间） 
  virtual bool   GetInternalOutSignalsListOfInSignal(QString inSignalKey,QVector<QString>* outSignalKeys) = 0;          //获得输入信号关联的输出信号列表（装置内部）
  virtual bool   GetInternalInSignalsListOfOutSignal(QString outSignalKey,QVector<QString>* inSignalKeys) = 0;          //获得输出信号关联的输入信号列表（装置内部）
  virtual bool   GetVirtualLinkBySignal(QString srcSignalKey,QString destSignalKey,QVirtualLink* link) = 0;             //根据两个信号获得虚拟链接        
  virtual bool   GetFunctionListOfInSignal(QString inSignalKey,QVector<QString>* funcKeys) = 0;                         //获得输入信号关联的功能列表（本装置功能）
  virtual bool   GetFunctionsListOfOutSignal(QString outSignalKey,QVector<QString>* funcKeys) = 0;                      //获得输出信号关联的功能列表 （其它装置功能）
  virtual bool   GetFunctionsListOfOutSignalWithDestIED(QString outSignalKey,QString destIEDKey,QVector<QString>* funcKeys) = 0; //获得输出信号相对于某装置的功能列表（某装置功能）  
  virtual bool   GetFunctionsListOfPort(QString portKey,QVector<QString>* FuncKeys) = 0;                                //获得物理端口功能列表     
  virtual bool   GetFunctionsListOfTrip(QString tripKey,QVector<QString>* FuncKeys) = 0;                                //获得压板对应的功能列表     
  virtual QString GetFunctionDesc(QString functionKey) = 0;                                                             //获得功能的功能描述
  virtual QString GetFunctionTripKey(QString functionKey) = 0;                                                          //获得功能的功能压板（可能不存在） 
  virtual QString GetIEDTripKey(QString IEDKey) = 0 ;                                                                   //获得设备的压板（表示断电一类的操作） 
  virtual QString GetPortTripKey(QString portKey) = 0;                                                                  //获得端口的压板（表示拨光纤一类的操作） 
  virtual QString GetSignalTripKey(QString signalKey) = 0;                                                              //获得信号的压板（软压板）   

  //模型编辑接口
  virtual bool   AddVoltageLevel(QString voltageLevelName) = 0;                                         //增加电压等级
  virtual bool   DeleteVoltageLevel(QString voltageLevelName) = 0;                                      //删除电压等级
  virtual bool   AddSubstation(QString voltageLevelKey,QString subName/*,QByteArray& fileData*/) = 0;       //增加变电站模型
  virtual int    GetSubstationAddedProgress() = 0;                                                      //获得SCD导入的进度
  virtual bool   DeleteSubstation(QString substationKey) = 0;                                           //删除变电站模型
  virtual bool   UpdateSubstationModel(QString voltageLevelKey,QString subName,QString SCDFilePath) = 0;//更新变电站模型
  virtual bool   AddPanelForSubstation(QString subKey,QString paneName) = 0;                            //为变电站增加屏柜
  virtual bool   DeletePanelForSubstation(QString PanelKey) = 0;                                        //为变电站删除屏柜   		
  virtual bool   AddIEDForPanel(QString panelKey,QString IEDName) = 0;                                  //为屏柜增加装置
  virtual bool   DeleteIEDForPanel(QString IEDKey) = 0;                                                 //为屏柜删除装置  
  virtual bool   AddPortForIED(QString IEDKey,QString portName) = 0;                                    //为装置增加物理端口
  virtual bool   DeletePortForIED(QString portKey) = 0;                                                 //为装置删除物理端口
  virtual bool   AddInSignalForIED(QString IEDKey,QString inSignalName) = 0;                            //为装置增加输入信号
  virtual bool   DeleteInSignalForIED(QString inSignalKey) = 0;                                         //为装置删除输入信号 
  virtual bool   AddOutSignalForIED(QString IEDKey,QString outSignalName) = 0;                          //为装置增加输出信号
  virtual bool   DeleteOutSignalForIED(QString outSignalKey) = 0;                                       //为装置删除输出信号
  virtual bool   AddFunction(QString IEDKey,QString funName,QString funcDesc) = 0;                      //为装置增加功能 
  virtual bool   ModifyFunction(QString funKey,QString funcDesc) = 0;                                   //为装置修改功能描述    
  virtual bool   DeleteFunction(QString funKey) = 0;                                                    //为装置删除功能 
  virtual bool   SetDescForPort(QString portKey,QString desc) = 0;                                      //设置物理端口的描述     
  virtual bool   SetInSignalsListOfOutSignal(QString outSignalKey,QVector<QString>* inSignalKeys) = 0;          //设置输出信号关联的输入信号列表（两个装置之间）   
  virtual bool   SetOutSignalsListOfInSignal(QString inSignalKey,QVector<QString>* outSignalKeys) = 0;          //设置输入信号关联的输出信号列表（两个装置之间） 
  virtual bool   SetInternalOutSignalsListOfInSignal(QString inSignalKey,QVector<QString>* outSignalKeys) = 0;  //设置输入信号关联的输出信号列表（装置内部）
  virtual bool   SetInternalInSignalsListOfOutSignal(QString outSignalKey,QVector<QString>* inSignalKeys) = 0;  //设置输出信号关联的输入信号列表（装置内部）
  virtual bool   AddFunctionOfInSignal(QString inSignalKey,QString funKey) = 0;                          //为输入信号增加功能关联  
  virtual bool   DeleteFunctionOfInSignal(QString inSignalKey,QVector<QString>* funcKeys) = 0;           //为输入信号删除功能关联 
  virtual bool   AddTripForIED(QString IEDKey,QString tripName) = 0;                                     //增加压板  
  virtual bool   DeleteTripForIED(QString tripKey) = 0;                                                  //删除压板  
  virtual bool   SetFunctionTripKey(QString functionKey,QString tripKey) = 0;                            //设置功能的功能压板（可能不存在） 
  virtual bool   SetIEDTripKey(QString IEDKey,QString tripKey) = 0;                                      //设置设备的压板（表示断电一类的操作） 
  virtual bool   SetPortTripKey(QString portKey,QString tripKey) = 0;                                    //设置端口的压板（表示拨光纤一类的操作） 
  virtual bool   SetSignalTripKey(QString signalKey,QString tripKey) = 0;                                //设置信号的压板（软压板）   

  //压板状态
  virtual bool   GetSoftTripState(QString tripKey,bool &state) = 0;                //获得压板状态
  virtual bool   SetSoftTripState(QString tripKey,bool state) = 0;     //设置压板状态

  // 取得子网下IED的AP列表
  virtual bool GetAPListOfIEDFromSubnet(const QString& subNetKey, const QString IEDName, QVector<QString> &APList) = 0;
  // 取得子网下IED某个AP包含的物理端口列表
  virtual bool GetPortListOfIEDAPFromSubNet(const QString& subNetKey, const QString IEDName, const QString APName, QVector<QString> &PortList) = 0;
  // 取得IED下AP的列表
  virtual bool GetAPListFromIED(const QString& IEDKey, QVector<QString> &APList) = 0;
  // 取得IED下某个AP对应的物理端口列表
  virtual bool GetAPPortListFormIED(const QString& IEDKey, const QString &APName, QVector<QString> &PortList) = 0;

  // 获取SSD信息
  virtual bool GetSSDInfoFromSCD(const QString subStationKey, SubstationCache &ssdInfo) = 0;

};

#endif