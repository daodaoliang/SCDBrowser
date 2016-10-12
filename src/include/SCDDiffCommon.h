#ifndef SCDDIFFCOMMON_H 
#define SCDDIFFCOMMON_H
#include <QString.h>
#include <vector>
#include <map>
#include "SCLModelSvrAdapterInterface.h"


// ������Ϣ�б�
enum ConfInfo
{
	// �ļ���Ϣ
	FILEINFO_VERSION,       // �ļ���Ϣ �汾��
	FILEINFO_REVERSION,     // �ļ���Ϣ �޶��汾��
	FILEINFO_CRC,			// �ļ�CRC
	// IED
	IED_DESC,
	IED_TYPE,
	IED_MANU,
	IED_CRC,
	// SV����
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
	IED_SV_DATASETADDR,			//	2014.5.19��� ���ݼ���ַ
	IED_SV_CBAPAPPID,			//	2014.5.19��� ���ƿ�Ap��ַ
	IED_SV_DATASET_ORD,
	IED_SV_DATASET_DOIDU,
	IED_SV_DATASET_DODESC,
	IED_SV_DATASET_LNDESC,
	IED_SV_DATASET_FC,
	IED_SV_DATASET_BDATYPE,
	IED_SV_DATASET_TRIPNAME,	//	2014.5.19��� ѹ������
	IED_SV_DATASET_SIGDESC,		//	2014.5.19��� �ź���������
	IED_SV_DATASET_PORTINDEX,	//	2014.5.19��� ����˿�����
	// GOOSE����
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
	IED_GOOSE_DATASETCOUNT,			// 2014.05.26 ���  ���ݼ����ź���Ŀ��
	IED_GOOSE_GOCBREF,
	IED_GOOSE_DATASETADDR,			//	2014.5.19��� ���ݼ���ַ
	IED_GOOSE_CBAPAPPID,			//	2014.5.19��� ���ƿ�Ap��ַ
	IED_GOOSE_DATASET_ORD,
	IED_GOOSE_DATASET_DOIDU,
	IED_GOOSE_DATASET_DODESC,
	IED_GOOSE_DATASET_LNDESC,
	IED_GOOSE_DATASET_FC,
	IED_GOOSE_DATASET_BDATYPE,
	IED_GOOSE_DATASET_TRIPNAME,		//	2014.5.19��� ѹ������
	IED_GOOSE_DATASET_SIGDESC,		//	2014.5.19��� �ź���������
	IED_GOOSE_DATASET_PORTINDEX,	//	2014.5.19��� ����˿�����
	// ��������
	IED_INPUTS_EXTREFCOUNT,
	//IED_INPUTS_EXTREF_ORD,
	IED_INPUTS_EXTREF_DESC,
	IED_INPUTS_EXTREF_INTADDR,
	IED_INPUTS_EXTREF_OUTADDR,
	IED_INPUTS_EXTREF_PORTNAME,		//	2014.5.19��� ����˿�����	

	// 2014.6.23����������źűȽ�
	IED_INPUTS_DESTSIG_DOIDU,
	IED_INPUTS_DESTSIG_DODESC,
	IED_INPUTS_DESTSIG_LNDESC,
	IED_INPUTS_DESTSIG_FC,
	IED_INPUTS_DESTSIG_BDATYPE,
	IED_INPUTS_DESTSIG_TRIPNAME,
	IED_INPUTS_DESTSIG_SIGDESC,
	IED_INPUTS_DESTSIG_PORTINDEX,


	// ����˿�
	IED_PHY_TRANSMEDIA,				    //����˿�����
	IED_PHY_PORTINDEX,				    //���
	IED_PHY_PLUG,						//��ͷ����
	IED_PHY_TYPE,						//������������
	IED_PHY_CABLE,						//�շ�����

	ITEMCOUNT               // ��������Ŀ
};

// ����״̬
enum DiffStates
{
	DIFFSTATE_UNKNOWN,					///< e.g. an empty file
	DIFFSTATE_NORMAL,					///< �ޱ仯
	DIFFSTATE_REMOVED,					///< ��׼�ļ����Ա�ɾ��
	DIFFSTATE_ADDED,					///< ���Ƚ��ļ�����
	DIFFSTATE_EDITED					///< �����ļ����в���
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

// �����ṹ
typedef struct BasicStruct
{
	//bool			b_Compared;         // �Ƿ��Ѿ��ȶ�
	QString			AttributeDesc;
	QString          baseAttributeName;  // ��׼�ļ���������
	QString          otherAttributeName; // �Ƚ��ļ���������
	DiffStates      state;              // �ȽϽ��

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
	//bool			b_Compared;         // �Ƿ��Ѿ��ȶ�
	QString		AttributeDesc;
	int          baseAttributeName;		// ��׼�ļ���������
	int          otherAttributeName;	// �Ƚ��ļ���������
	DiffStates      state;              // �ȽϽ��

	void clear()
	{
		//b_Compared = false;
		AttributeDesc.clear();
		baseAttributeName = -1;
		otherAttributeName = -1;
		state = DIFFSTATE_NORMAL;
	}
}BasicIntStru;


struct SignalStru						//��DataSetStru�޸�ΪSignalStru
{
	DiffStates state;

	BasicIntStru Ord;					// ��Ŀ���
	BasicStru DOIdU;					// DOI.dU
	BasicStru DODesc;					// DO����
	BasicStru LNDesc;					// LN����
	BasicStru FC;						// ����Լ��FC
	BasicStru BDAType;					// BDA��������
	BasicStru TripName;					//ѹ�����ƣ����ѹ������Ϊ�ձ�ʾû��ѹ�壩
	BasicStru Desc;						//�ź��������ƣ����ѹ������Ϊ�ձ�ʾû��ѹ�壩	
	BasicStru PortIndex;				//����˿����������ѹ������Ϊ�ձ�ʾû��ѹ�壩	
	QString   ID;						//�ź�ID
	QSignalTypeEnum type;				// �ź�����

	QString   ctrlBlockID;						//���ƿ�ID

	SignalStru()
	{
		state = DIFFSTATE_NORMAL;
		Ord.clear();
		DOIdU.clear();
		DODesc.clear();
		LNDesc.clear();
		FC.clear();
		BDAType.clear();
		TripName.clear();				//ѹ�����ƣ����ѹ������Ϊ�ձ�ʾû��ѹ�壩
		PortIndex.clear();
		Desc.clear();					//�ź��������ƣ����ѹ������Ϊ�ձ�ʾû��ѹ�壩	
		ID.clear();						//�ź�ID		
	}
};
typedef std::vector<SignalStru> VSignalStru;////////��DataSetStru�޸�ΪSignalStru


struct DSSettingSignalStru						//��DataSetStru�޸�ΪSignalStru
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

struct PhysicsPortStru					//����
{
	DiffStates state;

	BasicStru	pIndex;					//���
	BasicStru	plug;					//��ͷ����
	BasicStru	type;					//�ӿ�����
	BasicStru	transMedia;
	BasicStru	cable;

	QString   ID;						//�˿�ID

	PhysicsPortStru()
	{
		state = DIFFSTATE_NORMAL;
		transMedia.clear();					//����
		pIndex.clear();					//���
		plug.clear();					//��ͷ����
		type.clear();					//�ӿ�����
		cable.clear();					//�շ�����
		ID.clear();						//�˿�ID
	}
};

typedef std::vector<PhysicsPortStru> VPhysicsPortStru;	//��������˿ڶ���

// SV����
struct SVCBStru						//��SVStru�޸�ΪSVCBStru
{
	DiffStates state;

	BasicStru AppID;				// APPID
	BasicStru VLANID;				// VLANID
	BasicStru VLANPriority;			// VLAN���ȼ�
	BasicStru MACAddr;				// MAC��ַ
	BasicStru ConfVersion;			// ���ð汾��
	BasicIntStru ASDUCount;			// ASDU��Ŀ
	BasicStru smpRate;				// ������
	BasicStru CBName;				// ���ƿ�����
	BasicStru CBDesc;				// ���ƿ�����
	BasicStru DataSetName;			// ���ݼ�
	BasicStru DataSetDesc;			// ���ݼ�����
	BasicIntStruct DataSetCount;	// ���ݼ���Ŀ��
	BasicStruct DataSetAddr;		// ���ݼ���ַ
	BasicStruct apAppID;			// ���ƿ�ap��ַ		
	QString   ID;					//SV���ƿ�ID��ģ�Ϳ��е�ID��

	QString   type;

	VSignalStru vSignal;			//����ź�  ��VDataSetStru vDataSet  �� VSignalStru vSignal
	QVector<QString> vIEDList;      // ���ڱ�������źŵ�Ŀ��IED�б�


	BasicStru VLink;				// ������������

	SVCBStru()						//��SVStru��SVCBStru
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
		DataSetAddr.clear();			// ���ݼ���ַ
		apAppID.clear();				// ���ƿ�ap��ַ		
		ID.clear();						//SV���ƿ�ID��ģ�Ϳ��е�ID��
		vSignal.clear();				//����ź�  ��VDataSetStru vDataSet  �� VSignalStru vSignal		
		VLink.clear();
		vIEDList.clear();
	}
};

typedef std::vector<SVCBStru> VSVCBStru;	//��vector<SVCBStru> VSVCBStru ��vector<SVStru> VSVStru

struct ReportCBStru						
{
	DiffStates state;
	BasicStru AppID;				// APPID
	BasicStru VLANID;				// VLANID
	BasicStru VLANPriority;			// VLAN���ȼ�
	BasicStru MACAddr;				// MAC��ַ
	BasicStru ConfVersion;			// ���ð汾��
	BasicIntStru ASDUCount;			// ASDU��Ŀ
	BasicStru smpRate;				// ������
	BasicStru CBName;				// ���ƿ�����
	BasicStru CBDesc;				// ���ƿ�����
	BasicStru DataSetName;			// ���ݼ�
	BasicStru DataSetDesc;			// ���ݼ�����
	BasicIntStruct DataSetCount;	// ���ݼ���Ŀ��
	BasicStruct DataSetAddr;		// ���ݼ���ַ
	BasicStruct apAppID;			// ���ƿ�ap��ַ

	// add by zxc for ReportCB 2014.11.25
	QString LDInst;
	QString desc;
	QString intgPd; // report���ƿ�����������
	QString Buffered; // �涨�����Ƿ񻺴�
	QString bufTime;  // ����ʱ��

	QString   ID;					//SV���ƿ�ID��ģ�Ϳ��е�ID��
	QString   type;
	VSignalStru vSignal;			//����ź�  ��VDataSetStru vDataSet  �� VSignalStru vSignal
	QVector<QString> vIEDList;      // ���ڱ�������źŵ�Ŀ��IED�б�
	BasicStru VLink;				// ������������
	ReportCBStru()						//��SVStru��SVCBStru
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
		DataSetAddr.clear();			// ���ݼ���ַ
		apAppID.clear();				// ���ƿ�ap��ַ		
		ID.clear();						//SV���ƿ�ID��ģ�Ϳ��е�ID��
		vSignal.clear();				//����ź�  ��VDataSetStru vDataSet  �� VSignalStru vSignal		
		VLink.clear();
		vIEDList.clear();
		
		desc.clear();
		intgPd.clear();
		Buffered.clear();
		bufTime.clear();
	}
};
typedef std::vector<ReportCBStru> VReportCBStru;	//��vector<SVCBStru> VSVCBStru ��vector<SVStru> VSVStru
// GOOSE����
struct GOOSECBStru  ////////��GOOSEStru��GOOSECBStru
{
	DiffStates state;

	BasicStru AppID;				// APPID
	BasicStru VLANID;				// VLANID
	BasicStru VLANPriority;			// VLAN���ȼ�
	BasicStru MACAddr;				// MAC��ַ
	BasicStru ConfVersion;			// ���ð汾��
	BasicStru MaxTime;				// MaxTime
	BasicStru MinTime;				// MinTime
	BasicStru CBName;				// ���ƿ�����
	BasicStru CBDesc;				// ���ƿ�����
	BasicStru DataSetName;			// ���ݼ�
	BasicStru DataSetDesc;			// ���ݼ�����
	BasicIntStruct DataSetCount;	// ���ݼ���Ŀ��
	BasicStru GocbRef;				// gocbRef
	BasicStru VLink;				// ������������

	QString   type;

	VSignalStru vSignal;			// ����ź�    ////////��VDataSetStru vDataSet  �� VSignalStru vSignal
	BasicStruct DataSetAddr;		// ���ݼ���ַ
	BasicStruct apAppID;			// ���ƿ�ap��ַ		
	QString		ID;					//GOOSE���ƿ�ID��ģ�Ϳ��е�ID��
	QVector<QString> vIEDList;      // ���ڱ�������źŵ�Ŀ��IED�б�
	
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
		DataSetAddr.clear();		// ���ݼ���ַ
		apAppID.clear();			// ���ƿ�ap��ַ		
		ID.clear();					//GOOSE���ƿ�ID��ģ�Ϳ��е�ID��		
		VLink.clear();
		vIEDList.clear();
	}
};

typedef std::vector<GOOSECBStru> VGOOSECBStru;

// ExtRef��Ϣ
struct ExtRefStru
{
	DiffStates state;

	BasicIntStruct Ord;					// ��Ŀ���
	BasicStru desc;					    // �ڲ���������
	BasicStru OutAddr;					// �ⲿ���ӵ�ַ
	BasicStru VLink;					// ���������

	BasicStru IntAddr;					// �ڲ����ӵ�ַ
	QString   ID;						//������ID
	QString   srcSignalID;				//����ź�ID;
	QString   destSignalID;				//�����ź�ID;
	BasicStru PortName;
	QSignalTypeEnum type;
	SignalStru InSigInfo;				// �����źŵ���ϸ��Ϣ
	
	ExtRefStru()
	{
		state = DIFFSTATE_NORMAL;
		Ord.clear();
		IntAddr.clear();
		OutAddr.clear();
		VLink.clear();
		desc.clear();
		ID.clear();						//������ID
		srcSignalID.clear();			//����ź�ID;
		destSignalID.clear();			//�����ź�ID;
		PortName.clear();
	}
};

typedef std::vector<ExtRefStru> VExtRefStru;

// ��������
struct InputStru
{
	DiffStates state;

	BasicIntStruct ExtRefCount;			// �ⲿ������Ŀ��
	VExtRefStru vExtRef;			    // �ⲿ������Ŀ
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

// �ļ���Ϣ
struct FileInfoStru 
{
	DiffStates state;				// �ļ���Ϣ ״̬

	BasicStru version;				// �汾��Ϣ
	BasicStru reVersion;			//�޶��汾��
	BasicStru crc;					// ȫվ�����CRC
	
	QString   basefileName;			// ��׼�ļ���
	QString   baseSubstationName;
	QString   baseSubstationHitmTime;
	QString   basefileTime;			// ��׼�ļ�����ʱ��
	QString   basefileKey;

	QString   otherfileName;		// ��׼�ļ���
	QString   otherSubstationName;
	QString   otherSubstationHitmTime;
	QString   otherfileTime;		// ��׼�ļ�����ʱ��
	QString   otherfileKey;
		
	FileInfoStru()
	{
		state = DIFFSTATE_NORMAL;
		version.clear();
		reVersion.clear();
		crc.clear();
		basefileName.clear();		// ��׼�ļ���
		basefileTime.clear();		// ��׼�ļ�����ʱ��
		otherfileName.clear();		// ��׼�ļ���
		otherfileTime.clear();		// ��׼�ļ�����ʱ��		
		basefileKey.clear();
		otherfileKey.clear();
	}
};

// IED
struct IEDStru
{
	DiffStates state;				// IED ״̬

	BasicStru IEDDesc;				// IED����
	BasicStru IEDType;				// �豸�ͺ�
	BasicStru IEDManu;				// �豸����
	BasicStru IEDCRC;				// IED���������CRC
	
	QString   IEDName;				// IED����;2014.5.12����
	QString   ID;					// IED ID;2014.5.12����

	VSVCBStru			vSVCB;
	VGOOSECBStru		vGOOSECB;
	VInputStru			vInput;
	VVirtualLink		vVirLink;
	VPhysicsPortStru	vPortStru;		// ��������˿� 2014.5.12����;
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
		vPortStru.clear();			//��������˿�
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