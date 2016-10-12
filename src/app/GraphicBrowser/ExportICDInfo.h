#pragma once

#include <QtCore/qglobal.h>
#include <QString>
#include "SCLModelSvrAdapter.h"
#include "SCLNameTranslator.h"

#include "libxl.h"
using namespace libxl;

class CExportICDInfo
{
public:
	CExportICDInfo();
	~CExportICDInfo();

	void ExportICDVersionInfo();
	void SetSubStationKey(QString key);

private:
	SCLModelSvrAdapterInterface		*SCLModel;
	QString							m_SubStationKey;
	SCLNameTranslator				m_NameTranslator;
	QVector<QString>				m_IEDList;

private:
	Book* book;
	
	libxl::Font* contextFont;
	libxl::Font* blockFont;
	libxl::Font* headerFont;

	Format* headerFormat;
	Format* blockFormat;
	Format* contextFormat;

	void WriteBasicInfo();
	void SetFormat();

private:
	// 导出ICD版本信息所用成员及函数
	Sheet* sheetICDInfo;
	QMap<QString, int>				m_ICDVerInfoKeyMap;
	QMap<QString, QIEDInfo>			m_ICDVerInfoMap;

	void CreatICDVerInfoMap();	// 创建索引表
	void WriteICDVersionInfo();	// 写入Sheet中
	void SaveICDVerInfoFile();	// 保存文件
};