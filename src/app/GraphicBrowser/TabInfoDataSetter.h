/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      TabInfoDataSetter.h
  * Author:         牛子明;
  * Version:        1.0.0
  * Date:           2015/1/19
  * Description:   该类主要用来从控件获取数据并传递给导出类;
  */ 
#ifndef TABINFODATASETTER_H
#define TABINFODATASETTER_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QVector>

class TabInfoDataSetter
{
public:
	TabInfoDataSetter(const QString& sheetName);
	virtual ~TabInfoDataSetter(void);

	bool ExportXLS();

	void setSheetName(const QString& sheetName);
	QString& getSheetName();

	void setIEDNameDesc(const QString& IEDNameDesc);
	QString& getIEDNameDesc();

	void setMainColumnNames(const QStringList& mainColumnNames);
	QStringList& getMainColumnNames();
	
	void setSubsidiaryColumnNames(const QStringList& subsidiaryColumnNames);
	QStringList& getSubsidiaryColumnNames();

	void setMainData(const QMap<QString,QStringList> mainData);
	QMap<QString,QStringList>& getMainDataMap();
	
	void setSubsidiaryData(const QMap<QString,QVector<QStringList>> subsidiaryData);
	QMap<QString,QVector<QStringList>>& getSubsidiaryDataMap();

private:
	QStringList m_MainColumnNames;			//主列各列列名;
	QStringList m_SubsidiaryColumnNames;	//副列各列列名;
	QMap<QString,QStringList> m_MainData;	//填充主内容;
	QMap<QString,QVector<QStringList>> m_SubsidiaryData;	//填充副内容;
	QString m_SheetName;					//sheet页名;
	QString m_IEDNameDesc;					//该IED的名称与描述; 

};

#endif
