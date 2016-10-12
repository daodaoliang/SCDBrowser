#ifndef GRAPHICMODELDIFFIMPL_H
#define GRAPHICMODELDIFFIMPL_H

#include "GraphicModelInterface.h"
#include "SCLModelSvrAdapterInterface.h"
#include "SCDDiffCommon.h"
#include "SCDDifferCompare.h"
#include "SCLModelSvrAdapter.h"

class GraphicModelDiffImpl : public GraphicModelInterface
{
public:
	GraphicModelDiffImpl(SCDDiffCompare *scdDiffer);
	~GraphicModelDiffImpl();

	bool   GetBaseFileInfo(QSubStationInfo& FileInfo);//��û�׼�ļ���Ϣ	
	bool   GetOtherFileInfo(QSubStationInfo& FileInfo);//��ñ��Ƚ��ļ���Ϣ	

	bool   GetIEDList(QString panelKey, QVector<QString>* IEDKeys);//�������װ��ID
	bool   GetIEDListBySubNetKey(QString subNetKey, QVector<QString> *subIEDs);//��ȡ����������װ��ID;
	bool   GetSubNetKeysByIEDKey(QString IEDKey, QVector<QString> *subNetKeys);//����װ�û�ȡ��������;

	bool   GetIEDInfo(QString IEDKey, QIEDInfo& IEDInfo);//���װ����Ϣ	
	bool   GetInfoOfInSignal(QString signalKey,QSignalInfo *signalInfo);                             //��������źŵ���Ϣ  
	bool   GetInfoOfOutSignal(QString signalKey,QSignalInfo *signalInfo);                            //�������źŵ���Ϣ

	//���masterIEDKeyΪ�ؼ���IED��CBkey���ƿ鷢�͸�slaveIEDKeyΪ�ؼ���IED���������б�
	bool   GetSignalInfosByIEDKeyAndCBKey(const QString masterIEDKey, const QString slaveIEDKey, const QString CBkey, QVector<QVirtualLink> *VirtualLinkList);
	VirLinkDiff   GetVirLinkStateByIEDKeyAndCBKey(const QString masterIEDKey, const QString slaveIEDKey, const QString CBkey);
	bool   GetSignalInfosByIEDKey(const QString IEDKey, QVector<QVirtualLink> *VirtualLinkList);
	int	   GetSignalCountByIEDKey(const QString IEDKey, QSignalTypeEnum sigType);

	bool   GetCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList);					   //��ȡ��ǰIED�����ݼ��б�
	bool   GetCtrlBlockInfo(QString ctrlBlockKey, QCtrlBlockInfo& cbInfo);							   //��ȡ���ݼ���Ϣ
	bool   GetSlaveCtrlBlockByIEDKeys(QString masterKey, QString slaveKey, QVector<QString>* ctrlBlockList);	//��ȡ���źŵ�IED�Ŀ��ƿ�;
	bool   GetOutSignalsByCBKey(QString masterIEDKey, QString cbKey, QVector<QString>* outSignals);		//ͨ�����ƿ��ȡ������;

	bool   GetPhyPortInfo(QString pPortKey, QPhyPortInfo& pPortInfo);				//��ȡ����˿���Ϣ;
	bool   GetPhyPortListByIED(QString IEDKey, QVector<QString>* pPortList);		//����IED��ȡ����˿�;
	bool   GetPhyPortListByInSignalID(QString signalKey, QVector<QString> &pPortList);			//���������źŻ�ȡ�˿�;
	bool   GetPhyPortKeyByOutSignalID(QString signalKey, QString* pPortKey);		//��������źŻ�ȡ�˿�;

	//IED֮�����·�Ĳ�����ڿ��ƿ���ʵ��
	bool   GetIEDListByLogicalRealitionWithCurrentIED(QString IEDKey,QVector<QString>* associatedIEDKeys);    //���IED�Ĺ���IED�б��߼���

public:
	SCDDiffCompare					*m_SCDDiffCompare;
	SCLModelSvrAdapterInterface		*SCLModel;
	SCLNameTranslator				m_NameTranslator;

private:
	void SetAttributeValueFromBasicStru(QString &DestString, BasicStru &SrcStru);
	QString NameTranslator_GetKeyNameWithoutSubStation(QString qString);
	QString NameTranslator_GetIEDKeyFromSCDDiffResult(QString qString);

	QString SetIEDDiffDesc(IEDStru *pIED);
	QString SetSignalDiffDesc(SignalStru *pSignal);
	QString SetGooseDiffDesc(GOOSECBStru *pGooseCB);
	QString SetSVDiffDesc(SVCBStru *pSVCB);
	QString SetPhyPortDiffDesc(PhysicsPortStru* pPhyPort);
};

#endif