#include "TabInfoDataSetter.h"
#include "TabInfoExporter.h"

/** 
  * @brief    TabInfoDataSetter
  * 
  * ���캯��,��ʼ������Ա;
  * 
  * @class    TabInfoDataSetter
  * @param    const QString& sheetName   : sheetҳ��;
  * @param    
  * @return   ��;
  * @author   ţ����;
  * @date     2015/1/19
  * @remarks  
  */ 
TabInfoDataSetter::TabInfoDataSetter(const QString& sheetName)
{
	m_SheetName = sheetName;
	m_IEDNameDesc = "";

	m_MainColumnNames.clear();
	m_SubsidiaryColumnNames.clear();
	
	m_MainData.clear();
	m_SubsidiaryData.clear();
}


/** 
  * @brief    ~TabInfoDataSetter
  * 
  * ��������;
  * 
  * @class    TabInfoDataSetter
  * @param    void :
  * @param    
  * @return   
  * @author   ţ����;
  * @date     2015/1/19
  * @remarks  
  */ 
TabInfoDataSetter::~TabInfoDataSetter(void)
{
}

/** 
  * @brief    ExportXLS
  * 
  * ����Excel����
  * 
  * @class    TabInfoDataSetter
  * @param    
  * @return   bool
  * @author   ţ����;
  * @date     2015/1/19
  * @remarks  
  */ 
bool TabInfoDataSetter::ExportXLS()
{
	TabInfoExporter tIExporter(this);
	return tIExporter.ExportXLSFile();
}

/** 
  * @brief    setSheetName
  * 
  * ����sheetҳ��;
  * 
  * @class    TabInfoDataSetter
  * @param    const QString& sheetName : sheetҳ��;
  * @param    
  * @return   void
  * @author   ţ����;
  * @date     2015/1/20
  * @remarks  
  */ 
void TabInfoDataSetter::setSheetName(const QString& sheetName)
{
	m_SheetName = sheetName;
}

/** 
  * @brief    setIEDNameDesc
  * 
  * ����IED��������;
  * 
  * @class    TabInfoDataSetter
  * @param    const QString& IEDNameDesc : IED��������;
  * @param    
  * @return   void
  * @author   ţ����;
  * @date     2015/1/20
  * @remarks  
  */ 
void TabInfoDataSetter::setIEDNameDesc(const QString& IEDNameDesc)
{
	m_IEDNameDesc = IEDNameDesc;
}

/** 
  * @brief    setMainColumnNames
  * 
  * �������и�����;
  * 
  * @class    TabInfoDataSetter
  * @param    const QStringList& mainColumnNames : ���������б�;
  * @param    
  * @return   void
  * @author   ţ����;
  * @date     2015/1/19
  * @remarks  
  */ 
void TabInfoDataSetter::setMainColumnNames(const QStringList& mainColumnNames)
{
	m_MainColumnNames.clear();
	for (uint i = 0; i != mainColumnNames.size(); i++)
	{
		m_MainColumnNames.append(mainColumnNames.at(i));
	}
}

/** 
  * @brief    setSubsidiaryColumnNames
  * 
  * ���ø��и�����;
  * 
  * @class    TabInfoDataSetter
  * @param    const QStringList& subsidiaryColumnNames : ���������б�;
  * @param    
  * @return   void
  * @author   ţ����;
  * @date     2015/1/19
  * @remarks  
  */ 
void TabInfoDataSetter::setSubsidiaryColumnNames(const QStringList& subsidiaryColumnNames)
{
	m_SubsidiaryColumnNames.clear();
	for (uint i = 0; i != subsidiaryColumnNames.size(); i++)
	{
		m_SubsidiaryColumnNames.append(subsidiaryColumnNames.at(i));
	}
}

/** 
  * @brief    setMainData
  * 
  * �����������ֵ;
  * 
  * @class    TabInfoDataSetter
  * @param    const QMap<QString,QStringList> mainData : �������ֵMap;
  * @param    
  * @return   void
  * @author   ţ����;
  * @date     2015/1/19
  * @remarks  
  */ 
void TabInfoDataSetter::setMainData(const QMap<QString,QStringList> mainData)
{
	m_MainData.clear();
	for (QMap<QString,QStringList>::iterator iter = mainData.begin(); iter != mainData.end(); iter++)
	{
		m_MainData.insert(iter.key(), iter.value());
	}
}

/** 
  * @brief    setSubsidiaryData
  * 
  * ���ø������ֵ;
  * 
  * @class    TabInfoDataSetter
  * @param    const QMap<QString,QVector<QStringList>> subsidiaryData : �������ֵMap
  * @param    
  * @return   void
  * @author   ţ����;
  * @date     2015/1/19
  * @remarks  
  */ 
void TabInfoDataSetter::setSubsidiaryData(const QMap<QString,QVector<QStringList>> subsidiaryData)
{
	m_SubsidiaryData.clear();
	for (QMap<QString,QVector<QStringList>>::iterator iter = subsidiaryData.begin(); iter != subsidiaryData.end(); iter++)
	{
		m_SubsidiaryData.insert(iter.key(), iter.value());
	}
}

/** 
  * @brief    getIEDNameDesc
  * 
  * ��ȡIED���������ַ���;
  * 
  * @class    TabInfoDataSetter
  * @param    
  * @return   QString& IED��������;
  * @author   ţ����;
  * @date     2015/1/19
  * @remarks  
  */ 
QString& TabInfoDataSetter::getIEDNameDesc()
{
	return m_IEDNameDesc;
}

/** 
  * @brief    getSheetName
  * 
  * ��ȡsheetҳ���ַ���;
  * 
  * @class    TabInfoDataSetter
  * @param    
  * @return   QString& sheetҳ��;
  * @author   ţ����;
  * @date     2015/1/19
  * @remarks  
  */ 
QString& TabInfoDataSetter::getSheetName()
{
	return m_SheetName;
}

/** 
  * @brief    getMainColumnNames
  * 
  * ��ȡ��������;
  * 
  * @class    TabInfoDataSetter
  * @param    
  * @return   QStringList& ���������б�;
  * @author   ţ����;
  * @date     2015/1/19
  * @remarks  
  */ 
QStringList& TabInfoDataSetter::getMainColumnNames()
{
	return m_MainColumnNames;
}

/** 
  * @brief    getSubsidiaryColumnNames
  * 
  * ��ȡ��������
  * 
  * @class    TabInfoDataSetter
  * @param    
  * @return   QStringList& ���������б�;
  * @author   ţ����;
  * @date     2015/1/19
  * @remarks  
  */ 
QStringList& TabInfoDataSetter::getSubsidiaryColumnNames()
{
	return m_SubsidiaryColumnNames;
}

/** 
  * @brief    getMainDataMap
  * 
  * ��ȡ�������ֵ;
  * 
  * @class    TabInfoDataSetter
  * @param    
  * @return   QMap<QString,QStringList>& ֵMap;
  * @author   ţ����;
  * @date     2015/1/19
  * @remarks  
  */ 
QMap<QString,QStringList>& TabInfoDataSetter::getMainDataMap()
{
	return m_MainData;
}

/** 
  * @brief    getSubsidiaryDataMap
  * 
  * ��ȡ�������ֵ;
  * 
  * @class    TabInfoDataSetter
  * @param    
  * @return   QMap<QString,QVector<QStringList>>& ֵMap;
  * @author   ţ����;
  * @date     2015/1/19
  * @remarks  
  */ 
QMap<QString,QVector<QStringList>>& TabInfoDataSetter::getSubsidiaryDataMap()
{
	return m_SubsidiaryData;
}
