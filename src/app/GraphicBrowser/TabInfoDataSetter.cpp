#include "TabInfoDataSetter.h"
#include "TabInfoExporter.h"

/** 
  * @brief    TabInfoDataSetter
  * 
  * 构造函数,初始化各成员;
  * 
  * @class    TabInfoDataSetter
  * @param    const QString& sheetName   : sheet页名;
  * @param    
  * @return   无;
  * @author   牛子明;
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
  * 析构函数;
  * 
  * @class    TabInfoDataSetter
  * @param    void :
  * @param    
  * @return   
  * @author   牛子明;
  * @date     2015/1/19
  * @remarks  
  */ 
TabInfoDataSetter::~TabInfoDataSetter(void)
{
}

/** 
  * @brief    ExportXLS
  * 
  * 导出Excel报表
  * 
  * @class    TabInfoDataSetter
  * @param    
  * @return   bool
  * @author   牛子明;
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
  * 设置sheet页名;
  * 
  * @class    TabInfoDataSetter
  * @param    const QString& sheetName : sheet页名;
  * @param    
  * @return   void
  * @author   牛子明;
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
  * 设置IED名与描述;
  * 
  * @class    TabInfoDataSetter
  * @param    const QString& IEDNameDesc : IED名与描述;
  * @param    
  * @return   void
  * @author   牛子明;
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
  * 设置主列各列名;
  * 
  * @class    TabInfoDataSetter
  * @param    const QStringList& mainColumnNames : 主列列名列表;
  * @param    
  * @return   void
  * @author   牛子明;
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
  * 设置副列各列名;
  * 
  * @class    TabInfoDataSetter
  * @param    const QStringList& subsidiaryColumnNames : 副列列名列表;
  * @param    
  * @return   void
  * @author   牛子明;
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
  * 设置主列填充值;
  * 
  * @class    TabInfoDataSetter
  * @param    const QMap<QString,QStringList> mainData : 主列填充值Map;
  * @param    
  * @return   void
  * @author   牛子明;
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
  * 设置副列填充值;
  * 
  * @class    TabInfoDataSetter
  * @param    const QMap<QString,QVector<QStringList>> subsidiaryData : 主列填充值Map
  * @param    
  * @return   void
  * @author   牛子明;
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
  * 获取IED名与描述字符串;
  * 
  * @class    TabInfoDataSetter
  * @param    
  * @return   QString& IED名与描述;
  * @author   牛子明;
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
  * 获取sheet页名字符串;
  * 
  * @class    TabInfoDataSetter
  * @param    
  * @return   QString& sheet页名;
  * @author   牛子明;
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
  * 获取主列列名;
  * 
  * @class    TabInfoDataSetter
  * @param    
  * @return   QStringList& 主列列名列表;
  * @author   牛子明;
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
  * 获取副列列名
  * 
  * @class    TabInfoDataSetter
  * @param    
  * @return   QStringList& 副列列名列表;
  * @author   牛子明;
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
  * 获取主列填充值;
  * 
  * @class    TabInfoDataSetter
  * @param    
  * @return   QMap<QString,QStringList>& 值Map;
  * @author   牛子明;
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
  * 获取副列填充值;
  * 
  * @class    TabInfoDataSetter
  * @param    
  * @return   QMap<QString,QVector<QStringList>>& 值Map;
  * @author   牛子明;
  * @date     2015/1/19
  * @remarks  
  */ 
QMap<QString,QVector<QStringList>>& TabInfoDataSetter::getSubsidiaryDataMap()
{
	return m_SubsidiaryData;
}
