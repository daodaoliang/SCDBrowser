/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      TabInfoExporter.h
  * Author:         牛子明;
  * Version:        1.0.0
  * Date:           2015/1/19
  * Description:   Excel表格导出类;
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
	Book* m_Book;						//Book指针，指代Excel文件;
	Sheet* m_CurrentSheet;				//Sheet指针，指代一个当前Sheet页;

	libxl::Font* m_HeaderFont;			//标题信息字体;
	libxl::Font* m_ColumnFont;			//列名字体;
	libxl::Font* m_CellFont;			//普通单元格字体;

	Format* m_HeaderFormat;				//标题信息单元格格式;
	Format* m_ColumnFormat;				//列名单元格格式;
	Format* m_CellFormat;				//普通单元格格式;

	TabInfoDataSetter* m_DataSetter;	//数据类对象指针;
};

#endif
