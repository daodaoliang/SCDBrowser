#pragma once
#include "QSettings"
#include "TableModelInterface.h"
#include "SCLNameTranslator.h"
#include "SCLModelSvrAdapterInterface.h"

class CTableSCLModelImpl : public TableModelInterface
{
public:
	CTableSCLModelImpl(void);
	~CTableSCLModelImpl(void);

public:
	//ģ��ͨ�ýӿ�
	QString GetParentKey(QString childKey);			//�����ӽڵ�Key��ø��ڵ�Key
	QString GetParentName(QString childKey);		//�����ӽڵ�Key��ø��ڵ��Name
	QString GetNameFromKey(QString key);			//����Key�������

	bool   GetBaseFileInfo(FileInfoStru& FileInfo);//��û�׼�ļ���Ϣ;
	bool   GetOtherFileInfo(FileInfoStru& FileInfo);//��ñ��Ƚ��ļ���Ϣ;

	bool   GetIEDList(QString panelKey, QVector<QString>* IEDKeys);//�������װ��ID;
	bool   GetIEDInfo(QString IEDKey, IEDStru& IEDInfo);//���װ����Ϣ;

	bool   GetInfoOfInSignal(QString signalKey,SignalStru *signalInfo);//��������źŵ���Ϣ;
	bool   GetInfoOfOutSignal(QString signalKey,SignalStru *signalInfo);//�������źŵ���Ϣ;
	bool   GetInfoOfDSSettingOutSignal(QString signalKey,DSSettingSignalStru *signalInfo);

	bool   GetGooseCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList);//��ȡ��ǰIED��GOOSE���ƿ��б�;
	bool   GetSVCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList);//��ȡ��ǰIED��SV���ƿ��б�;
	bool   GetReportCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList);
	bool   GetDsSettingListByIED(QString IEDKey, QVector<QDataSetInfo>* dsSettingList);

	int	   GetSigCountByIED(QString IEDKey, bool IsInSig, QSignalTypeEnum sigType);//��ȡ��ǰIED(����/���/Goose/SV)�ź�����;

	bool   GetCtrlBlockInfo(QString ctrlBlockKey, SVCBStru& cbInfo);//��ȡSV���ƿ���Ϣ;
	bool   GetCtrlBlockInfo(QString ctrlBlockKey, GOOSECBStru& cbInfo);//��ȡGoose���ƿ���Ϣ;
	bool   GetCtrlBlockInfo(QString ctrlBlockKey, ReportCBStru& cbInfo); //��ȡReport���ƿ���Ϣ;
	bool   GetOutSignalsByCBKey(QString IEDKey, QString cbKey, QVector<QString>* outSignalKeys);//ͨ�����ƿ��ȡ����ź��б�;
	bool   GetOutSignalsByDataSetKey(QString IEDKey, QString cbKey, QVector<QString>* outSignalKeys);

	bool   GetInSignalsByIEDKey(QString IEDKey, QVector<QString>* inSignalKeys);//ͨ�����ƿ��ȡ����ź��б�;
	bool   GetInputsOfIED(QString IEDKey, QVector<ExtRefStru> &ExtRefs); // ȡ��IED��Inputs��Ϣ

	bool   GetCtrlBlockOfOutIEDByInsignal(QString signalKey, SVCBStru* CBKey);		//ͨ�������źŻ�ȡ�Զ�IED�Ŀ��ƿ�;
	bool   GetCtrlBlockOfOutIEDByInsignal(QString signalKey, GOOSECBStru* CBKey);	//ͨ�������źŻ�ȡ�Զ�IED�Ŀ��ƿ�;
	bool   GetCtrlBlockOfOutIEDByOutsignal(QString signalKey, SVCBStru* CBKey);
	bool   GetCtrlBlockOfOutIEDByOutsignal(QString signalKey, GOOSECBStru* CBKey);

	bool   GetInSignalsOfOutsignal(QString outSignalKey, QVector<QString>* inSignalKeys);//ͨ������źŻ�ȡ�Զ˵������ź��б�;
	bool   GetOutSignalsOfInsignal(QString outSignalKey, QVector<QString>* inSignalKeys);//ͨ�������źŻ�ȡ�Զ˵�����ź��б�; //chenxin add 2014.10.21

	bool   GetIEDListBySubnetKey(QString subnetKey, QVector<QString> &subIEDs);		//ͨ������Key��ȡ������IED;

	bool   GetApInfo(QString subnetKey, QString IEDKey, QString APName, QAPInfo &apInfo);			//ͨ������Key�Լ�IEDKey��ȡ�������Ϣ;

	bool   GetAPListOfIEDFromSubnet(const QString& subNetKey, const QString IEDName, QVector<QString> &APList);// ȡ��������IED��AP�б�
	bool   GetPortListOfIEDAPFromSubNet(const QString& subNetKey, const QString IEDName, const QString APName, QVector<QString> &PortList);// ȡ��������IEDĳ��AP����������˿��б�
	bool   GetAPListFromIED(const QString& IEDKey, QVector<QString> &APList);// ȡ��IED��AP���б�
	bool   GetAPPortListFormIED(const QString& IEDKey, const QString &APName, QVector<QString> &PortList);// ȡ��IED��ĳ��AP��Ӧ������˿��б�

private:
	QString GetAssoCBKeyBySignal(const QString sigKey);	//���������źŻ�ȡ�Զ˿��ƿ�;

private:
	QSettings *subSetting;
	QString subStationKey;
	SCLModelSvrAdapterInterface *SCLModel;

	SCLNameTranslator			  m_NameTranslator;
};

