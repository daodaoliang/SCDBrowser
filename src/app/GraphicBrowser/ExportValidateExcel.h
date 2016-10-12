///** 
//  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
//  * File name:      ExportValidateExcel.h
//  * Author:         zhang-xch
//  * Version:        V1.0
//  * Date:           2013/12/25
//  * Description:   ������Ҫ����������ǰSCD�ļ����Ŀ
//  */ 
//#pragma once
//
//#include "SCLModelSvrAdapter.h"
//
//#include "libxl.h"
//using namespace libxl;
//
//struct  ItemInfoStr
//{
//	QString name;
//	QString type;
//	QString fileName;
//	int count;
//};
//
//class CExportValidateExcel
//{
//public:
//	CExportValidateExcel(QString SubStationKey);
//	virtual ~CExportValidateExcel(void);
//
//    void run();
//
//private:
//	SCLModelSvrAdapterInterface		*SCLModel;
//	QString                         m_SubStationKey;
//	uint						    itemCount;
//	QSubStationInfo					m_subStationInfo;
//	QString							basicInfo;
//
//	Book* book;
//	Sheet* sheetValidateInfo;
//	libxl::Font* headerFont;
//	libxl::Font* contextFont;
//	Format* headerFormat;
//	Format* contextFormat;
//
//	void WriteBasicInfo();
//	void SetFormat();
//	void WriteValidateInfo();	// д��Sheet��
//	void SaveValidateInfoToFile();	// �����ļ�
//	int InsertItem(FILE *fp, QString type);
//	void OpenFile(ItemInfoStr &item);
//};
//
