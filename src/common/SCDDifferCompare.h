#ifndef SCDDIFFCOMPARE_H
#define SCDDIFFCOMPARE_H

#include "SCDDiffCommon.h"
#include "SCLNameTranslator.h"
#include "SCLModelSvrAdapterInterface.h"

class SCDDiffCompare
{
public:

	SCDDiffCompare();
	~SCDDiffCompare();

	static SCDDiffCompare * GetInstance() 
	{  
		if (m_pSCDInstance == NULL)
			m_pSCDInstance = new SCDDiffCompare();  
		return m_pSCDInstance; 
	};

	bool GetTotalDiff(QString baseSubStationKey, QString otherSubStationKey);
	bool GetFileInfoDiff(QSubStationInfo baseSubStationInfo, QSubStationInfo otherSubStationInfo);
	bool GetIedDiff(QVector<QString> &baseIEDList, QVector<QString> &otherIEDList, VIEDStru &vStrIED);

	bool GetControlBlockDiff(QVector<QString> &baseCBKey, QVector<QString> &otherCBKey, IEDStru& strIED, DiffStates parentState);
	bool GetDataSetDiff(QVector<QString> &baseSignalKeyList, QVector<QString> &otherSignalKeyList, QSignalTypeEnum signalType, VSignalStru &vStrSignal, DiffStates parentState);

	bool GetAssoIEDListDiff(QVector<QString> &baseIEDList, QVector<QString> &otherIEDList, QVector<QString> &vIEDList, DiffStates parentState);
	
	bool GetInputsDiff(QVector<QVirtualLink> &baseInputs, QVector<QVirtualLink> &otherInputs, IEDStru& strIED, DiffStates parentState);
	
	bool GetVLinkDiff(QVector<QVirtualLink> &baseInputs, QVector<QVirtualLink> &otherInputs, IEDStru& strIED, DiffStates parentState);

	bool GetPhyPortDiff(QVector<QString> &basePhyPortList, QVector<QString> &otherPhyPortList, IEDStru& strIED, DiffStates parentState);
	
	// �����ж��ļ���Ϣ�Ƿ񱻹�ѡ
	bool GetFileInfoSelectedState();
	// �����ж�IED�Ƿ񱻹�ѡ
	bool GetIEDSelectedState();

	void ClearMap();

public:
	int  m_CompareSpeed;
	static SCDDiffCompare  *m_pSCDInstance;
	class CGarbo
	{  
	public:  ~CGarbo()  
			 {  
				 if (SCDDiffCompare::m_pSCDInstance) 
				 {
					 delete SCDDiffCompare::m_pSCDInstance;  
				 }
			 }
	};
	static CGarbo Garbo;

	SCDStru					m_SCDDiffResult;
	bool					ConfigureInfo[ITEMCOUNT];	// �洢������Ϣ

	DifferMap				m_IEDDiffMap;				// �洢IED��Ϣ��Map
	DifferMap				m_CBDiffMap;				// �洢���ƿ���Ϣ��Map
	DifferMap				m_PhyPortDiffMap;			// �洢����˿���Ϣ��Map
	DifferMap				m_VLinkDiffMap;				// �洢��������Ϣ��Map ����������ָ��IED��ص����е������ӣ���src��dest��ָIED��ȷ��
	DifferMap				m_InputsDiffMap;			// �洢Inputs��Ϣ��Map ��DestΪ��װ��
	DifferMap				m_InputsOtherDiffMap;			// �洢Inputs��Ϣ��Map ��DestΪ��װ��
	DifferMap				m_InSigDiffMap;				// �洢�����ź���Ϣ��Map
	DifferMap				m_OutSigDiffMap;			// �洢����ź���Ϣ��Map


	DifferMap				m_IEDUnNormalDiffMap;				// �洢�仯IED��Ϣ��Map
	DifferMap				m_SVCBUnNormalDiffMap;				// �洢���ƿ���Ϣ��Map
	DifferMap				m_GOOSECBUnNormalDiffMap;				// �洢���ƿ���Ϣ��Map
	DifferMap				m_PhyPortUnNormalDiffMap;			// �洢����˿���Ϣ��Map
	DifferMap				m_InputsUnNormalDiffMap;			// �洢Inputs��Ϣ��Map ��DestΪ��װ��
	DifferMap				m_InputsOtherUnNormalDiffMap;
	DifferMap				m_OutSigUnNormalDiffMap;			// �洢����ź���Ϣ��Map
	DifferMap				m_InSigUnNormalDiffMap;

	QString					m_BaseSubStationKey, m_OtherSubStationKey;
	SCLModelSvrAdapterInterface   *SCLModel;
private:
	
	SCLNameTranslator			  m_NameTranslator;

	void CreatMap();
	bool IsIEDSelect();
	bool IsSVCBSelected();
	bool IsSVDataSetSelected();
	bool IsGOOSECBSelected();
	bool IsGOOSEDataSetSelected();
	bool IsInputsSelected();
	bool IsPhyPortSelected();

	bool bIEDSelected;
	bool bSVCBSelected;
	bool bSVDataSetSelected;
	bool bGooseCBSelected;
	bool bGooseDataSetSelected;
	bool bInputsSelected;
	bool bPhyPortSelected;


	QString ToQString(QString qString);
	bool SetValue(ConfInfo conf, BasicStru &basicStru, QString baseAtt, QString otherAtt, DiffStates parentState);
	bool SetValue(ConfInfo conf,  BasicIntStruct &basicStru, int baseAtt, int otherAtt, DiffStates parentState);
	QString NameTranslator_GetKeyNameWithoutSubStation(QString qString);
	QString NameTranslator_GetIEDKeyFromSCDDiffResult(QString qString);

	// ����������Ϣ��
	typedef struct ConfPair 
	{
		QString		confEnDesc;
		QString		confChDesc;
	}CONFPAIR;
	CONFPAIR m_ConfPair[ITEMCOUNT];
	void	SetConfigureInfo();
	void	SetConfigureInfoPair(ConfInfo ord, QString EnDesc, QString ChDesc);
private:
    bool    BuildSignalCache(const std::string &strFileMark1, const std::string &strFileMark2);
};

#endif