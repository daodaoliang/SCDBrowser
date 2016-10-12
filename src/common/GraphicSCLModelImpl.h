#pragma once
#include "QSettings"
#include "GraphicModelInterface.h"

class CGraphicSCLModelImpl : public GraphicModelInterface
{
public:
	CGraphicSCLModelImpl(void);
	~CGraphicSCLModelImpl(void);

public:
	bool   GetBaseFileInfo(QSubStationInfo& FileInfo);//��û�׼�ļ���Ϣ;
	bool   GetOtherFileInfo(QSubStationInfo& FileInfo);//��ñ��Ƚ��ļ���Ϣ;

	bool   GetIEDList(QString panelKey, QVector<QString>* IEDKeys);//�������װ��ID;
	bool   GetIEDListBySubNetKey(QString subNetKey, QVector<QString> *subIEDs);//��ȡ����������װ��ID;
	bool   GetSubNetKeysByIEDKey(QString IEDKey, QVector<QString> *subNetKeys);//����װ�û�ȡ��������;

	bool   GetIEDInfo(QString IEDKey, QIEDInfo& IEDInfo);//���װ����Ϣ;
	bool   GetInfoOfInSignal(QString signalKey,QSignalInfo *signalInfo);                             //��������źŵ���Ϣ;
	bool   GetInfoOfOutSignal(QString signalKey,QSignalInfo *signalInfo);                            //�������źŵ���Ϣ;

	//���masterIEDKeyΪ�ؼ���IED��CBkey���ƿ鷢�͸�slaveIEDKeyΪ�ؼ���IED���������б�;
	bool   GetSignalInfosByIEDKeyAndCBKey(const QString masterIEDKey, const QString slaveIEDKey, const QString CBkey, QVector<QVirtualLink> *VirtualLinkList);
	bool   GetSignalInfosByIEDKey(const QString IEDKey, QVector<QVirtualLink> *VirtualLinkList);
	VirLinkDiff   GetVirLinkStateByIEDKeyAndCBKey(const QString masterIEDKey, const QString slaveIEDKey, const QString CBkey);
	//�������������е���ʽ;
	bool   GetSignalInfosByIEDKeyAndCBKey(const QString masterIEDKey, const QString slaveIEDKey, const QString CBkey, QList<QSignalInfo> *mSigInfos, QList<QSignalInfo> *sSigInfos);
	bool   GetSignalInfosByIEDKey(const QString IEDKey, QList<QSignalInfo> *mSigInfos, QList<QSignalInfo> *sSigInfos);

	int	   GetSignalCountByIEDKey(const QString IEDKey, QSignalTypeEnum sigType);

	bool   GetCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList);					   //��ȡ��ǰIED�����ݼ��б�;
	bool   GetCtrlBlockInfo(QString ctrlBlockKey, QCtrlBlockInfo& cbInfo);							   //��ȡ���ݼ���Ϣ;
	bool   GetSlaveCtrlBlockByIEDKeys(QString masterKey, QString slaveKey, QVector<QString>* ctrlBlockList);	//��ȡ���źŵ�IED�Ŀ��ƿ�;
	bool   GetOutSignalsByCBKey(QString masterIEDKey, QString cbKey, QVector<QString>* outSignals);		//ͨ�����ƿ��ȡ������;

	bool   GetPhyPortInfo(QString pPortKey, QPhyPortInfo& pPortInfo);				//��ȡ����˿���Ϣ;
	bool   GetPhyPortListByIED(QString IEDKey, QVector<QString>* pPortList);		//����IED��ȡ����˿�;
	bool   GetPhyPortListByInSignalID(QString signalKey, QVector<QString> &pPortList);			//���������źŻ�ȡ�˿�;
	bool   GetPhyPortKeyByOutSignalID(QString signalKey, QString* pPortKey);		//��������źŻ�ȡ�˿�;

	//IED֮�����·�Ĳ�����ڿ��ƿ���ʵ��;
	//virtual int	   GetVLinkChangeType(QString masterIEDName, QString slaveIEDName, QString cbName) = 0;//��ȡ���·��������(��Ϣ��ͼ);
	bool   GetIEDListByLogicalRealitionWithCurrentIED(QString IEDKey,QVector<QString>* associatedIEDKeys);    //���IED�Ĺ���IED�б��߼�);

private:
	QSettings *subSetting;
	QString subStationKey;
	SCLModelSvrAdapterInterface *SCLModel;
};

