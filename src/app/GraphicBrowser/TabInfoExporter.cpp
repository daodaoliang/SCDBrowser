#include "TabInfoExporter.h"

#include <QString>
#include <QFileDialog>
#include <QMessageBox>

/** 
  * @brief    TabInfoExporter
  * 
  * 构造函数;
  * 
  * @class    TabInfoExporter
  * @param    TabInfoDataSetter* dataSetter : 数据类指针;
  * @param    
  * @return   
  * @author   牛子明;
  * @date     2015/1/19
  * @remarks  
  */ 
TabInfoExporter::TabInfoExporter(TabInfoDataSetter* dataSetter)
{
	m_DataSetter = dataSetter;
	m_Book = xlCreateBook();
	if (m_Book)
	{
		//注:公司名与序列号不可变;
		m_Book->setKey(L"济南容弗科技有限公司", L"windows-26292c0705cde60766bc6667a2hce3l9");
	}
}

/** 
  * @brief    ~TabInfoExporter
  * 
  * 析构函数;
  * 
  * @class    TabInfoExporter
  * @param    void :
  * @param    
  * @return   
  * @author   牛子明;
  * @date     2015/1/19
  * @remarks  
  */ 
TabInfoExporter::~TabInfoExporter(void)
{
	m_Book->release();
}

/** 
  * @brief    ExportXLSFile
  * 
  * 导出Excel报表;
  * 
  * @class    TabInfoExporter
  * @param    
  * @return   bool 是否导出成功;
  * @author   牛子明;
  * @date     2015/1/19
  * @remarks  
  */ 
bool TabInfoExporter::ExportXLSFile()
{
	bool isSuc;
	m_CurrentSheet = m_Book->addSheet(reinterpret_cast<const wchar_t*>(m_DataSetter->getSheetName().utf16()));
	setFormats();
	writeBasicInfo();
	writeDatas();
	isSuc = saveXLSFile();

	return isSuc;
}

/** 
  * @brief    setFormats
  * 
  * 设置各单元格格式;
  * 
  * @class    TabInfoExporter
  * @param    
  * @return   void
  * @author   牛子明;
  * @date     2015/1/19
  * @remarks  
  */ 
void TabInfoExporter::setFormats()
{
	m_CellFont = m_Book->addFont();
	m_ColumnFont = m_Book->addFont();
	m_HeaderFont = m_Book->addFont();

	m_CellFormat = m_Book->addFormat();
	m_ColumnFormat = m_Book->addFormat();
	m_HeaderFormat = m_Book->addFormat();

	m_CellFont->setName(L"宋体");
	m_CellFont->setSize(9);

	m_ColumnFont->setBold(true);
	m_ColumnFont->setName(L"宋体");
	m_ColumnFont->setSize(10);

	m_HeaderFont->setBold(true);
	m_HeaderFont->setName(L"宋体");
	m_HeaderFont->setSize(16);

	m_CellFormat->setFont(m_CellFont);
	m_CellFormat->setBorder(BORDERSTYLE_THIN);
	m_CellFormat->setAlignH(ALIGNH_CENTER);
	m_CellFormat->setAlignV(ALIGNV_CENTER);
	m_CellFormat->setWrap(true);

	m_ColumnFormat->setFont(m_ColumnFont);
	m_ColumnFormat->setBorder(BORDERSTYLE_THIN);
	m_ColumnFormat->setAlignH(ALIGNH_CENTER);
	m_ColumnFormat->setAlignV(ALIGNV_CENTER);
	m_ColumnFormat->setWrap(true);

	m_HeaderFormat->setFont(m_HeaderFont);
	m_HeaderFormat->setBorder(BORDERSTYLE_THIN);
	m_HeaderFormat->setAlignV(ALIGNV_CENTER);
	m_HeaderFormat->setAlignH(ALIGNH_CENTER);
	m_HeaderFormat->setWrap(true);
}

/** 
  * @brief    writeBasicInfo
  * 
  * 写入标题与列头;
  * 
  * @class    TabInfoExporter
  * @param    
  * @return   void
  * @author   牛子明;
  * @date     2015/1/19
  * @remarks  
  */ 
void TabInfoExporter::writeBasicInfo()
{
	QString titleStr = m_DataSetter->getIEDNameDesc();
	QString sheetName = m_DataSetter->getSheetName();
	int allColumnCount = m_DataSetter->getMainColumnNames().size() + m_DataSetter->getSubsidiaryColumnNames().size();

	//写入标题;
	m_CurrentSheet->setRow(0,25);
	m_CurrentSheet->setMerge(0, 0, 0, allColumnCount - 1);
	m_CurrentSheet->writeStr(0, 0, reinterpret_cast<const wchar_t*>(titleStr.utf16()), m_HeaderFormat);
	
	//写入主列名;
	int columnNum = 0;		//列号;
	for (uint i = 0; i != m_DataSetter->getMainColumnNames().size(); i++)
	{
		m_CurrentSheet->writeStr(1, columnNum, reinterpret_cast<const wchar_t*>(m_DataSetter->getMainColumnNames().at(i).utf16()), m_ColumnFormat);
		m_CurrentSheet->setCol(columnNum, columnNum, 12);
		columnNum++;
	}

	//写入副列名;
	for (uint j = 0; j != m_DataSetter->getSubsidiaryColumnNames().size(); j++)
	{
		m_CurrentSheet->writeStr(1, columnNum, reinterpret_cast<const wchar_t*>(m_DataSetter->getSubsidiaryColumnNames().at(j).utf16()), m_ColumnFormat);
		m_CurrentSheet->setCol(columnNum, columnNum, 25);
		columnNum++;
	}
}

/** 
  * @brief    writeDatas
  * 
  * 写入数据;
  * 
  * @class    TabInfoExporter
  * @param    
  * @return   void
  * @author   牛子明;
  * @date     2015/1/19
  * @remarks  
  */ 
void TabInfoExporter::writeDatas()
{
	int tempRow = 2;
	int colCount = 0;
	QMap<QString,QStringList>::iterator mainIter = m_DataSetter->getMainDataMap().begin();
	QMap<QString,QVector<QStringList>>::iterator subsIter = m_DataSetter->getSubsidiaryDataMap().begin();
	while (mainIter != m_DataSetter->getMainDataMap().end())
	{
		QStringList tempMainColList = mainIter.value();
		QVector<QStringList> tempSubsVec = subsIter.value();
		int subsInfoCount = tempSubsVec.size();
		int tempCol = 0;
		//填充主列数据;
		for (uint mainColIdx = 0; mainColIdx != tempMainColList.size(); mainColIdx++)
		{
			m_CurrentSheet->setMerge(tempRow, tempRow + subsInfoCount - 1, tempCol, tempCol);
			m_CurrentSheet->writeStr(tempRow, tempCol, reinterpret_cast<const wchar_t*>(tempMainColList.at(mainColIdx).utf16()), m_CellFormat);
			tempCol++;
		}

		int subsColFirstNum = tempCol;
		//填充副列数据;
		for (uint subsVecIdx = 0; subsVecIdx != subsInfoCount; subsVecIdx++)
		{
			if (tempCol != subsColFirstNum)
			{
				tempCol = subsColFirstNum;
			}
			QStringList tempSubColList = tempSubsVec.at(subsVecIdx);
			for (uint subsColIdx = 0; subsColIdx != tempSubColList.size(); subsColIdx++)
			{
				m_CurrentSheet->writeStr(tempRow, tempCol, reinterpret_cast<const wchar_t*>(tempSubColList.at(subsColIdx).utf16()), m_CellFormat);
				tempCol++;	
			}
			tempRow++;
		}
		colCount = tempCol - 1;
		mainIter++;
		subsIter++;
	}

	//设置数据单元格格式;
	for (uint fRIdx = 2; fRIdx != tempRow; fRIdx++)
	{
		for (uint fCIdx = 0; fCIdx != colCount; fCIdx++)
		{
			m_CurrentSheet->setCellFormat(fRIdx, fCIdx, m_CellFormat);
		}
	}
}

/** 
  * @brief    saveXLSFile
  * 
  * 保存Excel文件;
  * 
  * @class    TabInfoExporter
  * @param    
  * @return   bool 是否保存成功;
  * @author   牛子明;
  * @date     2015/1/19
  * @remarks  
  */ 
bool TabInfoExporter::saveXLSFile()
{
	QString tempFileName = m_DataSetter->getIEDNameDesc() + QString(" 的 ") + m_DataSetter->getSheetName();
	QString fileName = QFileDialog::getSaveFileName(NULL, "保存", tempFileName, "*.xls");  
	if (!fileName.isEmpty())  
	{  
		if (m_Book->save(reinterpret_cast<const wchar_t*>(fileName.utf16())))
		{
			QMessageBox::about(NULL, "保存文件", "文件保存成功！");
			return true;
		}
		else
		{
			QMessageBox::warning(NULL, "保存文件", "文件保存失败,请检查文件是否在其他程序中打开!");
			return false;
		}
	}
	return false;
}
