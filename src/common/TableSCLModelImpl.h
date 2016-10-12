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
	//模型通用接口
	QString GetParentKey(QString childKey);			//根据子节点Key获得父节点Key
	QString GetParentName(QString childKey);		//根据子节点Key获得父节点的Name
	QString GetNameFromKey(QString key);			//根据Key获得名称

	bool   GetBaseFileInfo(FileInfoStru& FileInfo);//获得基准文件信息;
	bool   GetOtherFileInfo(FileInfoStru& FileInfo);//获得被比较文件信息;

	bool   GetIEDList(QString panelKey, QVector<QString>* IEDKeys);//获得所有装置ID;
	bool   GetIEDInfo(QString IEDKey, IEDStru& IEDInfo);//获得装置信息;

	bool   GetInfoOfInSignal(QString signalKey,SignalStru *signalInfo);//获得输入信号的信息;
	bool   GetInfoOfOutSignal(QString signalKey,SignalStru *signalInfo);//获得输出信号的信息;
	bool   GetInfoOfDSSettingOutSignal(QString signalKey,DSSettingSignalStru *signalInfo);

	bool   GetGooseCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList);//获取当前IED的GOOSE控制块列表;
	bool   GetSVCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList);//获取当前IED的SV控制块列表;
	bool   GetReportCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList);
	bool   GetDsSettingListByIED(QString IEDKey, QVector<QDataSetInfo>* dsSettingList);

	int	   GetSigCountByIED(QString IEDKey, bool IsInSig, QSignalTypeEnum sigType);//获取当前IED(输入/输出/Goose/SV)信号数量;

	bool   GetCtrlBlockInfo(QString ctrlBlockKey, SVCBStru& cbInfo);//获取SV控制块信息;
	bool   GetCtrlBlockInfo(QString ctrlBlockKey, GOOSECBStru& cbInfo);//获取Goose控制块信息;
	bool   GetCtrlBlockInfo(QString ctrlBlockKey, ReportCBStru& cbInfo); //获取Report控制块信息;
	bool   GetOutSignalsByCBKey(QString IEDKey, QString cbKey, QVector<QString>* outSignalKeys);//通过控制块获取输出信号列表;
	bool   GetOutSignalsByDataSetKey(QString IEDKey, QString cbKey, QVector<QString>* outSignalKeys);

	bool   GetInSignalsByIEDKey(QString IEDKey, QVector<QString>* inSignalKeys);//通过控制块获取输出信号列表;
	bool   GetInputsOfIED(QString IEDKey, QVector<ExtRefStru> &ExtRefs); // 取得IED的Inputs信息

	bool   GetCtrlBlockOfOutIEDByInsignal(QString signalKey, SVCBStru* CBKey);		//通过输入信号获取对端IED的控制块;
	bool   GetCtrlBlockOfOutIEDByInsignal(QString signalKey, GOOSECBStru* CBKey);	//通过输入信号获取对端IED的控制块;
	bool   GetCtrlBlockOfOutIEDByOutsignal(QString signalKey, SVCBStru* CBKey);
	bool   GetCtrlBlockOfOutIEDByOutsignal(QString signalKey, GOOSECBStru* CBKey);

	bool   GetInSignalsOfOutsignal(QString outSignalKey, QVector<QString>* inSignalKeys);//通过输出信号获取对端的输入信号列表;
	bool   GetOutSignalsOfInsignal(QString outSignalKey, QVector<QString>* inSignalKeys);//通过输入信号获取对端的输出信号列表; //chenxin add 2014.10.21

	bool   GetIEDListBySubnetKey(QString subnetKey, QVector<QString> &subIEDs);		//通过子网Key获取子网内IED;

	bool   GetApInfo(QString subnetKey, QString IEDKey, QString APName, QAPInfo &apInfo);			//通过子网Key以及IEDKey获取接入点信息;

	bool   GetAPListOfIEDFromSubnet(const QString& subNetKey, const QString IEDName, QVector<QString> &APList);// 取得子网下IED的AP列表
	bool   GetPortListOfIEDAPFromSubNet(const QString& subNetKey, const QString IEDName, const QString APName, QVector<QString> &PortList);// 取得子网下IED某个AP包含的物理端口列表
	bool   GetAPListFromIED(const QString& IEDKey, QVector<QString> &APList);// 取得IED下AP的列表
	bool   GetAPPortListFormIED(const QString& IEDKey, const QString &APName, QVector<QString> &PortList);// 取得IED下某个AP对应的物理端口列表

private:
	QString GetAssoCBKeyBySignal(const QString sigKey);	//根据输入信号获取对端控制块;

private:
	QSettings *subSetting;
	QString subStationKey;
	SCLModelSvrAdapterInterface *SCLModel;

	SCLNameTranslator			  m_NameTranslator;
};

