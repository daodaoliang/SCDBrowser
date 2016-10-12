/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      TabInfoExporter.h
  * Author:         ţ����;
  * Version:        1.0.0
  * Date:           2015/1/19
  * Description:   Excel��񵼳���;
  */ 
#ifndef TABINFOEXPORTER_H
#define TABINFOEXPORTER_H

#include "TabInfoDataSetter.h"
#include "libxl.h"
using namespace libxl;

class TabInfoExporter
{
public:
	TabInfoExporter(TabInfoDataSetter* dataSetter);
	virtual ~TabInfoExporter(void);

	bool ExportXLSFile();

private:
	void setFormats();
	void writeBasicInfo();
	void writeDatas();
	bool saveXLSFile();

private:
	Book* m_Book;						//Bookָ�룬ָ��Excel�ļ�;
	Sheet* m_CurrentSheet;				//Sheetָ�룬ָ��һ����ǰSheetҳ;

	libxl::Font* m_HeaderFont;			//������Ϣ����;
	libxl::Font* m_ColumnFont;			//��������;
	libxl::Font* m_CellFont;			//��ͨ��Ԫ������;

	Format* m_HeaderFormat;				//������Ϣ��Ԫ���ʽ;
	Format* m_ColumnFormat;				//������Ԫ���ʽ;
	Format* m_CellFormat;				//��ͨ��Ԫ���ʽ;

	TabInfoDataSetter* m_DataSetter;	//���������ָ��;
};

#endif
