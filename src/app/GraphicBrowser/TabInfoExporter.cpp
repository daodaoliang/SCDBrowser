#include "TabInfoExporter.h"

#include <QString>
#include <QFileDialog>
#include <QMessageBox>

/** 
  * @brief    TabInfoExporter
  * 
  * ���캯��;
  * 
  * @class    TabInfoExporter
  * @param    TabInfoDataSetter* dataSetter : ������ָ��;
  * @param    
  * @return   
  * @author   ţ����;
  * @date     2015/1/19
  * @remarks  
  */ 
TabInfoExporter::TabInfoExporter(TabInfoDataSetter* dataSetter)
{
	m_DataSetter = dataSetter;
	m_Book = xlCreateBook();
	if (m_Book)
	{
		//ע:��˾�������кŲ��ɱ�;
		m_Book->setKey(L"�����ݸ��Ƽ����޹�˾", L"windows-26292c0705cde60766bc6667a2hce3l9");
	}
}

/** 
  * @brief    ~TabInfoExporter
  * 
  * ��������;
  * 
  * @class    TabInfoExporter
  * @param    void :
  * @param    
  * @return   
  * @author   ţ����;
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
  * ����Excel����;
  * 
  * @class    TabInfoExporter
  * @param    
  * @return   bool �Ƿ񵼳��ɹ�;
  * @author   ţ����;
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
  * ���ø���Ԫ���ʽ;
  * 
  * @class    TabInfoExporter
  * @param    
  * @return   void
  * @author   ţ����;
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

	m_CellFont->setName(L"����");
	m_CellFont->setSize(9);

	m_ColumnFont->setBold(true);
	m_ColumnFont->setName(L"����");
	m_ColumnFont->setSize(10);

	m_HeaderFont->setBold(true);
	m_HeaderFont->setName(L"����");
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
  * д���������ͷ;
  * 
  * @class    TabInfoExporter
  * @param    
  * @return   void
  * @author   ţ����;
  * @date     2015/1/19
  * @remarks  
  */ 
void TabInfoExporter::writeBasicInfo()
{
	QString titleStr = m_DataSetter->getIEDNameDesc();
	QString sheetName = m_DataSetter->getSheetName();
	int allColumnCount = m_DataSetter->getMainColumnNames().size() + m_DataSetter->getSubsidiaryColumnNames().size();

	//д�����;
	m_CurrentSheet->setRow(0,25);
	m_CurrentSheet->setMerge(0, 0, 0, allColumnCount - 1);
	m_CurrentSheet->writeStr(0, 0, reinterpret_cast<const wchar_t*>(titleStr.utf16()), m_HeaderFormat);
	
	//д��������;
	int columnNum = 0;		//�к�;
	for (uint i = 0; i != m_DataSetter->getMainColumnNames().size(); i++)
	{
		m_CurrentSheet->writeStr(1, columnNum, reinterpret_cast<const wchar_t*>(m_DataSetter->getMainColumnNames().at(i).utf16()), m_ColumnFormat);
		m_CurrentSheet->setCol(columnNum, columnNum, 12);
		columnNum++;
	}

	//д�븱����;
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
  * д������;
  * 
  * @class    TabInfoExporter
  * @param    
  * @return   void
  * @author   ţ����;
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
		//�����������;
		for (uint mainColIdx = 0; mainColIdx != tempMainColList.size(); mainColIdx++)
		{
			m_CurrentSheet->setMerge(tempRow, tempRow + subsInfoCount - 1, tempCol, tempCol);
			m_CurrentSheet->writeStr(tempRow, tempCol, reinterpret_cast<const wchar_t*>(tempMainColList.at(mainColIdx).utf16()), m_CellFormat);
			tempCol++;
		}

		int subsColFirstNum = tempCol;
		//��丱������;
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

	//�������ݵ�Ԫ���ʽ;
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
  * ����Excel�ļ�;
  * 
  * @class    TabInfoExporter
  * @param    
  * @return   bool �Ƿ񱣴�ɹ�;
  * @author   ţ����;
  * @date     2015/1/19
  * @remarks  
  */ 
bool TabInfoExporter::saveXLSFile()
{
	QString tempFileName = m_DataSetter->getIEDNameDesc() + QString(" �� ") + m_DataSetter->getSheetName();
	QString fileName = QFileDialog::getSaveFileName(NULL, "����", tempFileName, "*.xls");  
	if (!fileName.isEmpty())  
	{  
		if (m_Book->save(reinterpret_cast<const wchar_t*>(fileName.utf16())))
		{
			QMessageBox::about(NULL, "�����ļ�", "�ļ�����ɹ���");
			return true;
		}
		else
		{
			QMessageBox::warning(NULL, "�����ļ�", "�ļ�����ʧ��,�����ļ��Ƿ������������д�!");
			return false;
		}
	}
	return false;
}
