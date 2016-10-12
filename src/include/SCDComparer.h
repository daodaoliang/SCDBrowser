#ifndef SCDCOMPARER_H
#define SCDCOMPARER_H

#include <qstring>
#include <QVector>
#include "GlobalDef.h"
#include "SCLModelSvrAdapterInterface.h"

enum IEDDiffType
{
	IED_Equal = 0,
	IED_add,
	IED_delete,
	IED_update
};

struct IEDDiffStru 
{
	QString IEDKey;
	QString IEDName;
	QString IEDDesc;

	QString otherIEDKey;
	
	IEDDiffType diffType;
};

struct TerminalDiffStru
{

};

class SCDComparer
{
public:
	static SCDComparer * GetInstance() 
	   {  
		   if (m_pInstance == NULL)
			   m_pInstance = new SCDComparer();  
		   return m_pInstance; 
	   };
	SCDComparer();
	~SCDComparer();

	//比较所有IED的差异
	void LoadDiffVector();
	QVector<IEDDiffStru> GetDiffVector();
	
	//信息流图中的比较
	QVector<QString> GetAddedOtherIEDs(QVector<QString> *baseOtherIEDs, QVector<QString> *otherOtherIEDs);		//获取增加的关联IED
	bool isVLinksModified(const QVector<QVirtualLink> &baseVLinks, const QVector<QVirtualLink> &otherVLinks);	//比较虚回路是否有差异

	//虚回路图中的比较
	QList<int> GetDiffVTList(const QList<VirtualTerminal> &baseMasterList, const QList<VirtualTerminal> &baseSlaveList, 
							const QList<VirtualTerminal> &otherMasterList, const QList<VirtualTerminal> &otherSlaveList, 
							QList<VirtualTerminal> *allVTMasterList, QList<VirtualTerminal> *allVTSlaveLis);

	//设置&取出要比较的两个变电站Key
	QString GetBaseSubKey();
	QString GetOtherSubKey();
	void setSubstationKeys(const QString baseKey, const QString otherKey);
private:
	static SCDComparer *m_pInstance;
	class CGarbo
	{  
	public:  ~CGarbo()  
			 {  
				 if (SCDComparer::m_pInstance) 
				 {
					 delete SCDComparer::m_pInstance;  
				 }
			 }
	};
	static CGarbo Garbo;
	SCLModelSvrAdapterInterface   *SCLModel;

	QString m_BaseSubKey;
	QString m_OtherSubKey;
	QVector<IEDDiffStru> iedDiffVec;
};

#endif