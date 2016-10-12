#ifndef TABLEMODELDIFFIMPL_H
#define TABLEMODELDIFFIMPL_H

#include "TableModelInterface.h"
#include "SCDDifferCompare.h"

class TableModelDiffImpl : public  TableModelInterface
{
public:
	TableModelDiffImpl();
	~TableModelDiffImpl();
    
	virtual bool   GetBaseFileInfo(FileInfoStru& FileInfo);//��û�׼�ļ���Ϣ	
	virtual bool   GetOtherFileInfo(FileInfoStru& FileInfo);//��ñ��Ƚ��ļ���Ϣ	

	virtual bool   GetIEDList(QString panelKey, QVector<QString>* IEDKeys);//�������װ��ID			                                   
	virtual bool   GetIEDInfo(QString IEDKey, IEDStru& IEDInfo);//���װ����Ϣ

	virtual bool   GetInfoOfInSignal(QString signalKey,SignalStru *signalInfo);                             //��������źŵ���Ϣ  
	virtual bool   GetInfoOfOutSignal(QString signalKey,SignalStru *signalInfo);                            //�������źŵ���Ϣ

	virtual bool   GetGooseCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList);					   //��ȡ��ǰIED��GOOSE���ƿ��б�
	virtual bool   GetSVCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList);					   //��ȡ��ǰIED��SV���ƿ��б�

	virtual int	   GetSigCountByIED(QString IEDKey, bool IsInSig, QSignalTypeEnum sigType);			//��ȡ��ǰIED(����/���/Goose/SV)�ź�����;

	virtual bool   GetCtrlBlockInfo(QString ctrlBlockKey, SVCBStru& cbInfo);							   //��ȡSV���ƿ���Ϣ;
	virtual bool   GetCtrlBlockInfo(QString ctrlBlockKey, GOOSECBStru& cbInfo);							   //��ȡGoose���ƿ���Ϣ;
	virtual bool   GetCtrlBlockInfo(QString ctrlBlockKey, ReportCBStru& cbInfo);						   // ��ȡReport���ƿ���Ϣ 
	virtual bool   GetOutSignalsByCBKey(QString IEDKey, QString cbKey, QVector<QString>* outSignalKeys);		//ͨ�����ƿ��ȡ����ź��б�;

	virtual bool   GetInSignalsByIEDKey(QString IEDKey, QVector<QString>* inSignalKeys);		//ͨ�����ƿ��ȡ����ź��б�;
	virtual bool   GetInputsOfIED(QString IEDKey, QVector<ExtRefStru> &ExtRefs); // ȡ��IED��Inputs��Ϣ

	virtual bool   GetCtrlBlockOfOutIEDByInsignal(QString signalKey, SVCBStru* CBKey);		//ͨ�������źŻ�ȡ�Զ�IED�Ŀ��ƿ�;
	virtual bool   GetCtrlBlockOfOutIEDByInsignal(QString signalKey, GOOSECBStru* CBKey);	//ͨ�������źŻ�ȡ�Զ�IED�Ŀ��ƿ�;
	virtual bool   GetCtrlBlockOfOutIEDByOutsignal(QString signalKey, SVCBStru* CBKey);
	virtual bool   GetCtrlBlockOfOutIEDByOutsignal(QString signalKey, GOOSECBStru* CBKey);
	
	virtual bool   GetInSignalsOfOutsignal(QString outSignalKey, QVector<QString>* inSignalKeys);		//ͨ������źŻ�ȡ�Զ˵������ź��б�;

	virtual bool   GetIEDListBySubnetKey(QString subnetKey, QVector<QString> &subIEDs);		//ͨ������Key��ȡ������IED;

	virtual bool   GetApInfo(QString subnetKey, QString IEDKey, QString APName, QAPInfo &apInfo);			//ͨ������Key�Լ�IEDKey��ȡ�������Ϣ;

	virtual bool   GetAPListOfIEDFromSubnet(const QString& subNetKey, const QString IEDName, QVector<QString> &APList);// ȡ��������IED��AP�б�
	virtual bool   GetPortListOfIEDAPFromSubNet(const QString& subNetKey, const QString IEDName, const QString APName, QVector<QString> &PortList);// ȡ��������IEDĳ��AP����������˿��б�
	virtual bool   GetAPListFromIED(const QString& IEDKey, QVector<QString> &APList);// ȡ��IED��AP���б�
	virtual bool   GetAPPortListFormIED(const QString& IEDKey, const QString &APName, QVector<QString> &PortList);// ȡ��IED��ĳ��AP��Ӧ������˿��б�
public:
	SCDDiffCompare		   *m_ScdDiffCompare;

private:
	SCLNameTranslator		m_NameTranslator;
	void SetAttrValueFromBasicStru(BasicStru &DestBasic, BasicStru &SrcBasic);
	void SetAttrValueFromBasicIntStru(BasicIntStru &DestBasic, BasicIntStru &SrcBasic);
	QString NameTranslator_GetIEDKeyFromSCDDiffResult(QString qString);
};

#endif