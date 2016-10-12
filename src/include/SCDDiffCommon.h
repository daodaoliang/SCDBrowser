#ifndef SCDDIFFCOMMON_H 
#define SCDDIFFCOMMON_H
#include <QString.h>
#include <vector>
#include <map>
#include "SCLModelSvrAdapterInterface.h"


// 配置信息列表
enum ConfInfo
{
	// 文件信息
	FILEINFO_VERSION,       // 文件信息 版本号
	FILEINFO_REVERSION,     // 文件信息 修订版本号
	FILEINFO_CRC,			// 文件CRC
	// IED
	IED_DESC,
	IED_TYPE,
	IED_MANU,
	IED_CRC,
	// SV发送
	IED_SV_APPID,
	IED_SV_VLANID,
	IED_SV_VLANPRIORITY,
	IED_SV_MACADDR,
	IED_SV_CONFVERSION,
	//IED_SV_ASDUCOUNT,		// +
	IED_SV_SMPRATE,			// +
	IED_SV_CBNAME,
	// IED_SV_CBDESC,
	IED_SV_DATASETNAME,
	IED_SV_DATASETDESC,
	IED_SV_DATASETCOUNT,
	IED_SV_DATASETADDR,			//	2014.5.19添加 数据集地址
	IED_SV_CBAPAPPID,			//	2014.5.19添加 控制块Ap地址
	IED_SV_DATASET_ORD,
	IED_SV_DATASET_DOIDU,
	IED_SV_DATASET_DODESC,
	IED_SV_DATASET_LNDESC,
	IED_SV_DATASET_FC,
	IED_SV_DATASET_BDATYPE,
	IED_SV_DATASET_TRIPNAME,	//	2014.5.19添加 压板名称
	IED_SV_DATASET_SIGDESC,		//	2014.5.19添加 信号名称描述
	IED_SV_DATASET_PORTINDEX,	//	2014.5.19添加 物理端口索引
	// GOOSE发送
	IED_GOOSE_APPID,
	IED_GOOSE_VLANID,
	IED_GOOSE_VLANPRIORITY,
	IED_GOOSE_MACADDR,
	IED_GOOSE_CONFVERSION,
	IED_GOOSE_MAXTIME,		// +
	IED_GOOSE_MINTIME,		// +	
	IED_GOOSE_CBNAME,
	// IED_GOOSE_CBDESC,
	IED_GOOSE_DATASETNAME,
	IED_GOOSE_DATASETDESC,
	IED_GOOSE_DATASETCOUNT,			// 2014.05.26 添加  数据集中信号条目数
	IED_GOOSE_GOCBREF,
	IED_GOOSE_DATASETADDR,			//	2014.5.19添加 数据集地址
	IED_GOOSE_CBAPAPPID,			//	2014.5.19添加 控制块Ap地址
	IED_GOOSE_DATASET_ORD,
	IED_GOOSE_DATASET_DOIDU,
	IED_GOOSE_DATASET_DODESC,
	IED_GOOSE_DATASET_LNDESC,
	IED_GOOSE_DATASET_FC,
	IED_GOOSE_DATASET_BDATYPE,
	IED_GOOSE_DATASET_TRIPNAME,		//	2014.5.19添加 压板名称
	IED_GOOSE_DATASET_SIGDESC,		//	2014.5.19添加 信号名称描述
	IED_GOOSE_DATASET_PORTINDEX,	//	2014.5.19添加 物理端口索引
	// 数据输入
	IED_INPUTS_EXTREFCOUNT,
	//IED_INPUTS_EXTREF_ORD,
	IED_INPUTS_EXTREF_DESC,
	IED_INPUTS_EXTREF_INTADDR,
	IED_INPUTS_EXTREF_OUTADDR,
	IED_INPUTS_EXTREF_PORTNAME,		//	2014.5.19添加 物理端口索引	

	// 2014.6.23日添加输入信号比较
	IED_INPUTS_DESTSIG_DOIDU,
	IED_INPUTS_DESTSIG_DODESC,
	IED_INPUTS_DESTSIG_LNDESC,
	IED_INPUTS_DESTSIG_FC,
	IED_INPUTS_DESTSIG_BDATYPE,
	IED_INPUTS_DESTSIG_TRIPNAME,
	IED_INPUTS_DESTSIG_SIGDESC,
	IED_INPUTS_DESTSIG_PORTINDEX,


	// 物理端口
	IED_PHY_TRANSMEDIA,				    //物理端口类型
	IED_PHY_PORTINDEX,				    //序号
	IED_PHY_PLUG,						//插头类型
	IED_PHY_TYPE,						//物理网口类型
	IED_PHY_CABLE,						//收发类型

	ITEMCOUNT               // 配置项数目
};

// 差异状态
enum DiffStates
{
	DIFFSTATE_UNKNOWN,					///< e.g. an empty file
	DIFFSTATE_NORMAL,					///< 无变化
	DIFFSTATE_REMOVED,					///< 基准文件属性被删除
	DIFFSTATE_ADDED,					///< 被比较文件新增
	DIFFSTATE_EDITED					///< 两个文件中有差异
};

struct VirLinkDiff
{
	DiffStates state;
	int		   iCount;
	QString    diffDesc;

	VirLinkDiff()
	{
		state = DIFFSTATE_NORMAL;
		iCount = 0;
		diffDesc.clear();
	}
};

// 基础结构
typedef struct BasicStruct
{
	//bool			b_Compared;         // 是否已经比对
	QString			AttributeDesc;
	QString          baseAttributeName;  // 基准文件属性名称
	QString          otherAttributeName; // 比较文件属性名称
	DiffStates      state;              // 比较结果

	void clear()
	{
		//b_Compared = false;
		AttributeDesc.clear();
		baseAttributeName.clear();
		otherAttributeName.clear();
		state = DIFFSTATE_NORMAL;
	}
}BasicStru;

typedef struct BasicIntStruct
{
	//bool			b_Compared;         // 是否已经比对
	QString		AttributeDesc;
	int          baseAttributeName;		// 基准文件属性名称
	int          otherAttributeName;	// 比较文件属性名称
	DiffStates      state;              // 比较结果

	void clear()
	{
		//b_Compared = false;
		AttributeDesc.clear();
		baseAttributeName = -1;
		otherAttributeName = -1;
		state = DIFFSTATE_NORMAL;
	}
}BasicIntStru;


struct SignalStru						//从DataSetStru修改为SignalStru
{
	DiffStates state;

	BasicIntStru Ord;					// 条目序号
	BasicStru DOIdU;					// DOI.dU
	BasicStru DODesc;					// DO描述
	BasicStru LNDesc;					// LN描述
	BasicStru FC;						// 功能约束FC
	BasicStru BDAType;					// BDA数据类型
	BasicStru TripName;					//压板名称（如果压板名称为空表示没有压板）
	BasicStru Desc;						//信号描述名称（如果压板名称为空表示没有压板）	
	BasicStru PortIndex;				//物理端口索引（如果压板名称为空表示没有压板）	
	QString   ID;						//信号ID
	QSignalTypeEnum type;				// 信号类型

	QString   ctrlBlockID;						//控制块ID

	SignalStru()
	{
		state = DIFFSTATE_NORMAL;
		Ord.clear();
		DOIdU.clear();
		DODesc.clear();
		LNDesc.clear();
		FC.clear();
		BDAType.clear();
		TripName.clear();				//压板名称（如果压板名称为空表示没有压板）
		PortIndex.clear();
		Desc.clear();					//信号描述名称（如果压板名称为空表示没有压板）	
		ID.clear();						//信号ID		
	}
};
typedef std::vector<SignalStru> VSignalStru;////////从DataSetStru修改为SignalStru


struct DSSettingSignalStru						//从DataSetStru修改为SignalStru
{
	DiffStates state;

	QString  cdc;					
	QString  minVal;					
	QString  maxVal;					
	QString  stepSize;							
	QString  unit;						

	DSSettingSignalStru()
	{
		state = DIFFSTATE_NORMAL;
		cdc.clear();
		minVal.clear();
		maxVal.clear();
		stepSize.clear();
		unit.clear();
	}
};

struct PhysicsPortStru					//增加
{
	DiffStates state;

	BasicStru	pIndex;					//序号
	BasicStru	plug;					//插头类型
	BasicStru	type;					//接口类型
	BasicStru	transMedia;
	BasicStru	cable;

	QString   ID;						//端口ID

	PhysicsPortStru()
	{
		state = DIFFSTATE_NORMAL;
		transMedia.clear();					//描述
		pIndex.clear();					//序号
		plug.clear();					//插头类型
		type.clear();					//接口类型
		cable.clear();					//收发类型
		ID.clear();						//端口ID
	}
};

typedef std::vector<PhysicsPortStru> VPhysicsPortStru;	//增加物理端口定义

// SV发送
struct SVCBStru						//从SVStru修改为SVCBStru
{
	DiffStates state;

	BasicStru AppID;				// APPID
	BasicStru VLANID;				// VLANID
	BasicStru VLANPriority;			// VLAN优先级
	BasicStru MACAddr;				// MAC地址
	BasicStru ConfVersion;			// 配置版本号
	BasicIntStru ASDUCount;			// ASDU数目
	BasicStru smpRate;				// 采样率
	BasicStru CBName;				// 控制块名称
	BasicStru CBDesc;				// 控制块描述
	BasicStru DataSetName;			// 数据集
	BasicStru DataSetDesc;			// 数据集描述
	BasicIntStruct DataSetCount;	// 数据集条目数
	BasicStruct DataSetAddr;		// 数据集地址
	BasicStruct apAppID;			// 控制块ap地址		
	QString   ID;					//SV控制块ID（模型库中的ID）

	QString   type;

	VSignalStru vSignal;			//输出信号  从VDataSetStru vDataSet  到 VSignalStru vSignal
	QVector<QString> vIEDList;      // 用于保存输出信号的目的IED列表


	BasicStru VLink;				// 输出虚端子连线

	SVCBStru()						//从SVStru到SVCBStru
	{
		state = DIFFSTATE_NORMAL;
		AppID.clear();
		VLANID.clear();
		VLANPriority.clear();
		MACAddr.clear();
		ConfVersion.clear();
		ASDUCount.clear();
		smpRate.clear();
		CBName.clear();
		CBDesc.clear();
		DataSetName.clear();
		DataSetDesc.clear();
		DataSetCount.clear();
		DataSetAddr.clear();			// 数据集地址
		apAppID.clear();				// 控制块ap地址		
		ID.clear();						//SV控制块ID（模型库中的ID）
		vSignal.clear();				//输出信号  从VDataSetStru vDataSet  到 VSignalStru vSignal		
		VLink.clear();
		vIEDList.clear();
	}
};

typedef std::vector<SVCBStru> VSVCBStru;	//从vector<SVCBStru> VSVCBStru 到vector<SVStru> VSVStru

struct ReportCBStru						
{
	DiffStates state;
	BasicStru AppID;				// APPID
	BasicStru VLANID;				// VLANID
	BasicStru VLANPriority;			// VLAN优先级
	BasicStru MACAddr;				// MAC地址
	BasicStru ConfVersion;			// 配置版本号
	BasicIntStru ASDUCount;			// ASDU数目
	BasicStru smpRate;				// 采样率
	BasicStru CBName;				// 控制块名称
	BasicStru CBDesc;				// 控制块描述
	BasicStru DataSetName;			// 数据集
	BasicStru DataSetDesc;			// 数据集描述
	BasicIntStruct DataSetCount;	// 数据集条目数
	BasicStruct DataSetAddr;		// 数据集地址
	BasicStruct apAppID;			// 控制块ap地址

	// add by zxc for ReportCB 2014.11.25
	QString LDInst;
	QString desc;
	QString intgPd; // report控制块完整性周期
	QString Buffered; // 规定报告是否缓存
	QString bufTime;  // 缓存时间

	QString   ID;					//SV控制块ID（模型库中的ID）
	QString   type;
	VSignalStru vSignal;			//输出信号  从VDataSetStru vDataSet  到 VSignalStru vSignal
	QVector<QString> vIEDList;      // 用于保存输出信号的目的IED列表
	BasicStru VLink;				// 输出虚端子连线
	ReportCBStru()						//从SVStru到SVCBStru
	{
		state = DIFFSTATE_NORMAL;
		AppID.clear();
		VLANID.clear();
		VLANPriority.clear();
		MACAddr.clear();
		ConfVersion.clear();
		ASDUCount.clear();
		smpRate.clear();
		CBName.clear();
		CBDesc.clear();
		DataSetName.clear();
		DataSetDesc.clear();
		DataSetCount.clear();
		DataSetAddr.clear();			// 数据集地址
		apAppID.clear();				// 控制块ap地址		
		ID.clear();						//SV控制块ID（模型库中的ID）
		vSignal.clear();				//输出信号  从VDataSetStru vDataSet  到 VSignalStru vSignal		
		VLink.clear();
		vIEDList.clear();
		
		desc.clear();
		intgPd.clear();
		Buffered.clear();
		bufTime.clear();
	}
};
typedef std::vector<ReportCBStru> VReportCBStru;	//从vector<SVCBStru> VSVCBStru 到vector<SVStru> VSVStru
// GOOSE发送
struct GOOSECBStru  ////////从GOOSEStru到GOOSECBStru
{
	DiffStates state;

	BasicStru AppID;				// APPID
	BasicStru VLANID;				// VLANID
	BasicStru VLANPriority;			// VLAN优先级
	BasicStru MACAddr;				// MAC地址
	BasicStru ConfVersion;			// 配置版本号
	BasicStru MaxTime;				// MaxTime
	BasicStru MinTime;				// MinTime
	BasicStru CBName;				// 控制块名称
	BasicStru CBDesc;				// 控制块描述
	BasicStru DataSetName;			// 数据集
	BasicStru DataSetDesc;			// 数据集描述
	BasicIntStruct DataSetCount;	// 数据集条目数
	BasicStru GocbRef;				// gocbRef
	BasicStru VLink;				// 输出虚端子连线

	QString   type;

	VSignalStru vSignal;			// 输出信号    ////////从VDataSetStru vDataSet  到 VSignalStru vSignal
	BasicStruct DataSetAddr;		// 数据集地址
	BasicStruct apAppID;			// 控制块ap地址		
	QString		ID;					//GOOSE控制块ID（模型库中的ID）
	QVector<QString> vIEDList;      // 用于保存输出信号的目的IED列表
	
	GOOSECBStru()
	{
		state = DIFFSTATE_NORMAL;
		AppID.clear();
		VLANID.clear();
		VLANPriority.clear();
		MACAddr.clear();
		ConfVersion.clear();
		MaxTime.clear();
		MinTime.clear();
		CBName.clear();
		CBDesc.clear();
		DataSetName.clear();
		DataSetDesc.clear();
		DataSetCount.clear();
		GocbRef.clear();
		vSignal.clear();
		DataSetAddr.clear();		// 数据集地址
		apAppID.clear();			// 控制块ap地址		
		ID.clear();					//GOOSE控制块ID（模型库中的ID）		
		VLink.clear();
		vIEDList.clear();
	}
};

typedef std::vector<GOOSECBStru> VGOOSECBStru;

// ExtRef信息
struct ExtRefStru
{
	DiffStates state;

	BasicIntStruct Ord;					// 条目序号
	BasicStru desc;					    // 内部端子描述
	BasicStru OutAddr;					// 外部端子地址
	BasicStru VLink;					// 虚端子连线

	BasicStru IntAddr;					// 内部端子地址
	QString   ID;						//虚链接ID
	QString   srcSignalID;				//输出信号ID;
	QString   destSignalID;				//输入信号ID;
	BasicStru PortName;
	QSignalTypeEnum type;
	SignalStru InSigInfo;				// 输入信号的详细信息
	
	ExtRefStru()
	{
		state = DIFFSTATE_NORMAL;
		Ord.clear();
		IntAddr.clear();
		OutAddr.clear();
		VLink.clear();
		desc.clear();
		ID.clear();						//虚链接ID
		srcSignalID.clear();			//输出信号ID;
		destSignalID.clear();			//输入信号ID;
		PortName.clear();
	}
};

typedef std::vector<ExtRefStru> VExtRefStru;

// 数据输入
struct InputStru
{
	DiffStates state;

	BasicIntStruct ExtRefCount;			// 外部引用条目数
	VExtRefStru vExtRef;			    // 外部引用条目
	InputStru()
	{
		state = DIFFSTATE_NORMAL;
		ExtRefCount.clear();
		vExtRef.clear();
	}
};
typedef std::vector<InputStru> VInputStru;

struct VirtualLinkStur
{
	DiffStates state;

	QString	   srcSig;
	QString	   destSig;
	VirtualLinkStur()
	{
		state = DIFFSTATE_NORMAL;
		srcSig.clear();
		destSig.clear();
	}
};
typedef std::vector<VirtualLinkStur>  VVirtualLink;

// 文件信息
struct FileInfoStru 
{
	DiffStates state;				// 文件信息 状态

	BasicStru version;				// 版本信息
	BasicStru reVersion;			//修订版本号
	BasicStru crc;					// 全站虚端子CRC
	
	QString   basefileName;			// 基准文件名
	QString   baseSubstationName;
	QString   baseSubstationHitmTime;
	QString   basefileTime;			// 基准文件导入时间
	QString   basefileKey;

	QString   otherfileName;		// 基准文件名
	QString   otherSubstationName;
	QString   otherSubstationHitmTime;
	QString   otherfileTime;		// 基准文件导入时间
	QString   otherfileKey;
		
	FileInfoStru()
	{
		state = DIFFSTATE_NORMAL;
		version.clear();
		reVersion.clear();
		crc.clear();
		basefileName.clear();		// 基准文件名
		basefileTime.clear();		// 基准文件导入时间
		otherfileName.clear();		// 基准文件名
		otherfileTime.clear();		// 基准文件导入时间		
		basefileKey.clear();
		otherfileKey.clear();
	}
};

// IED
struct IEDStru
{
	DiffStates state;				// IED 状态

	BasicStru IEDDesc;				// IED描述
	BasicStru IEDType;				// 设备型号
	BasicStru IEDManu;				// 设备厂家
	BasicStru IEDCRC;				// IED虚端子配置CRC
	
	QString   IEDName;				// IED名字;2014.5.12新增
	QString   ID;					// IED ID;2014.5.12新增

	VSVCBStru			vSVCB;
	VGOOSECBStru		vGOOSECB;
	VInputStru			vInput;
	VVirtualLink		vVirLink;
	VPhysicsPortStru	vPortStru;		// 增加物理端口 2014.5.12新增;
	IEDStru()
	{
		state = DIFFSTATE_NORMAL;
		IEDDesc.clear();
		IEDType.clear();
		IEDManu.clear();
		IEDCRC.clear();
		vSVCB.clear();
		vGOOSECB.clear();
		vInput.clear();
		vPortStru.clear();			//增加物理端口
	}
};

typedef std::vector<IEDStru> VIEDStru;

struct SCDStru 
{
	FileInfoStru sFileInfo;

	VIEDStru vIEDStru;
};

typedef std::map<QString, void*> DifferMap;

typedef std::map<QString, QString> SearchMap;

#endif