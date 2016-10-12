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

//������SCLModelAdapter
//���ܣ���SCLModelServerImpl�ṩ��SCLģ�ͷ���ת���ɱ��ؽӿ�
//�ص㣺����ģʽ��������ģʽ

class SCLModelAdapter : public SCLModelSvrAdapterInterface
{
public:

	virtual QString GetSubstationImportTime(QString subStationKey);
	//ģ��ͨ�ýӿ�
	virtual QString GetParentKey(QString childKey);                                           //�����ӽڵ�Key��ø��ڵ�Key
	virtual QString GetParentName(QString childKey);                                          //�����ӽڵ�Key��ø��ڵ��Name
	virtual QString GetNameFromKey(QString key);                                              //����Key�������
	virtual QString GetChildKeyFromParentKeyAndChildName(QString parentKey,QString childName);//���ݸ��ڵ��Key���ӽڵ�Name����ӽڵ��Key 

	//ģ�ͻ������ʽӿ�
	virtual bool   GetVoltageLevelList(QVector<QString>* nameList);                                          //��õ�ѹ�ȼ����б�ͬKey��
	virtual bool   GetSubstationList(QString voltageLevelKey,QVector<QString>* subStationKeyList);           //��ñ��վKey�б�
	virtual bool   GetSubstationInfo(QString subStationKey, QSubStationInfo& subStationInfo);//��ñ��վ���ض�SCD�汾����Ϣ	
	virtual QString GetSubstationPrivateXMLInfo(const QString& subStationKey);//��ñ��վprivate��ǩXMLԭ��
	virtual QString GetSubstationHeaderXMLInfo(const QString& subStationKey);//��ñ��վHeader��ǩXMLԭ��
	virtual bool   GetSCDHItem(QString subStationKey, QVector<QHItemInfo>& hItemList);//���SCD��ʷ�޶���¼
	virtual bool   GetSubNetList(QString subStationKey, QVector<QString>& subNetKeyList);//�������Key�б�
	virtual bool   GetSubNetInfo(QString subNetKey, QSubNetInfo& subNetInfo);//���������Ϣ
	virtual QString GetSubNetXMLInfo(const QString& subNetKey);//�������XMLƬ��;
	virtual bool   GetAPInfo(QString subNetKey, QString IEDName, QString APName, QAPInfo& apInfo);//��÷��ʵ���Ϣ
	virtual bool   GetPanelList(QString subStationKey,QVector<QString>* subPanelKeyList);                    //�������Key�б�
	virtual bool   GetIEDList(QString panelKey,QVector<QString>* subIEDKeyList);                             //���װ��Key�б�
	virtual bool   GetIEDListBySubNet(QString subNetKey, QVector<QString>& subIEDKeyList);//���װ��Key�б�(��������)
	virtual bool   GetIEDInfo(QString IEDKey, QIEDInfo& IEDInfo);//���װ����Ϣ	 
    virtual QString GetIEDXMLInfo(QString& IEDKey);//���IED��XML��Ϣ
	virtual bool   GetIEDType(QString IEDKey,int* type);                                                     //���װ������
	virtual QString GetSCDVersion(QString subStationKey);                                                    //��õ�ǰģ�͵�SCD�ļ��汾��
	virtual bool   IsVoltageLevelExist(QString voltageLevelName);                                            //�Ƿ���ڵ�ѹ�ȼ�
	virtual bool   IsSubstationExist(QString substationKey);                                                 //�Ƿ���ڱ��վ		
	virtual bool   GetPortsList(QString IEDKey,QVector<QString>* portKeys);                                  //���װ�õ�����˿��б�
	virtual QString GetPortDescByPortKey(QString portKey);                                                   //�������˿ڵ�����
	virtual bool   GetLinksListOfIED(QString IEDKey,QVector<QPhysLink>* links);                              //���װ�õ����������б�  
	virtual bool   GetInSignalsListOfIED(QString IEDKey,QSignalTypeEnum signalType,QVector<QString>* inSignalKeys);                     //���װ�õ������ź��б�
	virtual bool   GetOutSignalsListOfIED(QString IEDKey,QSignalTypeEnum signalType,QVector<QString>* outSignalKeys);                   //���װ�õ�����ź��б�            
	virtual bool   GetVritualLinksOfIED(QString IEDKey,QSignalTypeEnum signalType,QVector<QVirtualLink>* links);                        //���װ�õ����������б�װ�ú�װ��֮�䣩
	virtual bool   GetInternalVritualLinksOfIED(QString IEDKey,QSignalTypeEnum signalType,QVector<QVirtualLink>* links);                //���װ���ڲ������������б�װ���ڲ���     
	virtual bool   GetFunctionsList(QString IEDKey,QVector<QString>* protectFuncKeys);                       //���װ�õĹ����б�
	virtual bool   GetVirtualLinksListOfPhysicsLink(QPhysLink phyLink,QSignalTypeEnum signalType,QVector<QVirtualLink>* links);         //����������ӹ��������������б�
	virtual bool   GetTripsList(QString IEDKey,QVector<QString>* tripKeys);                                  //���װ�õ�ѹ���б�
	virtual bool   GetInfoOfInSignal(QString signalKey,QSignalInfo *signalInfo);                             //��������źŵ���Ϣ  
	virtual bool   GetInfoOfOutSignal(QString signalKey,QSignalInfo *signalInfo);                            //�������źŵ���Ϣ
	virtual bool   GetInfoOfDSSettingOutSignal(QString signalKey,QDSSettingSignalInfo *signalInfo);

	virtual bool   GetSignalInfosByIEDKeyAndCBKey(const QString masterIEDKey, const QString slaveIEDKey, const QString CBkey, QList<QSignalInfo> *mSigInfos, QList<QSignalInfo> *sSigInfos);
	virtual bool   GetSignalInfosByIEDKey(const QString IEDKey, QList<QSignalInfo> *mSigInfos, QList<QSignalInfo> *sSigInfos);

	virtual bool   GetCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList);						 //��ȡ��ǰIED�����ݼ��б�
	virtual bool   GetDsSettingListByIED(QString IEDKey, QVector<QDataSetInfo>* dsSettingList);
	virtual bool   GetCtrlBlockInfo(QString ctrlBlockKey, QCtrlBlockInfo& cbInfo);								 //��ȡ���ݼ���Ϣ
	virtual bool   GetDataSetSignalsByCtrlBlockID(QString cbKey, QVector<QString>* dsSignalList);//��ȡ���ݼ����ź�
	virtual bool   GetSlaveCtrlBlockByIEDKeys(QString masterKey, QString slaveKey, QVector<QString>* ctrlBlockList);	//��������IED��Key��ȡ��IED�õ��Ŀ��ƿ�
	virtual bool   GetOutSignalsByCBKey(QString masterIEDKey, QString cbKey, QVector<QString>* outSignals);		//ͨ�����ƿ��ȡ������;
	virtual bool   GetOutSignalsByDataSetKey(QString masterIEDKey, QString dataSetKey, QVector<QString>* outSignals);
	virtual bool   GetAssoIEDListByCBKey(QString cbKey, QVector<QString>* assoIEDKeys);		//ͨ�����ƿ��ȡ�Զ�IED;

	virtual int    GetItemChangeType(QString ItemName);											//��ȡ��������
	virtual int    GetVLinkChangeType(QString masterIEDName, QString slaveIEDName, QString cbName);//��ȡ���·��������(��Ϣ��ͼ);

	virtual bool   GetPhyPortInfo(QString pPortKey, QPhyPortInfo& pPortInfo);				//��ȡ����˿���Ϣ;
	virtual bool   GetPhyPortListByIED(QString IEDKey, QVector<QString>* pPortList);		//����IED��ȡ����˿�;
	virtual bool   GetPhyPortListByInSignalID(QString signalKey, QVector<QString> &pPortList);			//�����źŻ�ȡ�˿�;
	virtual bool   GetPhyPortKeyByOutSignalID(QString signalKey, QString* pPortKey);		//��������źŻ�ȡ�˿�;

	//�߼����ʽӿ�
	virtual bool   GetIEDListByLogicalRealitionWithCurrentIED(QString IEDKey,QVector<QString>* associatedIEDKeys);    //���IED�Ĺ���IED�б��߼���
	virtual bool   GetRealitionTypeOfSrcIEDAndDestIED(QString srcIEDKey,QString destIEDKey,int *type);        //���IED֮��Ĺ������ͣ�0��ԴIEDΪ���룩
																										//1: ԴIEDΪ���
																										//2: ԴIED���������������
																										//-1:ԴIED��Ŀ��IEDû�й�ϵ
	virtual bool   GetIEDListByPhysicalRealitionWithCurrentIED(QString IEDKey,QVector<QString>* associatedIEDKeys);   //���IED�Ĺ���IED�б�����  
	virtual bool   GetPhysicsLinkAssociatedIED(QString srcIEDKey,QString destIEDKey,QVector<QPhysLink>* links);       //���IED��IED֮���������ӣ�������ӱ�ʾ�������ͨ����0�����ӱ�ʾ��ֱ�����ӣ�
	virtual bool   GetLinksBetweenPort(QString srcPortKey,QString destPortKey,QVector<QPhysLink>* links);             //���ԭport��Ŀ��port֮����������ӣ�������ӱ�ʾ֮���ж���豸��
	virtual bool   GetVirtualSignalsAssociatedIED(QString srcIEDKey,QString destIEDKey,QSignalTypeEnum signalType,QVector<QVirtualLink>* links); //���IED��IED֮�����ź�   
	virtual bool   GetPortsListOfPort(QString portKey,QVector<QString>* portKeys);                                    //���������˿ڹ�������������˿�
	virtual bool   GetPhysicsLinkeByPort(QString srcPortKey,QString destPortKey,QPhysLink* link);                      //���������˿ڻ����������   
	virtual bool   GetInSignalsListOfOutSignal(QString outSignalKey,QVector<QString>* inSignalKeys);                  //�������źŹ����������ź��б�����װ��֮�䣩   
	virtual bool   GetOutSignalsListOfInSignal(QString inSignalKey,QVector<QString>* outSignalKeys);                  //��������źŹ���������ź��б�����װ��֮�䣩 
	virtual bool   GetInternalOutSignalsListOfInSignal(QString inSignalKey,QVector<QString>* outSignalKeys);          //��������źŹ���������ź��б�װ���ڲ���
	virtual bool   GetInternalInSignalsListOfOutSignal(QString outSignalKey,QVector<QString>* inSignalKeys);          //�������źŹ����������ź��б�װ���ڲ���
	virtual bool   GetVirtualLinkBySignal(QString srcSignalKey,QString destSignalKey,QVirtualLink* link);              //���������źŻ����������        
	virtual bool   GetFunctionListOfInSignal(QString inSignalKey,QVector<QString>* funcKeys);                         //��������źŹ����Ĺ����б���װ�ù��ܣ�
	virtual bool   GetFunctionsListOfOutSignal(QString outSignalKey,QVector<QString>* funcKeys);                      //�������źŹ����Ĺ����б� ������װ�ù��ܣ�
	virtual bool   GetFunctionsListOfOutSignalWithDestIED(QString outSignalKey,QString destIEDKey,QVector<QString>* funcKeys); //�������ź������ĳװ�õĹ����б�ĳװ�ù��ܣ�  
	virtual bool   GetFunctionsListOfPort(QString portKey,QVector<QString>* FuncKeys);                                //�������˿ڹ����б�     
	virtual bool   GetFunctionsListOfTrip(QString tripKey,QVector<QString>* FuncKeys);                                //���ѹ���Ӧ�Ĺ����б�     
	virtual QString GetFunctionDesc(QString functionKey);                                                             //��ù��ܵĹ�������
	virtual QString GetFunctionTripKey(QString functionKey);                                                          //��ù��ܵĹ���ѹ�壨���ܲ����ڣ� 
	virtual QString GetIEDTripKey(QString IEDKey);                                                                    //����豸��ѹ�壨��ʾ�ϵ�һ��Ĳ����� 
	virtual QString GetPortTripKey(QString portKey);                                                                  //��ö˿ڵ�ѹ�壨��ʾ������һ��Ĳ����� 
	virtual QString GetSignalTripKey(QString signalKey);                                                              //����źŵ�ѹ�壨��ѹ�壩   


	//ģ�ͱ༭�ӿ�
	virtual bool   AddVoltageLevel(QString voltageLevelName);                                         //���ӵ�ѹ�ȼ�
	virtual bool   DeleteVoltageLevel(QString voltageLevelName);                                      //ɾ����ѹ�ȼ�
	virtual bool   AddSubstation(QString voltageLevelKey,QString subName/*,QByteArray& fileData*/);        //���ӱ��վģ��
	virtual int    GetSubstationAddedProgress();                                                      //���SCD����Ľ���
	virtual bool   DeleteSubstation(QString substationKey);                                           //ɾ�����վģ��
	virtual bool   UpdateSubstationModel(QString voltageLevelKey,QString subName,QString SCDFilePath);//���±��վģ��
	virtual bool   AddPanelForSubstation(QString subKey,QString paneName);                            //Ϊ���վ��������
	virtual bool   DeletePanelForSubstation(QString PanelKey);                                        //Ϊ���վɾ������   		
	virtual bool   AddIEDForPanel(QString panelKey,QString IEDName);                                  //Ϊ��������װ��
	virtual bool   DeleteIEDForPanel(QString IEDKey);                                                 //Ϊ����ɾ��װ��  
	virtual bool   AddPortForIED(QString IEDKey,QString portName);                                    //Ϊװ����������˿�
	virtual bool   DeletePortForIED(QString portKey);                                                 //Ϊװ��ɾ������˿�
	virtual bool   AddInSignalForIED(QString IEDKey,QString inSignalName);                            //Ϊװ�����������ź�
	virtual bool   DeleteInSignalForIED(QString inSignalKey);                                         //Ϊװ��ɾ�������ź� 
	virtual bool   AddOutSignalForIED(QString IEDKey,QString outSignalName);                          //Ϊװ����������ź�
	virtual bool   DeleteOutSignalForIED(QString outSignalKey);                                       //Ϊװ��ɾ������ź�
	virtual bool   AddFunction(QString IEDKey,QString funName,QString funcDesc);                      //Ϊװ�����ӹ��� 
	virtual bool   ModifyFunction(QString funKey,QString funcDesc);                                   //Ϊװ���޸Ĺ�������    
	virtual bool   DeleteFunction(QString funKey);                                                    //Ϊװ��ɾ������ 
	virtual bool   SetDescForPort(QString portKey,QString desc);                                      //��������˿ڵ�����     
	virtual bool   SetInSignalsListOfOutSignal(QString outSignalKey,QVector<QString>* inSignalKeys);          //��������źŹ����������ź��б�����װ��֮�䣩   
	virtual bool   SetOutSignalsListOfInSignal(QString inSignalKey,QVector<QString>* outSignalKeys);          //���������źŹ���������ź��б�����װ��֮�䣩 
	virtual bool   SetInternalOutSignalsListOfInSignal(QString inSignalKey,QVector<QString>* outSignalKeys);  //���������źŹ���������ź��б�װ���ڲ���
	virtual bool   SetInternalInSignalsListOfOutSignal(QString outSignalKey,QVector<QString>* inSignalKeys);  //��������źŹ����������ź��б�װ���ڲ���
	virtual bool   AddFunctionOfInSignal(QString inSignalKey,QString funKey);                         //Ϊ�����ź����ӹ��ܹ���  
	virtual bool   DeleteFunctionOfInSignal(QString inSignalKey,QVector<QString>* funcKeys);          //Ϊ�����ź�ɾ�����ܹ���            	
	virtual bool   AddTripForIED(QString IEDKey,QString tripName);                                     //����ѹ��  
	virtual bool   DeleteTripForIED(QString tripKey);                                                  //ɾ��ѹ��  
	virtual bool   SetFunctionTripKey(QString functionKey,QString tripKey);                            //���ù��ܵĹ���ѹ�壨���ܲ����ڣ� 
	virtual bool   SetIEDTripKey(QString IEDKey,QString tripKey);                                      //�����豸��ѹ�壨��ʾ�ϵ�һ��Ĳ����� 
	virtual bool   SetPortTripKey(QString portKey,QString tripKey);                                    //���ö˿ڵ�ѹ�壨��ʾ������һ��Ĳ����� 
	virtual bool   SetSignalTripKey(QString signalKey,QString tripKey);                                //�����źŵ�ѹ�壨��ѹ�壩   
	//ѹ��״̬
	virtual bool   GetSoftTripState(QString tripKey,bool &state);                //���ѹ��״̬
	virtual bool   SetSoftTripState(QString tripKey,bool state);     //����ѹ��״̬
	
	// ȡ��������IED��AP�б�
	virtual bool GetAPListOfIEDFromSubnet(const QString& subNetKey, const QString IEDName, QVector<QString> &APList);
	// ȡ��������IEDĳ��AP����������˿��б�
	virtual bool GetPortListOfIEDAPFromSubNet(const QString& subNetKey, const QString IEDName, const QString APName, QVector<QString> &PortList);
	// ȡ��IED��AP���б�
	virtual bool GetAPListFromIED(const QString& IEDKey, QVector<QString> &APList);
	// ȡ��IED��ĳ��AP��Ӧ������˿��б�
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
		m_CDCDescMap.insert("SPS", "����״̬");
		m_CDCDescMap.insert("DPS", "˫��״̬");
		m_CDCDescMap.insert("INS", "����״̬");
		m_CDCDescMap.insert("SIG", "״̬�ź���");
		m_CDCDescMap.insert("ACT", "�̵籣��������Ϣ");
		m_CDCDescMap.insert("ACD", "���򱣻�������Ϣ");
		m_CDCDescMap.insert("SEC", "��ȫΥ�����");
		m_CDCDescMap.insert("BCR", "�����Ƽ�������");

		m_CDCDescMap.insert("MV", "����ֵ");
		m_CDCDescMap.insert("CMV", "��������ֵ");
		m_CDCDescMap.insert("SAV", "����ֵ");
		m_CDCDescMap.insert("WYE Class", "Y�ӷ���");
		m_CDCDescMap.insert("Delta DEL", "���ӷ���");
		m_CDCDescMap.insert("SEQ", "����");
		m_CDCDescMap.insert("HVWYE", "Y�ӷ�г��ֵ��");
		m_CDCDescMap.insert("HDEL", "���ӷ�г��ֵ��");

		m_CDCDescMap.insert("SPC", "�ɿص���");
		m_CDCDescMap.insert("DPC", "�ɿ�˫��");
		m_CDCDescMap.insert("INC", "�ɿ�����״̬");
		m_CDCDescMap.insert("BSC", "�����ƿɿز�λ����Ϣ");
		m_CDCDescMap.insert("ISC", "�����ɿز�λ����Ϣ");

		m_CDCDescMap.insert("APC", "�ɿ�ģ�������Ϣ");

		m_CDCDescMap.insert("SPG", "����״̬����");
		m_CDCDescMap.insert("ING", "����״̬����");

		m_CDCDescMap.insert("ASG", "ģ�ⶨֵ");
		m_CDCDescMap.insert("CURVE", "���߶�ֵ");

		m_CDCDescMap.insert("DPL", "�豸����");
		m_CDCDescMap.insert("LPL", "�߼��ڵ�����");
		m_CDCDescMap.insert("CSD", "������״����");
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
			// SiisEnableStat(true); // ͳ�Ƹ��ӿڵķ��ʴ�����ʱ��
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
    // ������������źţ���ֹһ���źż��������������
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