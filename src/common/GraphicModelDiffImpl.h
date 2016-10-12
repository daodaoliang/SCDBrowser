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

	bool   GetBaseFileInfo(QSubStationInfo& FileInfo);//获得基准文件信息	
	bool   GetOtherFileInfo(QSubStationInfo& FileInfo);//获得被比较文件信息	

	bool   GetIEDList(QString panelKey, QVector<QString>* IEDKeys);//获得所有装置ID
	bool   GetIEDListBySubNetKey(QString subNetKey, QVector<QString> *subIEDs);//获取子网下所有装置ID;
	bool   GetSubNetKeysByIEDKey(QString IEDKey, QVector<QString> *subNetKeys);//根据装置获取所在子网;

	bool   GetIEDInfo(QString IEDKey, QIEDInfo& IEDInfo);//获得装置信息	
	bool   GetInfoOfInSignal(QString signalKey,QSignalInfo *signalInfo);                             //获得输入信号的信息  
	bool   GetInfoOfOutSignal(QString signalKey,QSignalInfo *signalInfo);                            //获得输出信号的信息

	//获得masterIEDKey为关键字IED的CBkey控制块发送给slaveIEDKey为关键字IED的虚链接列表
	bool   GetSignalInfosByIEDKeyAndCBKey(const QString masterIEDKey, const QString slaveIEDKey, const QString CBkey, QVector<QVirtualLink> *VirtualLinkList);
	VirLinkDiff   GetVirLinkStateByIEDKeyAndCBKey(const QString masterIEDKey, const QString slaveIEDKey, const QString CBkey);
	bool   GetSignalInfosByIEDKey(const QString IEDKey, QVector<QVirtualLink> *VirtualLinkList);
	int	   GetSignalCountByIEDKey(const QString IEDKey, QSignalTypeEnum sigType);

	bool   GetCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList);					   //获取当前IED的数据集列表
	bool   GetCtrlBlockInfo(QString ctrlBlockKey, QCtrlBlockInfo& cbInfo);							   //获取数据集信息
	bool   GetSlaveCtrlBlockByIEDKeys(QString masterKey, QString slaveKey, QVector<QString>* ctrlBlockList);	//获取发信号的IED的控制块;
	bool   GetOutSignalsByCBKey(QString masterIEDKey, QString cbKey, QVector<QString>* outSignals);		//通过控制块获取虚链接;

	bool   GetPhyPortInfo(QString pPortKey, QPhyPortInfo& pPortInfo);				//获取物理端口信息;
	bool   GetPhyPortListByIED(QString IEDKey, QVector<QString>* pPortList);		//根据IED获取物理端口;
	bool   GetPhyPortListByInSignalID(QString signalKey, QVector<QString> &pPortList);			//根据输入信号获取端口;
	bool   GetPhyPortKeyByOutSignalID(QString signalKey, QString* pPortKey);		//根据输出信号获取端口;

	//IED之间虚回路的差异放在控制块中实现
	bool   GetIEDListByLogicalRealitionWithCurrentIED(QString IEDKey,QVector<QString>* associatedIEDKeys);    //获得IED的关联IED列表（逻辑）

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