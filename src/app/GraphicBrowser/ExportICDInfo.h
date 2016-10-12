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
	// ����ICD�汾��Ϣ���ó�Ա������
	Sheet* sheetICDInfo;
	QMap<QString, int>				m_ICDVerInfoKeyMap;
	QMap<QString, QIEDInfo>			m_ICDVerInfoMap;

	void CreatICDVerInfoMap();	// ����������
	void WriteICDVersionInfo();	// д��Sheet��
	void SaveICDVerInfoFile();	// �����ļ�
};