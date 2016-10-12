#ifndef  _SCL_MODEL_ADAPTER
#define  _SCL_MODEL_ADAPTER

/*
1���ļ��а����������������:SCLModelAdapterForClient��SclDacLib
2��SCLModelAdapterForClient�ఴ��Ӧ�ò�Ľӿ�Ҫ�󷵻���Ϣ����ģ�Ͳ��ģ�Ͷ�����Ϣ����
3��SclDacLib���װ�˶�SCD�����ļ����ļ�����Ĺ���
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
	//ģ��ͨ�ýӿ�
	std::string GetSubstationImportTime(const std::string& subStationKey);

    //ģ�ͻ������ʽӿ�
	virtual bool GetVoltageLevelList(std::vector<std::string>& nameList);   //��õ�ѹ�ȼ����б�ͬKey��
	virtual bool GetSubstationList(const std::string& voltageLevelKey, std::vector<std::string>& subStationKeyList);//��ñ��վKey�б�
	virtual bool GetSubstationInfo(const std::string& subStationKey, SubStationInfo& subStationInfo);//��ñ��վ���ض�SCD�汾����Ϣ
	virtual std::string GetSubstationPrivateXMLInfo(const std::string& subStationKey);//��ñ��վprivate��ǩXMLԭ��
	virtual std::string GetSubstationHeaderXMLInfo(const std::string& subStationKey);//��ñ��վHeader��ǩXMLԭ��
	virtual bool GetSCDHItem(const std::string& subStationKey, std::vector<HItemInfo>& hItemList);//���SCD��ʷ�޶���¼
	virtual bool GetSubNetList(const std::string& subStationKey, std::vector<std::string>& subNetKeyList);//�������Key�б�
	virtual bool GetSubNetInfo(const std::string& subNetKey, SubNetInfo& subNetInfo);//���������Ϣ
	virtual std::string GetSubNetXMLInfo(const ::std::string& subNetKey);//�������XMLƬ��
	virtual bool GetApInfo(const std::string& subStationKey, const std::string& subNetName,const std::string& IEDName, const std::string APName, APInfo& apInfo);//��÷��ʵ���Ϣ
	virtual bool GetPanelList(const std::string& subStationKey,std::vector<std::string>& subPanelKeyList);//�������Key�б�
	virtual bool GetIEDList(const std::string& panelKey,std::vector<std::string>& subIEDKeyList);//���װ��Key�б�
	virtual bool GetIEDListBySubNet(const std::string& subNetKey, std::vector<std::string>& subIEDKeyList);//���װ��Key�б�(��������)
	virtual bool GetIEDInfo(const std::string& IEDKey, DEVInfo& IEDInfo);//���װ����Ϣ	
	virtual std::string GetIEDXMLInfo(const ::std::string& IEDKey);//���IED��XML��Ϣ

	virtual bool GetIEDType(const std::string& IEDKey,  int& type);//���װ������
	virtual std::string GetSCDVersion(const std::string& subStationKey);//��õ�ǰģ�͵�SCD�ļ��汾��
	virtual bool IsVoltageLevelExist(const std::string& voltageLevelName);//�Ƿ���ڵ�ѹ�ȼ�
	virtual bool IsSubstationExist(const std::string& substationKey);//�Ƿ���ڱ��վ
	virtual bool GetPortsList(const std::string& IEDKey,std::vector<std::string>& portKeys);//���װ�õ�����˿��б�
	virtual std::string GetPortDescByPortKey(const std::string& portKey);//�������˿ڵ�����
	virtual bool GetLinksListOfIED(const std::string& IEDKey, std::vector<PhysLinkInfo>& links);//���װ�õ����������б�
	virtual bool GetInSignalsListOfIED(const std::string& IEDKey, SignalTypeEnum signalType,std::vector<std::string>& inSignalKeys);//���װ�õ������ź��б�
	virtual bool GetOutSignalsListOfIED(const std::string& IEDKey,  SignalTypeEnum signalType,std::vector<std::string>& outSignalKeys);//���װ�õ�����ź��б�
	virtual bool GetVritualLinksOfIED(const std::string& IEDKey,  SignalTypeEnum signalType, std::vector<VirtualLinkInfo>& links);//���װ�õ����������б�װ�ú�װ��֮�䣩
	virtual bool GetInternalVritualLinksOfIED(const std::string& IEDKey,  SignalTypeEnum signalType, std::vector<VirtualLinkInfo>& links);//���װ���ڲ������������б�װ���ڲ���
	virtual bool GetFunctionsList(const std::string& IEDKey,std::vector<std::string>& protectFuncKeys);//���װ�õĹ����б�
	virtual bool GetVirtualLinksListOfPhysicsLink(const PhysLinkInfo& phyLink,  SignalTypeEnum signalType, std::vector<VirtualLinkInfo>& links);//����������ӹ��������������б�
	virtual bool GetTripsList(const std::string& IEDKey,std::vector<std::string>& tripKeys);//���װ�õ�ѹ���б�  
	virtual bool GetInfoOfInSignal(const std::string& signalKey, SignalInfo& signalInfo);//��������źŵ���Ϣ  
	virtual bool GetInfoOfOutSignal(const std::string& signalKey, SignalInfo& signalInfo);//�������źŵ���Ϣ
	virtual bool GetInfoOfDSSettingOutSignal(const std::string& signalKey, DSSettingSignalInfo& signalInfo);

	virtual bool GetCtrlBlockListByIED(const std::string& IEDKey, std::vector<std::string>& ctrlBlockKeys);//��ȡ��ǰIED�����ݼ��б�
	virtual bool GetCtrlBlockInfo(const std::string& ctrlBlockKey, CtrlBlockInfo& cbInfo);//��ȡ���ݼ���Ϣ
	virtual bool GetDataSetSignalsByCtrlBlockID(const std::string& cbKey, std::vector<std::string>& dsSignalList);//��ȡ���ݼ����ź�
	virtual bool GetSlaveCtrlBlockByIEDKeys(const std::string& masterKey, const std::string& slaveKey, std::vector<std::string>& ctrlBlockList);//��������IED��Key��ȡ��IED�õ��Ŀ��ƿ�
	virtual bool GetOutSignalsByCBKey(const std::string& masterIEDKey, const std::string& cbKey, std::vector<std::string>& outSignals);		//ͨ�����ƿ��ȡ������;
	virtual bool GetAssoIEDListByCBKey(const std::string& cbKey, std::vector<std::string>& assoIEDKeys);		//ͨ�����ƿ��ȡ�Զ�IED;

	virtual bool GetPhyPortInfo(const std::string& pPortKey, PhyPortInfo& pPortInfo);				//��ȡ����˿���Ϣ;
	virtual bool GetPhyPortListByIED(const std::string& IEDKey, std::vector<std::string>& pPortList);		//����IED��ȡ����˿�;
	virtual bool GetPhyPortListByInSignalID(const std::string& signalKey, std::vector<std::string>& pPortList);			//�����źŻ�ȡ�˿�;
	virtual bool GetPhyPortKeyByOutSignalID(const std::string& signalKey, std::string& pPortKey);			//�����źŻ�ȡ�˿�;

	//�߼����ʽӿ�
	virtual bool GetIEDListByLogicalRealitionWithCurrentIED(const std::string& IEDKey,std::vector<std::string>& associatedIEDKeys);//���IED�Ĺ���IED�б��߼���
	virtual bool GetRealitionTypeOfSrcIEDAndDestIED(const std::string& srcIEDKey, const std::string& destIEDKey, int& type);//���IED֮��Ĺ������ͣ�0��ԴIEDΪ���룩																																												//1: ԴIEDΪ���																																											//2: ԴIED��������������																																												//-1:ԴIED��Ŀ��IEDû�й�ϵ��
	virtual bool GetIEDListByPhysicalRealitionWithCurrentIED(const std::string& IEDKey,std::vector<std::string>& associatedIEDKeys);//���IED�Ĺ���IED�б�����  
	virtual bool GetPhysicsLinkAssociatedIED(const std::string& srcIEDKey, const std::string& destIEDKey, std::vector<PhysLinkInfo>& links);//���IED��IED֮���������ӣ�������ӱ�ʾ�������ͨ����0�����ӱ�ʾ��ֱ�����ӣ�
	virtual bool GetLinksBetweenPort(const std::string& srcPortKey, const std::string& destPortKey, std::vector<PhysLinkInfo>& links);//���ԭport��Ŀ��port֮����������ӣ�������ӱ�ʾ֮���ж���豸��
	virtual bool GetVirtualSignalsAssociatedIED(const std::string& srcIEDKey, const std::string& destIEDKey,  SignalTypeEnum signalType, std::vector<VirtualLinkInfo>& links);//���IED��IED֮�����ź� 
	virtual bool GetPortsListOfPort(const std::string& portKey,std::vector<std::string>& portKeys);//���������˿ڹ�������������˿�
	virtual bool GetPhysicsLinkeByPort(const std::string& srcPortKey, const std::string& destPortKey, PhysLinkInfo& link);//���������˿ڻ���������� 
	virtual bool GetInSignalsListOfOutSignal(const std::string& outSignalKey,std::vector<std::string>& inSignalKeys);//�������źŹ����������ź��б�����װ��֮�䣩
	virtual bool GetOutSignalsListOfInSignal(const std::string& inSignalKey,std::vector<std::string>& outSignalKeys);//��������źŹ���������ź��б�����װ��֮�䣩
	virtual bool GetInternalOutSignalsListOfInSignal(const std::string& inSignalKey,std::vector<std::string>& outSignalKeys);//��������źŹ���������ź��б�װ���ڲ���
	virtual bool GetInternalInSignalsListOfOutSignal(const std::string& outSignalKey,std::vector<std::string>& inSignalKeys);//�������źŹ����������ź��б�װ���ڲ���
	virtual bool GetVirtualLinkBySignal(const std::string& srcSignalKey, const std::string& destSignalKey, VirtualLinkInfo& link);//���������źŻ���������� 
	virtual bool GetFunctionListOfInSignal(const std::string& inSignalKey,std::vector<std::string>& funcKeys);//��������źŹ����Ĺ����б���װ�ù��ܣ�
	virtual bool GetFunctionsListOfOutSignal(const std::string& outSignalKey,std::vector<std::string>& funcKeys);//�������źŹ����Ĺ����б� ������װ�ù��ܣ�
	virtual bool GetFunctionsListOfOutSignalWithDestIED(const std::string& outSignalKey, const std::string& destIEDKey,std::vector<std::string>& funcKeys);//�������ź������ĳװ�õĹ����б�ĳװ�ù��ܣ�
	virtual bool GetFunctionsListOfPort(const std::string& portKey,std::vector<std::string>& FuncKeys);//�������˿ڹ����б�
	virtual bool GetFunctionsListOfTrip(const std::string& tripKey,std::vector<std::string>& FuncKeys);//���ѹ���Ӧ�Ĺ����б�
	virtual std::string GetFunctionDesc(const std::string& functionKey);//��ù��ܵĹ�������
	virtual std::string GetFunctionTripKey(const std::string& functionKey);//��ù��ܵĹ���ѹ�壨���ܲ����ڣ�
	virtual std::string GetIEDTripKey(const std::string& IEDKey) ;//����豸��ѹ�壨��ʾ�ϵ�һ��Ĳ�����
	virtual std::string GetPortTripKey(const std::string& portKey) ;//��ö˿ڵ�ѹ�壨��ʾ������һ��Ĳ�����
	virtual std::string GetSignalTripKey(const std::string& signalKey);//����źŵ�ѹ�壨��ѹ�壩

    //ģ�ͱ༭�ӿ�
	virtual bool AddVoltageLevel(const std::string& voltageLevelName);//���ӵ�ѹ�ȼ�
	virtual bool DeleteVoltageLevel(const std::string& voltageLevelName);//ɾ����ѹ�ȼ�
	virtual bool AddSubstation(const std::string& voltageLevelKey, const std::string& subName/*, const std::vector<char>& fileData*/);//���ӱ��վģ��
	virtual int  GetSubstationAddedProgress();//��õ�ǰ�������
	virtual bool DeleteSubstation(const std::string& substationKey);//ɾ�����վģ��
	virtual bool UpdateSubstationModel(const std::string& voltageLevelKey, const std::string& subName, const std::string& SCDFilePath);//���±��վģ��
	virtual bool AddPanelForSubstation(const std::string& subKey, const std::string& paneName);//Ϊ���վ��������
	virtual bool DeletePanelForSubstation(const std::string& PanelKey);//Ϊ���վɾ������
	virtual bool AddIEDForPanel(const std::string& panelKey, const std::string& IEDName);//Ϊ��������װ��
	virtual bool DeleteIEDForPanel(const std::string& IEDKey);//Ϊ����ɾ��װ��
	virtual bool AddPortForIED(const std::string& IEDKey, const std::string& portName);//Ϊװ����������˿�
	virtual bool DeletePortForIED(const std::string& portKey);//Ϊװ��ɾ������˿�
	virtual bool AddInSignalForIED(const std::string& IEDKey, const std::string& inSignalName);//Ϊװ�����������ź�
	virtual bool DeleteInSignalForIED(const std::string& inSignalKey);//Ϊװ��ɾ�������ź�
	virtual bool AddOutSignalForIED(const std::string& IEDKey, const std::string& outSignalName); //Ϊװ����������ź�
	virtual bool DeleteOutSignalForIED(const std::string& outSignalKey);//Ϊװ��ɾ������ź�
	virtual bool AddFunction(const std::string& IEDKey, const std::string& funName, const std::string& funcDesc);//Ϊװ�����ӹ���
	virtual bool ModifyFunction(const std::string& funKey, const std::string& funcDesc);//Ϊװ���޸Ĺ�������  
	virtual bool DeleteFunction(const std::string& funKey);//Ϊװ��ɾ������ 
	virtual bool SetDescForPort(const std::string& portKey, const std::string& desc);//��������˿ڵ�����  
	virtual bool SetInSignalsListOfOutSignal(const std::string& outSignalKey, const std::vector<std::string>& inSignalKeys);//��������źŹ����������ź��б�����װ��֮�䣩
	virtual bool SetOutSignalsListOfInSignal(const std::string& inSignalKey, const std::vector<std::string>& outSignalKeys);//���������źŹ���������ź��б�����װ��֮�䣩
	virtual bool SetInternalOutSignalsListOfInSignal(const std::string& inSignalKey, const std::vector<std::string>& outSignalKeys);//���������źŹ���������ź��б�װ���ڲ���
	virtual bool SetInternalInSignalsListOfOutSignal(const std::string& outSignalKey, const std::vector<std::string>& inSignalKeys);//��������źŹ����������ź��б�װ���ڲ���
	virtual bool AddFunctionOfInSignal(const std::string& inSignalKey, const std::string& funKey);//Ϊ�����ź����ӹ��ܹ���
	virtual bool DeleteFunctionOfInSignal(const std::string& inSignalKey,const std::vector<std::string>& funcKeys);//Ϊ�����ź�ɾ�����ܹ��� 
	virtual bool AddTripForIED(const std::string& IEDKey, const std::string& tripName);//����ѹ��
	virtual bool DeleteTripForIED(const std::string& tripKey);//ɾ��ѹ�� 
	virtual bool SetFunctionTripKey(const std::string& functionKey, const std::string& tripKey);//���ù��ܵĹ���ѹ�壨���ܲ����ڣ� 
	virtual bool SetIEDTripKey(const std::string& IEDKey, const std::string& tripKey);//�����豸��ѹ�壨��ʾ�ϵ�һ��Ĳ����� 
	virtual bool SetPortTripKey(const std::string& portKey, const std::string& tripKey);//���ö˿ڵ�ѹ�壨��ʾ������һ��Ĳ�����
	virtual bool SetSignalTripKey(const std::string& signalKey, const std::string& tripKey);//�����źŵ�ѹ�壨��ѹ�壩  

	virtual bool GetSoftTripState(const std::string& tripKey, bool& state);//���ѹ��״̬
	virtual bool SetSoftTripState(const std::string& tripKey, bool state);//����ѹ��״̬

	// ȡ��������IED��AP�б�
	virtual bool GetAPListOfIEDFromSubnet(const ::std::string& subStationID, const ::std::string& subNetName, const ::std::string IEDName, std::vector<std::string> &APList);
	// ȡ��������IEDĳ��AP����������˿��б�
	virtual bool GetPortListOfIEDAPFromSubNet(const ::std::string& subStationID, const ::std::string& subNetName, const ::std::string IEDName, const ::std::string APName, std::vector<std::string> &PortList);
	// ȡ��IED��AP���б�
	virtual bool GetAPListFromIED(const ::std::string& subStationID, const ::std::string& IEDName, std::vector<std::string> &APList);
	// ȡ��IED��ĳ��AP��Ӧ������˿��б�
	virtual bool GetAPPortListFormIED(const ::std::string& subStationID, const ::std::string& IEDName, const ::std::string &APName, std::vector<std::string> &PortList);


	virtual bool GetProtectionSettingList(const ::std::string& masterIEDKey, std::vector<DataSetInfo>& dsSettingList);
	// ȡ��IED�µĶ�ֵ��Ŀ�б�
	virtual bool GetProtectionSettingSignalList(const ::std::string& masterIEDKey, const ::std::string& DataSetName, ::std::vector<std::string>& outSignalKeys);
	// ȡ�ö�ֵ��Ŀ����Ӧ���б�

    virtual bool GetAllRemainingDataSetSignalList(const ::std::string& masterIEDKey, const ::std::string& ldInst, 
        const ::std::string& DataSetName, ::std::vector<std::string>& outSignalKeys);
    virtual bool GetAllRemainingDataSetList(const ::std::string& masterIEDKey, 
        std::vector<DataSetInfo>& dsSettingList);

    virtual bool GetDataTypeTemplateInfo(const ::std::string &subStationKey, DataTypeTemplateType& dtt);
    virtual bool GetIEDListByLNodeType(const ::std::string &subStationKey, const ::std::string &lnID, ::std::vector<std::string> &assoIEDList);

    virtual bool GetSSDInfo(const ::std::string &subStationKey, SubstationCache &ssdInfo);
};

// ��ͨ�ò����࣬½��
class SCLDATAACCESSCOMPONENT_API SclDacLib
{
public:
    // ȫ�ֳ�ʼ������
    // ���������ַ�������ʹ��Ĭ��·��������dll��exe����·����
    // ���ȷ��·�����ڣ���������޷������ʵ�
    static bool Startup(const std::string &strGlobalWorkPath = "");

    // ��ȡ����Ŀ¼���Զ�������β��·���ָ���
    // lpFileMarkΪNULL��հ��ַ���ʱ������ȫ��Ŀ¼
    // lpFileMarkΪ�����ļ���ʶʱ�������ļ���ʶĿ¼
    // ���Զ�����Ŀ¼
    // ��Ŀ¼�����ڣ����ؿհ��ַ���
    static std::string GetWorkPath(const char *lpFileMark);

    // ȫ�����ٺ���
    static void Shutdown();

    // ������scd�ļ����ڲ����ܵ���LoadScdFile������
    static bool ImportScdFile(const std::string &strFilePath, std::string &strFileMark);

    // ����һ���ѵ������scd�ļ�
    static bool LoadScdFile(const std::string &strFileMark);

    // ���ڴ�ж��һ���Ѽ��ص�scd�ļ�
    static bool UnloadScdFile(const std::string &strFileMark);

    // �����ɾ��һ���ѵ����scd�ļ������Զ�����UnloadScdFile
    static bool RemoveSavedData(const std::string &strFileMark);

    // ��ȡ�ѵ����scd�ļ���ʶ�б�
    static std::vector<std::string> GetImportedFileMarks();

    // ��ȡ�Ѽ��ص�scd�ļ���ʶ�б�
    static std::vector<std::string> GetLoadedFileMarks();

    // ��scd�ļ������ļ���ʶ
    static std::string CalcFileMark(const std::string &strFilePath);

    // ���ѵ����scd�ļ����������sqlite
    static bool ExportScdFileInfomationToSqliteFile(const std::string &strFileMark, const std::string &strFilePath);

    // �����ײ㿪ʼ�����źŵĻ���ִ�У����ȴ�����
    static bool BuildSignalCache(const std::string &strFileMark);
};

#endif