#ifndef GRAPHICMODELINTERFACE_H
#define GRAPHICMODELINTERFACE_H

#include <QString>
#include <QVector>
#include "SCLModelSvrAdapterInterface.h"
#include "SCDDiffCommon.h"

//�ӿ����ƣ�GraphicModelInterface;
//�ӿ����ã���װ�˻���ͼ������Ҫ��ģ����Ϣ;
//�ӿ�˵��������ʵ��ͼ�λ��ƺͶ�������չʾ�Ŀͻ�����˵���ýӿ���Ҫ�����˲��컯ģ�ͺͷǲ��컯ģ�͵Ĳ���;
            /*
            ע1���ýӿ�ͨ�������Key��Ψһ��ʶ���󣬲���ͨ������Key����ȡ��������ԡ����У����ڷǲ��컯ģ����˵;
            �����Key��ʵʱ���е�Key��ȫһ�£����ڲ��컯ģ����˵������Key���ڷǲ��컯Key�Ļ�����ȥ���˱��վName;
            ��Ϣ��;
            
            ע2�����еĿͻ������ݽṹ��QIEDInfo��QPhyPortInfo�ȶ�����״̬λ����ʾ���ӡ�ɾ�����޸ļ�δ�ı�;
            
            ע3���Ƚ�ģ����Ҫ��������˿���Ϣ;
            
            ע4���Ƚ�ģ����Ҫ����map(����ID��ģ��ID)���������;
            
            ע5���Ƚ�ģ����Ҫ�ڸ�����Ľṹ�������Ӷ���ID;
            
            ע6���Ƚ�ģ����չ�˲��ֲ�����Ƚϣ�������Ҫ��ʾ������;
            */

class GraphicModelInterface
{
public:
	virtual bool   GetBaseFileInfo(QSubStationInfo& FileInfo) = 0;//��û�׼�ļ���Ϣ;
	virtual bool   GetOtherFileInfo(QSubStationInfo& FileInfo) = 0;//��ñ��Ƚ��ļ���Ϣ;
				
	virtual bool   GetIEDList(QString panelKey, QVector<QString>* IEDKeys) = 0;//�������װ��ID;
	virtual bool   GetIEDListBySubNetKey(QString subNetKey, QVector<QString> *subIEDs) = 0;//��ȡ����������װ��ID;
	virtual bool   GetSubNetKeysByIEDKey(QString IEDKey, QVector<QString> *subNetKeys) = 0;//����װ�û�ȡ��������;

	virtual bool   GetIEDInfo(QString IEDKey, QIEDInfo& IEDInfo) = 0;//���װ����Ϣ;
	virtual bool   GetInfoOfInSignal(QString signalKey,QSignalInfo *signalInfo) = 0;//��������źŵ���Ϣ;
	virtual bool   GetInfoOfOutSignal(QString signalKey,QSignalInfo *signalInfo) = 0;//�������źŵ���Ϣ;
	
	//���masterIEDKeyΪ�ؼ���IED��CBkey���ƿ鷢�͸�slaveIEDKeyΪ�ؼ���IED���������б�;
	virtual bool   GetSignalInfosByIEDKeyAndCBKey(const QString masterIEDKey, const QString slaveIEDKey, const QString CBkey, QVector<QVirtualLink> *VirtualLinkList) = 0;
	virtual VirLinkDiff   GetVirLinkStateByIEDKeyAndCBKey(const QString masterIEDKey, const QString slaveIEDKey, const QString CBkey) = 0;
	
	virtual bool   GetSignalInfosByIEDKey(const QString IEDKey, QVector<QVirtualLink> *VirtualLinkList) = 0;
	virtual int	   GetSignalCountByIEDKey(const QString IEDKey, QSignalTypeEnum sigType) = 0;

	virtual bool   GetCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList) = 0;//��ȡ��ǰIED�����ݼ��б�;
	virtual bool   GetCtrlBlockInfo(QString ctrlBlockKey, QCtrlBlockInfo& cbInfo) = 0;//��ȡ���ݼ���Ϣ;
	virtual bool   GetSlaveCtrlBlockByIEDKeys(QString masterKey, QString slaveKey, QVector<QString>* ctrlBlockList) = 0;//��ȡ���źŵ�IED�Ŀ��ƿ�;
	virtual bool   GetOutSignalsByCBKey(QString masterIEDKey, QString cbKey, QVector<QString>* outSignals) = 0;//ͨ�����ƿ��ȡ������;

	virtual bool   GetPhyPortInfo(QString pPortKey, QPhyPortInfo& pPortInfo) = 0;//��ȡ����˿���Ϣ;
	virtual bool   GetPhyPortListByIED(QString IEDKey, QVector<QString>* pPortList) = 0;//����IED��ȡ����˿�;
	virtual bool   GetPhyPortListByInSignalID(QString signalKey, QVector<QString> &pPortList) = 0;//���������źŻ�ȡ�˿�;
	virtual bool   GetPhyPortKeyByOutSignalID(QString signalKey, QString* pPortKey) = 0;//��������źŻ�ȡ�˿�;

  //IED֮�����·�Ĳ�����ڿ��ƿ���ʵ��;
	//virtual int	   GetVLinkChangeType(QString masterIEDName, QString slaveIEDName, QString cbName) = 0;//��ȡ���·��������(��Ϣ��ͼ);
	virtual bool   GetIEDListByLogicalRealitionWithCurrentIED(QString IEDKey,QVector<QString>* associatedIEDKeys) = 0;    //���IED�Ĺ���IED�б��߼�);
};

#endif	//GRAPHICMODELINTERFACE_H