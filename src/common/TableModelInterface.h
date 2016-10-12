#ifndef TABLEMODELINTERFACE_H
#define TABLEMODELINTERFACE_H

#include <QString>
#include <QVector>
#include "SCDDiffCommon.h"

//接口名称：TableModelInterface;
//接口作用：封装了表格所需要的模型信息;
//接口说明：对于实现图形绘制和对象属性展示的客户端来说，该接口主要屏蔽了差异化模型和非差异化模型的差异;
            /*
            注1：该接口通过对象的Key来唯一标识对象，并且通过对象Key来获取对象的属性。其中，对于非差异化模型来说;
            其对象Key与实时库中的Key完全一致，对于差异化模型来说，对象Key是在非差异化Key的基础上去掉了变电站Name;
            信息。;
            
            注2：该接口返回控制块信息时区分GOOSE控制块和SV控制块;
            
            注3：该接口返回的所有对象的属性都包括了基准文件中的值和被比较文件中的值;
            
            注4：基准文件与被比较文件的差异如何展示，是需要考虑的;
            
            注5：所有的客户端数据结构如QIEDInfo、QPhyPortInfo等都加上状态位，表示增加、删除、修改及未改变;
            
            注6：比较模型需要增加物理端口信息;
            
            注7：需要考虑如何展示差异化的文件属性;
            */

class TableModelInterface
{
public:
	virtual bool   GetBaseFileInfo(FileInfoStru& FileInfo) = 0;//获得基准文件信息;
	virtual bool   GetOtherFileInfo(FileInfoStru& FileInfo) = 0;//获得被比较文件信息;
				
	virtual bool   GetIEDList(QString panelKey, QVector<QString>* IEDKeys) = 0;//获得所有装置ID;
	virtual bool   GetIEDInfo(QString IEDKey, IEDStru& IEDInfo) = 0;//获得装置信息;
		
	virtual bool   GetInfoOfInSignal(QString signalKey,SignalStru *signalInfo) = 0;						//获得输入信号的信息;
	virtual bool   GetInfoOfOutSignal(QString signalKey,SignalStru *signalInfo) = 0;					//获得输出信号的信息;
	
	virtual bool   GetGooseCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList) = 0;		//获取当前IED的GOOSE控制块列表;
	virtual bool   GetSVCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList) = 0;		//获取当前IED的SV控制块列表;

	virtual int	   GetSigCountByIED(QString IEDKey, bool IsInSig, QSignalTypeEnum sigType) = 0;			//获取当前IED(输入/输出/Goose/SV)信号数量;
		
	virtual bool   GetCtrlBlockInfo(QString ctrlBlockKey, SVCBStru& cbInfo) = 0;						//获取SV控制块信息;
	virtual bool   GetCtrlBlockInfo(QString ctrlBlockKey, GOOSECBStru& cbInfo) = 0;						//获取Goose控制块信息;
	virtual bool   GetOutSignalsByCBKey(QString IEDKey, QString cbKey, QVector<QString>* outSignalKeys) = 0;		//通过控制块获取输出信号列表;
	
	virtual bool   GetInSignalsByIEDKey(QString IEDKey, QVector<QString>* inSignalKeys) = 0;		//通过控制块获取输出信号列表;

	virtual bool   GetInputsOfIED(QString IEDKey, QVector<ExtRefStru> &ExtRefs) = 0;		// 取得IED的Inputs信息
	
	virtual bool   GetCtrlBlockOfOutIEDByInsignal(QString signalKey, SVCBStru* CBKey) = 0;		//通过输入信号获取对端IED的控制块;
	virtual bool   GetCtrlBlockOfOutIEDByInsignal(QString signalKey, GOOSECBStru* CBKey) = 0;	//通过输入信号获取对端IED的控制块;

	virtual bool   GetCtrlBlockOfOutIEDByOutsignal(QString signalKey, SVCBStru* CBKey) = 0;
	virtual bool   GetCtrlBlockOfOutIEDByOutsignal(QString signalKey, GOOSECBStru* CBKey) = 0;

	virtual bool   GetInSignalsOfOutsignal(QString outSignalKey, QVector<QString>* inSignalKeys) = 0;		//通过输出信号获取对端的输入信号列表;

	virtual bool   GetIEDListBySubnetKey(QString subnetKey, QVector<QString> &subIEDs) = 0;		//通过子网Key获取子网内IED;

	virtual bool   GetApInfo(QString subnetKey, QString IEDKey, QString APName, QAPInfo &apInfo) = 0;			//通过子网Key以及IEDKey获取接入点信息;

	virtual bool GetAPListOfIEDFromSubnet(const QString& subNetKey, const QString IEDName, QVector<QString> &APList) = 0;// 取得子网下IED的AP列表
	virtual bool GetPortListOfIEDAPFromSubNet(const QString& subNetKey, const QString IEDName, const QString APName, QVector<QString> &PortList) = 0;// 取得子网下IED某个AP包含的物理端口列表
	virtual bool GetAPListFromIED(const QString& IEDKey, QVector<QString> &APList) = 0;// 取得IED下AP的列表
	virtual bool GetAPPortListFormIED(const QString& IEDKey, const QString &APName, QVector<QString> &PortList) = 0;// 取得IED下某个AP对应的物理端口列表
};
#endif