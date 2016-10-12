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
	//ģ��ͨ�ýӿ�
	QString GetSubstationImportTime();
	virtual ::std::string GetParentKey(const ::std::string& childKey); //�����ӽڵ�Key��ø��ڵ�Key
	virtual ::std::string GetParentName(const ::std::string& childKey);//�����ӽڵ�Key��ø��ڵ��Name
	virtual ::std::string GetNameFromKey(const ::std::string& key);    //����Key�������
	virtual ::std::string GetChildKeyFromParentKeyAndChildName(const ::std::string& parentKey, const ::std::string& childName);//���ݸ��ڵ��Key���ӽڵ�Name����ӽڵ��Key 

    //ģ�ͻ������ʽӿ�
	virtual bool GetVoltageLevelList(::std::vector<::std::string>& nameList);   //��õ�ѹ�ȼ����б�ͬKey��
	virtual bool GetSubstationList(const ::std::string& voltageLevelKey, std::vector<std::string>& subStationKeyList);//��ñ��վKey�б�
	virtual bool GetSubstationInfo(const ::std::string& subStationKey, SubStationInfo& subStationInfo);//��ñ��վ���ض�SCD�汾����Ϣ	
	virtual bool GetSCDHItem(const ::std::string& subStationKey, std::vector<HItemInfo>& hItemList);//���SCD��ʷ�޶���¼
	virtual bool GetSubNetList(const ::std::string& subStationKey, std::vector<std::string>& subNetKeyList);//�������Key�б�
	virtual bool GetSubNetInfo(const ::std::string& subNetKey, SubNetInfo& subNetInfo);//���������Ϣ
	virtual bool GetApInfo(const ::std::string& subNetName,const ::std::string& IEDName, APInfo& apInfo);//��÷��ʵ���Ϣ
	virtual bool GetPanelList(const ::std::string& subStationKey,std::vector<std::string>& subPanelKeyList);//�������Key�б�
	virtual bool GetIEDList(const ::std::string& panelKey,std::vector<std::string>& subIEDKeyList);//���װ��Key�б�
	virtual bool GetIEDListBySubNet(const ::std::string& subNetKey, std::vector<std::string>& subIEDKeyList);//���װ��Key�б�(��������)
	virtual bool GetIEDInfo(const ::std::string& IEDKey, DEVInfo& IEDInfo);//���װ����Ϣ	
	virtual bool GetIEDType(const ::std::string& IEDKey,  int& type);//���װ������
	virtual ::std::string GetSCDVersion(const ::std::string& subStationKey);//��õ�ǰģ�͵�SCD�ļ��汾��
	virtual bool IsVoltageLevelExist(const ::std::string& voltageLevelName);//�Ƿ���ڵ�ѹ�ȼ�
	virtual bool IsSubstationExist(const ::std::string& substationKey);//�Ƿ���ڱ��վ
	virtual bool GetPortsList(const ::std::string& IEDKey,std::vector<std::string>& portKeys);//���װ�õ�����˿��б�
	virtual ::std::string GetPortDescByPortKey(const ::std::string& portKey);//�������˿ڵ�����
	virtual bool GetLinksListOfIED(const ::std::string& IEDKey, std::vector<PhysLinkInfo>& links);//���װ�õ����������б�
	virtual bool GetInSignalsListOfIED(const ::std::string& IEDKey, SignalTypeEnum signalType,std::vector<std::string>& inSignalKeys);//���װ�õ������ź��б�
	virtual bool GetOutSignalsListOfIED(const ::std::string& IEDKey,  SignalTypeEnum signalType,std::vector<std::string>& outSignalKeys);//���װ�õ�����ź��б�
	virtual bool GetVritualLinksOfIED(const ::std::string& IEDKey,  SignalTypeEnum signalType, std::vector<VirtualLinkInfo>& links);//���װ�õ����������б�װ�ú�װ��֮�䣩
	virtual bool GetInternalVritualLinksOfIED(const ::std::string& IEDKey,  SignalTypeEnum signalType, std::vector<VirtualLinkInfo>& links);//���װ���ڲ������������б�װ���ڲ���
	virtual bool GetFunctionsList(const ::std::string& IEDKey,std::vector<std::string>& protectFuncKeys);//���װ�õĹ����б�
	virtual bool GetVirtualLinksListOfPhysicsLink(const PhysLinkInfo& phyLink,  SignalTypeEnum signalType, std::vector<VirtualLinkInfo>& links);//����������ӹ��������������б�
	virtual bool GetTripsList(const ::std::string& IEDKey,std::vector<std::string>& tripKeys);//���װ�õ�ѹ���б�  
	virtual bool GetInfoOfInSignal(const ::std::string& signalKey, SignalInfo& signalInfo);//��������źŵ���Ϣ  
	virtual bool GetInfoOfOutSignal(const ::std::string& signalKey, SignalInfo& signalInfo);//�������źŵ���Ϣ

	virtual bool GetCtrlBlockListByIED(const ::std::string& IEDKey, std::vector<std::string>& ctrlBlockKeys);//��ȡ��ǰIED�����ݼ��б�
	virtual bool GetCtrlBlockInfo(const ::std::string& ctrlBlockKey, CtrlBlockInfo& cbInfo);//��ȡ���ݼ���Ϣ
	virtual bool GetDataSetSignalsByCtrlBlockID(const ::std::string& cbKey, std::vector<std::string>& dsSignalList);//��ȡ���ݼ����ź�
	virtual bool GetSlaveCtrlBlockByIEDKeys(const ::std::string& masterKey, const ::std::string& slaveKey, std::vector<std::string>& ctrlBlockList);//��������IED��Key��ȡ��IED�õ��Ŀ��ƿ�
	virtual bool GetOutSignalsByCBKey(const ::std::string& masterIEDKey, const ::std::string& cbKey, std::vector<std::string>& outSignals);		//ͨ�����ƿ��ȡ������;
	virtual bool GetAssoIEDListByCBKey(const ::std::string& cbKey, std::vector<std::string>& assoIEDKeys);		//ͨ�����ƿ��ȡ�Զ�IED;

	virtual bool GetPhyPortInfo(const ::std::string& pPortKey, PhyPortInfo& pPortInfo);				//��ȡ����˿���Ϣ;
	virtual bool GetPhyPortListByIED(const ::std::string& IEDKey, std::vector<std::string>& pPortList);		//����IED��ȡ����˿�;
	virtual bool GetPhyPortKeyByInSignalID(const ::std::string& signalKey, ::std::string& pPortKey);			//�����źŻ�ȡ�˿�;
	virtual bool GetPhyPortKeyByOutSignalID(const ::std::string& signalKey, ::std::string& pPortKey);			//�����źŻ�ȡ�˿�;

	//�߼����ʽӿ�
	virtual bool GetIEDListByLogicalRealitionWithCurrentIED(const ::std::string& IEDKey,std::vector<std::string>& associatedIEDKeys);//���IED�Ĺ���IED�б��߼���
	virtual bool GetRealitionTypeOfSrcIEDAndDestIED(const ::std::string& srcIEDKey, const ::std::string& destIEDKey, int& type);//���IED֮��Ĺ������ͣ�0��ԴIEDΪ���룩																																												//1: ԴIEDΪ���																																											//2: ԴIED��������������																																												//-1:ԴIED��Ŀ��IEDû�й�ϵ��
	virtual bool GetIEDListByPhysicalRealitionWithCurrentIED(const ::std::string& IEDKey,std::vector<std::string>& associatedIEDKeys);//���IED�Ĺ���IED�б�����  
	virtual bool GetPhysicsLinkAssociatedIED(const ::std::string& srcIEDKey, const ::std::string& destIEDKey, std::vector<PhysLinkInfo>& links);//���IED��IED֮���������ӣ�������ӱ�ʾ�������ͨ����0�����ӱ�ʾ��ֱ�����ӣ�
	virtual bool GetLinksBetweenPort(const ::std::string& srcPortKey, const ::std::string& destPortKey, std::vector<PhysLinkInfo>& links);//���ԭport��Ŀ��port֮����������ӣ�������ӱ�ʾ֮���ж���豸��
	virtual bool GetVirtualSignalsAssociatedIED(const ::std::string& srcIEDKey, const ::std::string& destIEDKey,  SignalTypeEnum signalType, std::vector<VirtualLinkInfo>& links);//���IED��IED֮�����ź� 
	virtual bool GetPortsListOfPort(const ::std::string& portKey,std::vector<std::string>& portKeys);//���������˿ڹ�������������˿�
	virtual bool GetPhysicsLinkeByPort(const ::std::string& srcPortKey, const ::std::string& destPortKey, PhysLinkInfo& link);//���������˿ڻ���������� 
	virtual bool GetInSignalsListOfOutSignal(const ::std::string& outSignalKey,std::vector<std::string>& inSignalKeys);//�������źŹ����������ź��б�����װ��֮�䣩
	virtual bool GetOutSignalsListOfInSignal(const ::std::string& inSignalKey,std::vector<std::string>& outSignalKeys);//��������źŹ���������ź��б�����װ��֮�䣩
	virtual bool GetInternalOutSignalsListOfInSignal(const ::std::string& inSignalKey,std::vector<std::string>& outSignalKeys);//��������źŹ���������ź��б�װ���ڲ���
	virtual bool GetInternalInSignalsListOfOutSignal(const ::std::string& outSignalKey,std::vector<std::string>& inSignalKeys);//�������źŹ����������ź��б�װ���ڲ���
	virtual bool GetVirtualLinkBySignal(const ::std::string& srcSignalKey, const ::std::string& destSignalKey, VirtualLinkInfo& link);//���������źŻ���������� 
	virtual bool GetFunctionListOfInSignal(const ::std::string& inSignalKey,std::vector<std::string>& funcKeys);//��������źŹ����Ĺ����б���װ�ù��ܣ�
	virtual bool GetFunctionsListOfOutSignal(const ::std::string& outSignalKey,std::vector<std::string>& funcKeys);//�������źŹ����Ĺ����б� ������װ�ù��ܣ�
	virtual bool GetFunctionsListOfOutSignalWithDestIED(const ::std::string& outSignalKey, const ::std::string& destIEDKey,std::vector<std::string>& funcKeys);//�������ź������ĳװ�õĹ����б�ĳװ�ù��ܣ�
	virtual bool GetFunctionsListOfPort(const ::std::string& portKey,std::vector<std::string>& FuncKeys);//�������˿ڹ����б�
	virtual bool GetFunctionsListOfTrip(const ::std::string& tripKey,std::vector<std::string>& FuncKeys);//���ѹ���Ӧ�Ĺ����б�
	virtual ::std::string GetFunctionDesc(const ::std::string& functionKey);//��ù��ܵĹ�������
	virtual ::std::string GetFunctionTripKey(const ::std::string& functionKey);//��ù��ܵĹ���ѹ�壨���ܲ����ڣ�
	virtual ::std::string GetIEDTripKey(const ::std::string& IEDKey) ;//����豸��ѹ�壨��ʾ�ϵ�һ��Ĳ�����
	virtual ::std::string GetPortTripKey(const ::std::string& portKey) ;//��ö˿ڵ�ѹ�壨��ʾ������һ��Ĳ�����
	virtual ::std::string GetSignalTripKey(const ::std::string& signalKey);//����źŵ�ѹ�壨��ѹ�壩


    //ģ�ͱ༭�ӿ�
	virtual bool AddVoltageLevel(const ::std::string& voltageLevelName);//���ӵ�ѹ�ȼ�
	virtual bool DeleteVoltageLevel(const ::std::string& voltageLevelName);//ɾ����ѹ�ȼ�
	virtual bool AddSubstation(const ::std::string& voltageLevelKey, const ::std::string& subName/*, const std::vector<char>& fileData*/);//���ӱ��վģ��
	virtual int  GetSubstationAddedProgress();//��õ�ǰ�������
	virtual bool DeleteSubstation(const ::std::string& substationKey);//ɾ�����վģ��
	virtual bool UpdateSubstationModel(const ::std::string& voltageLevelKey, const ::std::string& subName, const ::std::string& SCDFilePath);//���±��վģ��
	virtual bool AddPanelForSubstation(const ::std::string& subKey, const ::std::string& paneName);//Ϊ���վ��������
	virtual bool DeletePanelForSubstation(const ::std::string& PanelKey);//Ϊ���վɾ������
	virtual bool AddIEDForPanel(const ::std::string& panelKey, const ::std::string& IEDName);//Ϊ��������װ��
	virtual bool DeleteIEDForPanel(const ::std::string& IEDKey);//Ϊ����ɾ��װ��
	virtual bool AddPortForIED(const ::std::string& IEDKey, const ::std::string& portName);//Ϊװ����������˿�
	virtual bool DeletePortForIED(const ::std::string& portKey);//Ϊװ��ɾ������˿�
	virtual bool AddInSignalForIED(const ::std::string& IEDKey, const ::std::string& inSignalName);//Ϊװ�����������ź�
	virtual bool DeleteInSignalForIED(const ::std::string& inSignalKey);//Ϊװ��ɾ�������ź�
	virtual bool AddOutSignalForIED(const ::std::string& IEDKey, const ::std::string& outSignalName); //Ϊװ����������ź�
	virtual bool DeleteOutSignalForIED(const ::std::string& outSignalKey);//Ϊװ��ɾ������ź�
	virtual bool AddFunction(const ::std::string& IEDKey, const ::std::string& funName, const ::std::string& funcDesc);//Ϊװ�����ӹ���
	virtual bool ModifyFunction(const ::std::string& funKey, const ::std::string& funcDesc);//Ϊװ���޸Ĺ�������  
	virtual bool DeleteFunction(const ::std::string& funKey);//Ϊװ��ɾ������ 
	virtual bool SetDescForPort(const ::std::string& portKey, const ::std::string& desc);//��������˿ڵ�����  
	virtual bool SetInSignalsListOfOutSignal(const ::std::string& outSignalKey, const std::vector<std::string>& inSignalKeys);//��������źŹ����������ź��б�����װ��֮�䣩
	virtual bool SetOutSignalsListOfInSignal(const ::std::string& inSignalKey, const std::vector<std::string>& outSignalKeys);//���������źŹ���������ź��б�����װ��֮�䣩
	virtual bool SetInternalOutSignalsListOfInSignal(const ::std::string& inSignalKey, const std::vector<std::string>& outSignalKeys);//���������źŹ���������ź��б�װ���ڲ���
	virtual bool SetInternalInSignalsListOfOutSignal(const ::std::string& outSignalKey, const std::vector<std::string>& inSignalKeys);//��������źŹ����������ź��б�װ���ڲ���
	virtual bool AddFunctionOfInSignal(const ::std::string& inSignalKey, const ::std::string& funKey);//Ϊ�����ź����ӹ��ܹ���
	virtual bool DeleteFunctionOfInSignal(const ::std::string& inSignalKey,const std::vector<std::string>& funcKeys);//Ϊ�����ź�ɾ�����ܹ��� 
	virtual bool AddTripForIED(const ::std::string& IEDKey, const ::std::string& tripName);//����ѹ��
	virtual bool DeleteTripForIED(const ::std::string& tripKey);//ɾ��ѹ�� 
	virtual bool SetFunctionTripKey(const ::std::string& functionKey, const ::std::string& tripKey);//���ù��ܵĹ���ѹ�壨���ܲ����ڣ� 
	virtual bool SetIEDTripKey(const ::std::string& IEDKey, const ::std::string& tripKey);//�����豸��ѹ�壨��ʾ�ϵ�һ��Ĳ����� 
	virtual bool SetPortTripKey(const ::std::string& portKey, const ::std::string& tripKey);//���ö˿ڵ�ѹ�壨��ʾ������һ��Ĳ�����
	virtual bool SetSignalTripKey(const ::std::string& signalKey, const ::std::string& tripKey);//�����źŵ�ѹ�壨��ѹ�壩  

	virtual bool GetSoftTripState(const ::std::string& tripKey, bool& state);//���ѹ��״̬
	virtual bool SetSoftTripState(const ::std::string& tripKey, bool state);//����ѹ��״̬

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

	//���ݼ�ֵȥ��Ӧʵʱ����ȡ��¼������Ϊ��ֵ�Ϳ�ָ�룬���Ϊ��¼data
	// bool			GetRdbRecByKey(const ::std::string& keystring,Db *db,Dbt &rec);
	std::string			GetIedKeyFromSignal(const std::string& keystring);

	//����OutSignal�ļ�ֵȡ������IED����:Vol.Sub.Panel.Ied.Port.OutSignal
	// SCDLoader scdLoader;
	// SCDUnLoader scdUnLoader;
	int  curProgress;
	// CRDBEnvironment *env;

private:
	bool IsRecv(const std::string& masterKey, const std::string& slaveKey);
};

