#pragma once
#include "SCDDiffCommon.h"

#include "libxl.h"
using namespace libxl;

#include <QtCore/qglobal.h>
#include <QString>
#include "SCDDifferCompare.h"

class CXLCreat
{
public:
	CXLCreat(void);
	~CXLCreat(void);

	void Run();
private:

	void CreatFormat();
	void SetFormat();
	void SaveFile();

	void CreatDocInfoSheet();
	void CreatIEDDiffSheet();
	void CreatIEDEditDiffSheet();
	void AddFileInfoTableHead(Sheet* pSheet);
	void AddIEDDiffTableHead(Sheet* pSheet);
	void AddIEDEditTableHead(Sheet* pSheet);

	struct Point
	{
		int row;
		int col;
	};

	Point m_CurentPoint;

	SCDDiffCompare *m_SCDDiffCompare;
	
	void ExportFileInFoDiff();
	void ExportIEDDiff();
	void ExportSVDiff(IEDStru *pIED);
	void ExportGooseDiff(IEDStru *pIED);
	void ExportInputsDiff(IEDStru *pIED);
	void ExportPhyPortDiff(IEDStru *pIED);

	void ExportSignaleDiff(QString IEDName, GOOSECBStru *pGoose);
	void ExportSignaleDiff(QString IEDName, SVCBStru *pSV);
	void ExportExtRefDiff(QString IEDName, InputStru *pInput);

	void InitPoint();
	void SetFileInfoDiffAttributeValue(BasicStru pBasicStr, QString AttributeName);
	void SetIEDDiffAttributeValue(IEDStru* pIED);
	void SetIEDEditDiffAttributeValue(QString IEDName, QString DiffType, QString qObject, QString Path, QString AttributeName, BasicStru pBasicStr);
	void SetIEDEditDiffAttributeIntValue(QString IEDName, QString DiffType, QString qObject, QString Path, QString AttributeName, BasicIntStru pBasicStr);

private:
	QString NameTranslator_GetObjNameFromSCDDiffResult(QString qString);
	Book* book;
	Sheet* sheetDocInfo;
	Sheet* sheetIEDDiff;
	Sheet* sheetIEDEdit;
	SCLNameTranslator m_NameTranslator;

	libxl::Font* attrabuteValueFont;
	libxl::Font* headerFont;
	Format* headerFormat;
	Format* pFormat;
	Format* attrabuteValueFormat;
	Format* attrabute2ValueFormat;
	Format* attrabute1ValueFormat;

	int iOrd;
	int iCount;
};

