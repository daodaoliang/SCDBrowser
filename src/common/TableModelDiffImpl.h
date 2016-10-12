#ifndef TABLEMODELDIFFIMPL_H
#define TABLEMODELDIFFIMPL_H

#include "TableModelInterface.h"
#include "SCDDifferCompare.h"

class TableModelDiffImpl : public  TableModelInterface
{
public:
	TableModelDiffImpl();
	~TableModelDiffImpl();
    
	virtual bool   GetBaseFileInfo(FileInfoStru& FileInfo);//获得基准文件信息	
	virtual bool   GetOtherFileInfo(FileInfoStru& FileInfo);//获得被比较文件信息	

	virtual bool   GetIEDList(QString panelKey, QVector<QString>* IEDKeys);//获得所有装置ID			                                   
	virtual bool   GetIEDInfo(QString IEDKey, IEDStru& IEDInfo);//获得装置信息

	virtual bool   GetInfoOfInSignal(QString signalKey,SignalStru *signalInfo);                             //获得输入信号的信息  
	virtual bool   GetInfoOfOutSignal(QString signalKey,SignalStru *signalInfo);                            //获得输出信号的信息

	virtual bool   GetGooseCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList);					   //获取当前IED的GOOSE控制块列表
	virtual bool   GetSVCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList);					   //获取当前IED的SV控制块列表

	virtual int	   GetSigCountByIED(QString IEDKey, bool IsInSig, QSignalTypeEnum sigType);			//获取当前IED(输入/输出/Goose/SV)信号数量;

	virtual bool   GetCtrlBlockInfo(QString ctrlBlockKey, SVCBStru& cbInfo);							   //获取SV控制块信息;
	virtual bool   GetCtrlBlockInfo(QString ctrlBlockKey, GOOSECBStru& cbInfo);							   //获取Goose控制块信息;
	virtual bool   GetCtrlBlockInfo(QString ctrlBlockKey, ReportCBStru& cbInfo);						   // 获取Report控制块信息 
	virtual bool   GetOutSignalsByCBKey(QString IEDKey, QString cbKey, QVector<QString>* outSignalKeys);		//通过控制块获取输出信号列表;

	virtual bool   GetInSignalsByIEDKey(QString IEDKey, QVector<QString>* inSignalKeys);		//通过控制块获取输出信号列表;
	virtual bool   GetInputsOfIED(QString IEDKey, QVector<ExtRefStru> &ExtRefs); // 取得IED的Inputs信息

	virtual bool   GetCtrlBlockOfOutIEDByInsignal(QString signalKey, SVCBStru* CBKey);		//通过输入信号获取对端IED的控制块;
	virtual bool   GetCtrlBlockOfOutIEDByInsignal(QString signalKey, GOOSECBStru* CBKey);	//通过输入信号获取对端IED的控制块;
	virtual bool   GetCtrlBlockOfOutIEDByOutsignal(QString signalKey, SVCBStru* CBKey);
	virtual bool   GetCtrlBlockOfOutIEDByOutsignal(QString signalKey, GOOSECBStru* CBKey);
	
	virtual bool   GetInSignalsOfOutsignal(QString outSignalKey, QVector<QString>* inSignalKeys);		//通过输出信号获取对端的输入信号列表;

	virtual bool   GetIEDListBySubnetKey(QString subnetKey, QVector<QString> &subIEDs);		//通过子网Key获取子网内IED;

	virtual bool   GetApInfo(QString subnetKey, QString IEDKey, QString APName, QAPInfo &apInfo);			//通过子网Key以及IEDKey获取接入点信息;

	virtual bool   GetAPListOfIEDFromSubnet(const QString& subNetKey, const QString IEDName, QVector<QString> &APList);// 取得子网下IED的AP列表
	virtual bool   GetPortListOfIEDAPFromSubNet(const QString& subNetKey, const QString IEDName, const QString APName, QVector<QString> &PortList);// 取得子网下IED某个AP包含的物理端口列表
	virtual bool   GetAPListFromIED(const QString& IEDKey, QVector<QString> &APList);// 取得IED下AP的列表
	virtual bool   GetAPPortListFormIED(const QString& IEDKey, const QString &APName, QVector<QString> &PortList);// 取得IED下某个AP对应的物理端口列表
public:
	SCDDiffCompare		   *m_ScdDiffCompare;

private:
	SCLNameTranslator		m_NameTranslator;
	void SetAttrValueFromBasicStru(BasicStru &DestBasic, BasicStru &SrcBasic);
	void SetAttrValueFromBasicIntStru(BasicIntStru &DestBasic, BasicIntStru &SrcBasic);
	QString NameTranslator_GetIEDKeyFromSCDDiffResult(QString qString);
};

#endif