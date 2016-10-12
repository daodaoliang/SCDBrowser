#ifndef GRAPHICMODELINTERFACE_H
#define GRAPHICMODELINTERFACE_H

#include <QString>
#include <QVector>
#include "SCLModelSvrAdapterInterface.h"
#include "SCDDiffCommon.h"

//接口名称：GraphicModelInterface;
//接口作用：封装了绘制图形所需要的模型信息;
//接口说明：对于实现图形绘制和对象属性展示的客户端来说，该接口主要屏蔽了差异化模型和非差异化模型的差异;
            /*
            注1：该接口通过对象的Key来唯一标识对象，并且通过对象Key来获取对象的属性。其中，对于非差异化模型来说;
            其对象Key与实时库中的Key完全一致，对于差异化模型来说，对象Key是在非差异化Key的基础上去掉了变电站Name;
            信息。;
            
            注2：所有的客户端数据结构如QIEDInfo、QPhyPortInfo等都加上状态位，表示增加、删除、修改及未改变;
            
            注3：比较模型需要增加物理端口信息;
            
            注4：比较模型需要增加map(对象ID，模型ID)，方便查找;
            
            注5：比较模型需要在各对象的结构体中增加对象ID;
            
            注6：比较模型扩展了部分不参与比较，但是需要显示的属性;
            */

class GraphicModelInterface
{
public:
	virtual bool   GetBaseFileInfo(QSubStationInfo& FileInfo) = 0;//获得基准文件信息;
	virtual bool   GetOtherFileInfo(QSubStationInfo& FileInfo) = 0;//获得被比较文件信息;
				
	virtual bool   GetIEDList(QString panelKey, QVector<QString>* IEDKeys) = 0;//获得所有装置ID;
	virtual bool   GetIEDListBySubNetKey(QString subNetKey, QVector<QString> *subIEDs) = 0;//获取子网下所有装置ID;
	virtual bool   GetSubNetKeysByIEDKey(QString IEDKey, QVector<QString> *subNetKeys) = 0;//根据装置获取所在子网;

	virtual bool   GetIEDInfo(QString IEDKey, QIEDInfo& IEDInfo) = 0;//获得装置信息;
	virtual bool   GetInfoOfInSignal(QString signalKey,QSignalInfo *signalInfo) = 0;//获得输入信号的信息;
	virtual bool   GetInfoOfOutSignal(QString signalKey,QSignalInfo *signalInfo) = 0;//获得输出信号的信息;
	
	//获得masterIEDKey为关键字IED的CBkey控制块发送给slaveIEDKey为关键字IED的虚链接列表;
	virtual bool   GetSignalInfosByIEDKeyAndCBKey(const QString masterIEDKey, const QString slaveIEDKey, const QString CBkey, QVector<QVirtualLink> *VirtualLinkList) = 0;
	virtual VirLinkDiff   GetVirLinkStateByIEDKeyAndCBKey(const QString masterIEDKey, const QString slaveIEDKey, const QString CBkey) = 0;
	
	virtual bool   GetSignalInfosByIEDKey(const QString IEDKey, QVector<QVirtualLink> *VirtualLinkList) = 0;
	virtual int	   GetSignalCountByIEDKey(const QString IEDKey, QSignalTypeEnum sigType) = 0;

	virtual bool   GetCtrlBlockListByIED(QString IEDKey, QVector<QString>* ctrlBlockList) = 0;//获取当前IED的数据集列表;
	virtual bool   GetCtrlBlockInfo(QString ctrlBlockKey, QCtrlBlockInfo& cbInfo) = 0;//获取数据集信息;
	virtual bool   GetSlaveCtrlBlockByIEDKeys(QString masterKey, QString slaveKey, QVector<QString>* ctrlBlockList) = 0;//获取发信号的IED的控制块;
	virtual bool   GetOutSignalsByCBKey(QString masterIEDKey, QString cbKey, QVector<QString>* outSignals) = 0;//通过控制块获取虚链接;

	virtual bool   GetPhyPortInfo(QString pPortKey, QPhyPortInfo& pPortInfo) = 0;//获取物理端口信息;
	virtual bool   GetPhyPortListByIED(QString IEDKey, QVector<QString>* pPortList) = 0;//根据IED获取物理端口;
	virtual bool   GetPhyPortListByInSignalID(QString signalKey, QVector<QString> &pPortList) = 0;//根据输入信号获取端口;
	virtual bool   GetPhyPortKeyByOutSignalID(QString signalKey, QString* pPortKey) = 0;//根据输出信号获取端口;

  //IED之间虚回路的差异放在控制块中实现;
	//virtual int	   GetVLinkChangeType(QString masterIEDName, QString slaveIEDName, QString cbName) = 0;//获取虚回路差异类型(信息流图);
	virtual bool   GetIEDListByLogicalRealitionWithCurrentIED(QString IEDKey,QVector<QString>* associatedIEDKeys) = 0;    //获得IED的关联IED列表（逻辑);
};

#endif	//GRAPHICMODELINTERFACE_H