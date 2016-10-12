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
	
	// 用来判断文件信息是否被勾选
	bool GetFileInfoSelectedState();
	// 用来判断IED是否被勾选
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
	bool					ConfigureInfo[ITEMCOUNT];	// 存储配置信息

	DifferMap				m_IEDDiffMap;				// 存储IED信息的Map
	DifferMap				m_CBDiffMap;				// 存储控制块信息的Map
	DifferMap				m_PhyPortDiffMap;			// 存储物理端口信息的Map
	DifferMap				m_VLinkDiffMap;				// 存储虚链接信息的Map ：虚链接是指与IED相关的所有的虚链接，即src，dest所指IED不确定
	DifferMap				m_InputsDiffMap;			// 存储Inputs信息的Map ：Dest为本装置
	DifferMap				m_InputsOtherDiffMap;			// 存储Inputs信息的Map ：Dest为本装置
	DifferMap				m_InSigDiffMap;				// 存储输入信号信息的Map
	DifferMap				m_OutSigDiffMap;			// 存储输出信号信息的Map


	DifferMap				m_IEDUnNormalDiffMap;				// 存储变化IED信息的Map
	DifferMap				m_SVCBUnNormalDiffMap;				// 存储控制块信息的Map
	DifferMap				m_GOOSECBUnNormalDiffMap;				// 存储控制块信息的Map
	DifferMap				m_PhyPortUnNormalDiffMap;			// 存储物理端口信息的Map
	DifferMap				m_InputsUnNormalDiffMap;			// 存储Inputs信息的Map ：Dest为本装置
	DifferMap				m_InputsOtherUnNormalDiffMap;
	DifferMap				m_OutSigUnNormalDiffMap;			// 存储输出信号信息的Map
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

	// 设置配置信息对
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