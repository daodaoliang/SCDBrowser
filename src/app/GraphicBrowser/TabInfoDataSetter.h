/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      TabInfoDataSetter.h
  * Author:         ţ����;
  * Version:        1.0.0
  * Date:           2015/1/19
  * Description:   ������Ҫ�����ӿؼ���ȡ���ݲ����ݸ�������;
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
	QStringList m_MainColumnNames;			//���и�������;
	QStringList m_SubsidiaryColumnNames;	//���и�������;
	QMap<QString,QStringList> m_MainData;	//���������;
	QMap<QString,QVector<QStringList>> m_SubsidiaryData;	//��丱����;
	QString m_SheetName;					//sheetҳ��;
	QString m_IEDNameDesc;					//��IED������������; 

};

#endif
