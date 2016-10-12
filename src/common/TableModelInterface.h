#ifndef TABLEMODELINTERFACE_H
#define TABLEMODELINTERFACE_H

#include <QString>
#include <QVector>
#include "SCDDiffCommon.h"

//�ӿ����ƣ�TableModelInterface;
//�ӿ����ã���װ�˱������Ҫ��ģ����Ϣ;
//�ӿ�˵��������ʵ��ͼ�λ��ƺͶ�������չʾ�Ŀͻ�����˵���ýӿ���Ҫ�����˲��컯ģ�ͺͷǲ��컯ģ�͵Ĳ���;
            /*
            ע1���ýӿ�ͨ�������Key��Ψһ��ʶ���󣬲���ͨ������Key����ȡ��������ԡ����У����ڷǲ��컯ģ����˵;
            �����Key��ʵʱ���е�Key��ȫһ�£����ڲ��컯ģ����˵������Key���ڷǲ��컯Key�Ļ�����ȥ���˱��վName;
            ��Ϣ��;
            
            ע2���ýӿڷ��ؿ��ƿ���Ϣʱ����GOOSE���ƿ��SV���ƿ�;
            
            ע3���ýӿڷ��ص����ж�������Զ������˻�׼�ļ��е�ֵ�ͱ��Ƚ��ļ��е�ֵ;
            
            ע4����׼�ļ��뱻�Ƚ��ļ��Ĳ������չʾ������Ҫ���ǵ�;
            
            ע5�����еĿͻ������ݽṹ��QIEDInfo��QPhyPortInfo�ȶ�����״̬λ����ʾ���ӡ�ɾ�����޸ļ�δ�ı�;
            
            ע6���Ƚ�ģ����Ҫ��������˿���Ϣ;
            
            ע7����Ҫ�������չʾ���컯���ļ�����;
            */

class TableModelInterface
{
public:
	virtual bool   GetBaseFileInfo(FileInfoStru& FileInfo) = 0;//��û�׼�ļ���Ϣ;
	virtual bool   GetOtherFileInfo(FileInfoStru& FileInfo) = 0;//��ñ��Ƚ��ļ���Ϣ;
				
	virtual bool   GetIEDList(QString panelKey, QVector<QString>* IEDKeys) = 0;//�������װ��ID;
	virtual bool   GetIEDInfo(QString IEDKey, IEDStru& IEDInfo) = 0;//���װ����Ϣ;
		
	virtual bool   GetInfoOfInSignal(QString signalKey,SignalStru *signalInfo) = 0;						//��������źŵ���Ϣ;
	virtual bool   GetInfoOfOutSignal(QString signalKey,SignalStru *signalInfo) = 0;					//�������źŵ���Ϣ;
	
	virtual bool   GetGooseCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList) = 0;		//��ȡ��ǰIED��GOOSE���ƿ��б�;
	virtual bool   GetSVCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList) = 0;		//��ȡ��ǰIED��SV���ƿ��б�;

	virtual int	   GetSigCountByIED(QString IEDKey, bool IsInSig, QSignalTypeEnum sigType) = 0;			//��ȡ��ǰIED(����/���/Goose/SV)�ź�����;
		
	virtual bool   GetCtrlBlockInfo(QString ctrlBlockKey, SVCBStru& cbInfo) = 0;						//��ȡSV���ƿ���Ϣ;
	virtual bool   GetCtrlBlockInfo(QString ctrlBlockKey, GOOSECBStru& cbInfo) = 0;						//��ȡGoose���ƿ���Ϣ;
	virtual bool   GetOutSignalsByCBKey(QString IEDKey, QString cbKey, QVector<QString>* outSignalKeys) = 0;		//ͨ�����ƿ��ȡ����ź��б�;
	
	virtual bool   GetInSignalsByIEDKey(QString IEDKey, QVector<QString>* inSignalKeys) = 0;		//ͨ�����ƿ��ȡ����ź��б�;

	virtual bool   GetInputsOfIED(QString IEDKey, QVector<ExtRefStru> &ExtRefs) = 0;		// ȡ��IED��Inputs��Ϣ
	
	virtual bool   GetCtrlBlockOfOutIEDByInsignal(QString signalKey, SVCBStru* CBKey) = 0;		//ͨ�������źŻ�ȡ�Զ�IED�Ŀ��ƿ�;
	virtual bool   GetCtrlBlockOfOutIEDByInsignal(QString signalKey, GOOSECBStru* CBKey) = 0;	//ͨ�������źŻ�ȡ�Զ�IED�Ŀ��ƿ�;

	virtual bool   GetCtrlBlockOfOutIEDByOutsignal(QString signalKey, SVCBStru* CBKey) = 0;
	virtual bool   GetCtrlBlockOfOutIEDByOutsignal(QString signalKey, GOOSECBStru* CBKey) = 0;

	virtual bool   GetInSignalsOfOutsignal(QString outSignalKey, QVector<QString>* inSignalKeys) = 0;		//ͨ������źŻ�ȡ�Զ˵������ź��б�;

	virtual bool   GetIEDListBySubnetKey(QString subnetKey, QVector<QString> &subIEDs) = 0;		//ͨ������Key��ȡ������IED;

	virtual bool   GetApInfo(QString subnetKey, QString IEDKey, QString APName, QAPInfo &apInfo) = 0;			//ͨ������Key�Լ�IEDKey��ȡ�������Ϣ;

	virtual bool GetAPListOfIEDFromSubnet(const QString& subNetKey, const QString IEDName, QVector<QString> &APList) = 0;// ȡ��������IED��AP�б�
	virtual bool GetPortListOfIEDAPFromSubNet(const QString& subNetKey, const QString IEDName, const QString APName, QVector<QString> &PortList) = 0;// ȡ��������IEDĳ��AP����������˿��б�
	virtual bool GetAPListFromIED(const QString& IEDKey, QVector<QString> &APList) = 0;// ȡ��IED��AP���б�
	virtual bool GetAPPortListFormIED(const QString& IEDKey, const QString &APName, QVector<QString> &PortList) = 0;// ȡ��IED��ĳ��AP��Ӧ������˿��б�
};
#endif