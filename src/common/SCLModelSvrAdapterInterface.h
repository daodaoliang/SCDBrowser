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

// ��ֵ��Ŀ���ݼ���Ϣ
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
	QString inSigInfo;	//���������ź�����;

	QString diffDesc; // ���Ӳ��컯���������ڵ��ͼ����ʾ������Ϣ;

	int DiffState;	//��������;
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

	QString diffDesc; // ���Ӳ��컯���������ڵ��ͼ����ʾ������Ϣ

	int DiffState;	//��������;
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

	QString diffDesc; // ���Ӳ��컯���������ڵ��ͼ����ʾ������Ϣ

	int DiffState;	//��������;
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
	QString intgPd; // report���ƿ�����������
	QString Buffered; // �涨�����Ƿ񻺴�
	QString bufTime;  // ����ʱ��


	QString diffDesc; // ���Ӳ��컯���������ڵ��ͼ����ʾ������Ϣ
	
	int DiffState;	//��������;
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

	QString diffDesc; // ���Ӳ��컯���������ڵ��ͼ����ʾ������Ϣ

	int DiffState;	//��������;
};

// -----------------------------------------------------------------------------SSD�ṹ---------------------------------------------------------------
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

// �����豸
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

// ��ѹ������
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

// ��ѹ���豸
struct QPowerTransformerCache{
    QString key;

    QString name;
    QString desc;
    QString vir;
    QString type;

    QTransformerWindingCacheVec tfWindingVec; // ����
    QLNodeCacheVec lnodeVec; 
};
typedef QMap<QString, QPowerTransformerCache> QPowerTransformerCacheMap;

struct QBayCache{
    QString key;

    QString name;
    QString desc;
    QString dtype; // �������

    //QPowerTransformerCacheMap ptMap; //��ѹ���б�
    //QConductingEquipmentCacheMap ceMap; //�����豸�б�
    //QConnectivityNodeCacheMap cnMap; // ����ڵ㣻
    //QLNodeCacheMap lnMap; // LNode �б�
    // FunctionCacheMap funcMap; // �������е�����
};
typedef QMap<QString, QBayCache> QBayCacheMap;

struct QVoltageCache{
    QString multiplier;
    QString unit;
    QString text;
};

// ��ѹ�ȼ�
struct QVoltageLevelCache{
    QString key;

    QString name;
    QString desc;

    QVoltageCache voltage;
    //QBayCacheMap bayMap;
};
typedef QVector<QVoltageLevelCache> QVoltageLevelVector;

// Substation �ṹ
struct QSSDSubstationCache{
    QString key;

    QString name;
    QString desc;

    // private �ڵ�
    QString dtype; // ���վ���� CIME-type
    QString areaName; // ���վ������д CIME-area
    QString areaDesc; // ���վ�������� CIME-area

    //QVoltageLevelVector vVoltageLevel; // �˴���vector�洢 ���ں��ڱȽ�˳��
};

class SCLModelSvrAdapterInterface
{
public:
  virtual QString GetSubstationImportTime(QString subStationKey) = 0;
  //ģ��ͨ�ýӿ�;
  virtual QString GetParentKey(QString childKey) = 0;                                           //�����ӽڵ�Key��ø��ڵ�Key
  virtual QString GetParentName(QString childKey) = 0;                                          //�����ӽڵ�Key��ø��ڵ��Name
  virtual QString GetNameFromKey(QString key) = 0;                                              //����Key�������
  virtual QString GetChildKeyFromParentKeyAndChildName(QString parentKey,QString childName) = 0;//���ݸ��ڵ��Key���ӽڵ�Name����ӽڵ��Key 

  //ģ�ͻ������ʽӿ�
  virtual bool   GetVoltageLevelList(QVector<QString>* nameList) = 0;                                          //��õ�ѹ�ȼ����б�ͬKey��
  virtual bool   GetSubstationList(QString voltageLevelKey,QVector<QString>* subStationKeyList) = 0;           //��ñ��վKey�б�
  virtual bool   GetSubstationInfo(QString subStationKey, QSubStationInfo& subStationInfo) = 0;//��ñ��վ���ض�SCD�汾����Ϣ
  virtual QString GetSubstationPrivateXMLInfo(const QString& subStationKey) = 0;//��ñ��վprivate��ǩXMLԭ��
  virtual QString GetSubstationHeaderXMLInfo(const QString& subStationKey) = 0;//��ñ��վHeader��ǩXMLԭ��
  virtual bool   GetSCDHItem(QString subStationKey, QVector<QHItemInfo>& hItemList) = 0;//���SCD��ʷ�޶���¼
  virtual bool   GetSubNetList(QString subStationKey, QVector<QString>& subNetKeyList) = 0;//�������Key�б�
  virtual bool   GetSubNetInfo(QString subNetKey, QSubNetInfo& subNetInfo) = 0;//���������Ϣ
  virtual QString GetSubNetXMLInfo(const QString& subNetKey) = 0;//�������XMLƬ��;
  virtual bool   GetAPInfo(QString subNetName, QString IEDName, QString APName, QAPInfo& apInfo) = 0;//��÷��ʵ���Ϣ
  virtual bool   GetPanelList(QString subStationKey,QVector<QString>* subPanelKeyList) = 0;                    //�������Key�б�
  virtual bool   GetIEDList(QString panelKey,QVector<QString>* subIEDKeyList) = 0;                             //���װ��Key�б�
  virtual bool   GetIEDListBySubNet(QString subNetKey, QVector<QString>& subIEDKeyList) = 0;//���װ��Key�б�(��������)
  virtual bool   GetIEDInfo(QString IEDKey, QIEDInfo& IEDInfo) = 0;//���װ����Ϣ	 
  virtual QString GetIEDXMLInfo(QString& IEDKey) = 0;//���IED��XML��Ϣ
  virtual bool   GetIEDType(QString IEDKey,int* type) = 0;                                                     //���װ������
  virtual QString GetSCDVersion(QString subStationKey) = 0;                                                    //��õ�ǰģ�͵�SCD�ļ��汾��
  virtual bool   IsVoltageLevelExist(QString voltageLevelName) = 0;                                            //�Ƿ���ڵ�ѹ�ȼ�
  virtual bool   IsSubstationExist(QString substationKey) = 0;                                                 //�Ƿ���ڱ��վ		
  virtual bool   GetPortsList(QString IEDKey,QVector<QString>* portKeys) = 0;                                  //���װ�õ�����˿��б�
  virtual QString GetPortDescByPortKey(QString portKey) = 0;                                                   //�������˿ڵ�����
  virtual bool   GetLinksListOfIED(QString IEDKey,QVector<QPhysLink>* links) = 0;                              //���װ�õ����������б�  
  virtual bool   GetInSignalsListOfIED(QString IEDKey,QSignalTypeEnum signalType,QVector<QString>* inSignalKeys) = 0;                     //���װ�õ������ź��б�
  virtual bool   GetOutSignalsListOfIED(QString IEDKey,QSignalTypeEnum signalType,QVector<QString>* outSignalKeys) = 0;                   //���װ�õ�����ź��б�            
  virtual bool   GetVritualLinksOfIED(QString IEDKey,QSignalTypeEnum signalType,QVector<QVirtualLink>* links) = 0;                        //���װ�õ����������б�װ�ú�װ��֮�䣩
  virtual bool   GetInternalVritualLinksOfIED(QString IEDKey,QSignalTypeEnum signalType,QVector<QVirtualLink>* links) = 0;                //���װ���ڲ������������б�װ���ڲ���     
  virtual bool   GetFunctionsList(QString IEDKey,QVector<QString>* protectFuncKeys) = 0;                       //���װ�õĹ����б�
  virtual bool   GetVirtualLinksListOfPhysicsLink(QPhysLink phyLink,QSignalTypeEnum signalType,QVector<QVirtualLink>* links) = 0;         //����������ӹ��������������б�
  virtual bool   GetTripsList(QString IEDKey,QVector<QString>* tripKeys) = 0;                                  //���װ�õ�ѹ���б�
  virtual bool   GetInfoOfInSignal(QString signalKey,QSignalInfo *signalInfo) = 0;                             //��������źŵ���Ϣ  
  virtual bool   GetInfoOfOutSignal(QString signalKey,QSignalInfo *signalInfo) = 0;                            //�������źŵ���Ϣ
  virtual bool   GetInfoOfDSSettingOutSignal(QString signalKey,QDSSettingSignalInfo *signalInfo) = 0;
  virtual bool   GetSignalInfosByIEDKeyAndCBKey(const QString masterIEDKey, const QString slaveIEDKey, const QString CBkey, QList<QSignalInfo> *mSigInfos, QList<QSignalInfo> *sSigInfos) = 0;
  virtual bool   GetSignalInfosByIEDKey(const QString IEDKey, QList<QSignalInfo> *mSigInfos, QList<QSignalInfo> *sSigInfos) = 0;

  virtual bool   GetCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList) = 0;					   //��ȡ��ǰIED�����ݼ��б�
  virtual bool   GetDsSettingListByIED(QString IEDKey, QVector<QDataSetInfo>* dsSettingList) = 0;
  virtual bool   GetCtrlBlockInfo(QString ctrlBlockKey, QCtrlBlockInfo& cbInfo) = 0;							   //��ȡ���ݼ���Ϣ
  virtual bool   GetDataSetSignalsByCtrlBlockID(QString cbKey, QVector<QString>* dsSignalList) = 0;//���ݿ��ƿ�ID��ȡ���ݼ��ź�
  virtual bool   GetSlaveCtrlBlockByIEDKeys(QString masterKey, QString slaveKey, QVector<QString>* ctrlBlockList) = 0;	//��ȡ���źŵ�IED�Ŀ��ƿ�;
  virtual bool   GetOutSignalsByCBKey(QString masterIEDKey, QString cbKey, QVector<QString>* outSignals) = 0;		//ͨ�����ƿ��ȡ������;
  virtual bool   GetOutSignalsByDataSetKey(QString masterIEDKey, QString dataSetKey, QVector<QString>* outSignals) = 0;
  virtual bool   GetAssoIEDListByCBKey(QString cbKey, QVector<QString>* assoIEDKeys) = 0;		//ͨ�����ƿ��ȡ�Զ�IED;

  virtual int    GetItemChangeType(QString ItemName) = 0;									//��ȡ��������
  virtual int	 GetVLinkChangeType(QString masterIEDName, QString slaveIEDName, QString cbName) = 0;//��ȡ���·��������(��Ϣ��ͼ);

  virtual bool   GetPhyPortInfo(QString pPortKey, QPhyPortInfo& pPortInfo) = 0;				//��ȡ����˿���Ϣ;
  virtual bool   GetPhyPortListByIED(QString IEDKey, QVector<QString>* pPortList) = 0;		//����IED��ȡ����˿�;
  virtual bool   GetPhyPortListByInSignalID(QString signalKey, QVector<QString> &pPortList) = 0;			//���������źŻ�ȡ�˿�;
  virtual bool   GetPhyPortKeyByOutSignalID(QString signalKey, QString* pPortKey) = 0;			//��������źŻ�ȡ�˿�;

  //virtual bool   GetSlaveCtrlBlockCountByIEDKeys(QString);

  //�߼����ʽӿ�
  virtual bool   GetIEDListByLogicalRealitionWithCurrentIED(QString IEDKey,QVector<QString>* associatedIEDKeys) = 0;    //���IED�Ĺ���IED�б��߼���
  virtual bool   GetRealitionTypeOfSrcIEDAndDestIED(QString srcIEDKey,QString destIEDKey,int *type) = 0;        //���IED֮��Ĺ������ͣ�0��ԴIEDΪ���룩
																																  //1: ԴIEDΪ���
																																  //2: ԴIED���������������
																																  //-1:ԴIED��Ŀ��IEDû�й�ϵ��

  virtual bool   GetIEDListByPhysicalRealitionWithCurrentIED(QString IEDKey,QVector<QString>* associatedIEDKeys) = 0;   //���IED�Ĺ���IED�б�����  
  virtual bool   GetPhysicsLinkAssociatedIED(QString srcIEDKey,QString destIEDKey,QVector<QPhysLink>* links) = 0;       //���IED��IED֮���������ӣ�������ӱ�ʾ�������ͨ����0�����ӱ�ʾ��ֱ�����ӣ�
  virtual bool   GetLinksBetweenPort(QString srcPortKey,QString destPortKey,QVector<QPhysLink>* links) = 0;             //���ԭport��Ŀ��port֮����������ӣ�������ӱ�ʾ֮���ж���豸��
  virtual bool   GetVirtualSignalsAssociatedIED(QString srcIEDKey,QString destIEDKey,QSignalTypeEnum signalType,QVector<QVirtualLink>* links) = 0; //���IED��IED֮�����ź�   
  virtual bool   GetPortsListOfPort(QString portKey,QVector<QString>* portKeys) = 0;                                    //���������˿ڹ�������������˿�
  virtual bool   GetPhysicsLinkeByPort(QString srcPortKey,QString destPortKey,QPhysLink* link) = 0;                     //���������˿ڻ����������   
  virtual bool   GetInSignalsListOfOutSignal(QString outSignalKey,QVector<QString>* inSignalKeys) = 0;                  //�������źŹ����������ź��б�����װ��֮�䣩   
  virtual bool   GetOutSignalsListOfInSignal(QString inSignalKey,QVector<QString>* outSignalKeys) = 0;                  //��������źŹ���������ź��б�����װ��֮�䣩 
  virtual bool   GetInternalOutSignalsListOfInSignal(QString inSignalKey,QVector<QString>* outSignalKeys) = 0;          //��������źŹ���������ź��б�װ���ڲ���
  virtual bool   GetInternalInSignalsListOfOutSignal(QString outSignalKey,QVector<QString>* inSignalKeys) = 0;          //�������źŹ����������ź��б�װ���ڲ���
  virtual bool   GetVirtualLinkBySignal(QString srcSignalKey,QString destSignalKey,QVirtualLink* link) = 0;             //���������źŻ����������        
  virtual bool   GetFunctionListOfInSignal(QString inSignalKey,QVector<QString>* funcKeys) = 0;                         //��������źŹ����Ĺ����б���װ�ù��ܣ�
  virtual bool   GetFunctionsListOfOutSignal(QString outSignalKey,QVector<QString>* funcKeys) = 0;                      //�������źŹ����Ĺ����б� ������װ�ù��ܣ�
  virtual bool   GetFunctionsListOfOutSignalWithDestIED(QString outSignalKey,QString destIEDKey,QVector<QString>* funcKeys) = 0; //�������ź������ĳװ�õĹ����б�ĳװ�ù��ܣ�  
  virtual bool   GetFunctionsListOfPort(QString portKey,QVector<QString>* FuncKeys) = 0;                                //�������˿ڹ����б�     
  virtual bool   GetFunctionsListOfTrip(QString tripKey,QVector<QString>* FuncKeys) = 0;                                //���ѹ���Ӧ�Ĺ����б�     
  virtual QString GetFunctionDesc(QString functionKey) = 0;                                                             //��ù��ܵĹ�������
  virtual QString GetFunctionTripKey(QString functionKey) = 0;                                                          //��ù��ܵĹ���ѹ�壨���ܲ����ڣ� 
  virtual QString GetIEDTripKey(QString IEDKey) = 0 ;                                                                   //����豸��ѹ�壨��ʾ�ϵ�һ��Ĳ����� 
  virtual QString GetPortTripKey(QString portKey) = 0;                                                                  //��ö˿ڵ�ѹ�壨��ʾ������һ��Ĳ����� 
  virtual QString GetSignalTripKey(QString signalKey) = 0;                                                              //����źŵ�ѹ�壨��ѹ�壩   

  //ģ�ͱ༭�ӿ�
  virtual bool   AddVoltageLevel(QString voltageLevelName) = 0;                                         //���ӵ�ѹ�ȼ�
  virtual bool   DeleteVoltageLevel(QString voltageLevelName) = 0;                                      //ɾ����ѹ�ȼ�
  virtual bool   AddSubstation(QString voltageLevelKey,QString subName/*,QByteArray& fileData*/) = 0;       //���ӱ��վģ��
  virtual int    GetSubstationAddedProgress() = 0;                                                      //���SCD����Ľ���
  virtual bool   DeleteSubstation(QString substationKey) = 0;                                           //ɾ�����վģ��
  virtual bool   UpdateSubstationModel(QString voltageLevelKey,QString subName,QString SCDFilePath) = 0;//���±��վģ��
  virtual bool   AddPanelForSubstation(QString subKey,QString paneName) = 0;                            //Ϊ���վ��������
  virtual bool   DeletePanelForSubstation(QString PanelKey) = 0;                                        //Ϊ���վɾ������   		
  virtual bool   AddIEDForPanel(QString panelKey,QString IEDName) = 0;                                  //Ϊ��������װ��
  virtual bool   DeleteIEDForPanel(QString IEDKey) = 0;                                                 //Ϊ����ɾ��װ��  
  virtual bool   AddPortForIED(QString IEDKey,QString portName) = 0;                                    //Ϊװ����������˿�
  virtual bool   DeletePortForIED(QString portKey) = 0;                                                 //Ϊװ��ɾ������˿�
  virtual bool   AddInSignalForIED(QString IEDKey,QString inSignalName) = 0;                            //Ϊװ�����������ź�
  virtual bool   DeleteInSignalForIED(QString inSignalKey) = 0;                                         //Ϊװ��ɾ�������ź� 
  virtual bool   AddOutSignalForIED(QString IEDKey,QString outSignalName) = 0;                          //Ϊװ����������ź�
  virtual bool   DeleteOutSignalForIED(QString outSignalKey) = 0;                                       //Ϊװ��ɾ������ź�
  virtual bool   AddFunction(QString IEDKey,QString funName,QString funcDesc) = 0;                      //Ϊװ�����ӹ��� 
  virtual bool   ModifyFunction(QString funKey,QString funcDesc) = 0;                                   //Ϊװ���޸Ĺ�������    
  virtual bool   DeleteFunction(QString funKey) = 0;                                                    //Ϊװ��ɾ������ 
  virtual bool   SetDescForPort(QString portKey,QString desc) = 0;                                      //��������˿ڵ�����     
  virtual bool   SetInSignalsListOfOutSignal(QString outSignalKey,QVector<QString>* inSignalKeys) = 0;          //��������źŹ����������ź��б�����װ��֮�䣩   
  virtual bool   SetOutSignalsListOfInSignal(QString inSignalKey,QVector<QString>* outSignalKeys) = 0;          //���������źŹ���������ź��б�����װ��֮�䣩 
  virtual bool   SetInternalOutSignalsListOfInSignal(QString inSignalKey,QVector<QString>* outSignalKeys) = 0;  //���������źŹ���������ź��б�װ���ڲ���
  virtual bool   SetInternalInSignalsListOfOutSignal(QString outSignalKey,QVector<QString>* inSignalKeys) = 0;  //��������źŹ����������ź��б�װ���ڲ���
  virtual bool   AddFunctionOfInSignal(QString inSignalKey,QString funKey) = 0;                          //Ϊ�����ź����ӹ��ܹ���  
  virtual bool   DeleteFunctionOfInSignal(QString inSignalKey,QVector<QString>* funcKeys) = 0;           //Ϊ�����ź�ɾ�����ܹ��� 
  virtual bool   AddTripForIED(QString IEDKey,QString tripName) = 0;                                     //����ѹ��  
  virtual bool   DeleteTripForIED(QString tripKey) = 0;                                                  //ɾ��ѹ��  
  virtual bool   SetFunctionTripKey(QString functionKey,QString tripKey) = 0;                            //���ù��ܵĹ���ѹ�壨���ܲ����ڣ� 
  virtual bool   SetIEDTripKey(QString IEDKey,QString tripKey) = 0;                                      //�����豸��ѹ�壨��ʾ�ϵ�һ��Ĳ����� 
  virtual bool   SetPortTripKey(QString portKey,QString tripKey) = 0;                                    //���ö˿ڵ�ѹ�壨��ʾ������һ��Ĳ����� 
  virtual bool   SetSignalTripKey(QString signalKey,QString tripKey) = 0;                                //�����źŵ�ѹ�壨��ѹ�壩   

  //ѹ��״̬
  virtual bool   GetSoftTripState(QString tripKey,bool &state) = 0;                //���ѹ��״̬
  virtual bool   SetSoftTripState(QString tripKey,bool state) = 0;     //����ѹ��״̬

  // ȡ��������IED��AP�б�
  virtual bool GetAPListOfIEDFromSubnet(const QString& subNetKey, const QString IEDName, QVector<QString> &APList) = 0;
  // ȡ��������IEDĳ��AP����������˿��б�
  virtual bool GetPortListOfIEDAPFromSubNet(const QString& subNetKey, const QString IEDName, const QString APName, QVector<QString> &PortList) = 0;
  // ȡ��IED��AP���б�
  virtual bool GetAPListFromIED(const QString& IEDKey, QVector<QString> &APList) = 0;
  // ȡ��IED��ĳ��AP��Ӧ������˿��б�
  virtual bool GetAPPortListFormIED(const QString& IEDKey, const QString &APName, QVector<QString> &PortList) = 0;

  // ��ȡSSD��Ϣ
  virtual bool GetSSDInfoFromSCD(const QString subStationKey, SubstationCache &ssdInfo) = 0;

};

#endif