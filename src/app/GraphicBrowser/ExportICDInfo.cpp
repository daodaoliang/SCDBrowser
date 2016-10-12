#include "ExportICDInfo.h"
#include "SCLModelSvrAdapter.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDate>

CExportICDInfo::CExportICDInfo()
{
	SCLModel = SCLModelAdapter::GetInstance();

	book = xlCreateBook();
	if (book)
	{
		book->setKey(L"�����ݸ��Ƽ����޹�˾", L"windows-26292c0705cde60766bc6667a2hce3l9");
	}
}

CExportICDInfo::~CExportICDInfo()
{
	book->release();
}

/** 
  * @brief    SetSubStationKey
  * 
  * ���õ�ǰ���վ��key
  * 
  * @class    CExportICDInfo
  * @param    QString key : ��Frme�д��뵱ǰ���վKey
  * @return   void
  * @author   zhang-xch
  * @date     2014/9/11
  * @remarks  
  */ 
void CExportICDInfo::SetSubStationKey(QString key)
{
	m_SubStationKey = key;
}

/** 
  * @brief    SetFormat
  * 
  * ���õ�����ʽ
  * 
  * @class    CExportICDInfo
  * @return   void
  * @author   zhang-xch
  * @date     2014/9/9
  * @remarks  
  */ 
void CExportICDInfo::SetFormat()
{
	contextFont = book->addFont();
	blockFont = book->addFont();
	headerFont = book->addFont();

	contextFormat = book->addFormat();
	blockFormat = book->addFormat();
	headerFormat = book->addFormat();

	contextFont->setName(L"����");
	contextFont->setSize(9);

	blockFont->setBold(true);
	blockFont->setName(L"����");
	blockFont->setSize(10);
	
	headerFont->setBold();		
	headerFont->setName(L"����");
	headerFont->setSize(16);

	contextFormat->setFont(contextFont);
	contextFormat->setBorder(BORDERSTYLE_THIN);
	contextFormat->setAlignH(ALIGNH_CENTER);
	contextFormat->setAlignV(ALIGNV_CENTER);
	contextFormat->setWrap(true);

	blockFormat->setFont(blockFont);
	blockFormat->setBorder(BORDERSTYLE_THIN);
	blockFormat->setAlignH(ALIGNH_CENTER);
	blockFormat->setAlignV(ALIGNV_CENTER);
	blockFormat->setWrap(true);

	headerFormat->setFont(headerFont);
	headerFormat->setBorder(BORDERSTYLE_THIN);
	headerFormat->setAlignV(ALIGNV_CENTER);
	headerFormat->setAlignH(ALIGNH_CENTER);
	headerFormat->setWrap(true);
}

/** 
  * @brief    ExportBasicInfo
  * 
  * �����ļ�������Ϣ
  * 
  * @class    CExportICDInfo
  * @return   void
  * @author   zhang-xch
  * @date     2014/9/9
  * @remarks  
  */ 
void CExportICDInfo::WriteBasicInfo()
{
	QSubStationInfo subStationInfo;
	SCLModel->GetSubstationInfo(m_SubStationKey, subStationInfo);
	QString panelKey = m_SubStationKey + ".����";
	SCLModel->GetIEDList(panelKey, &m_IEDList);
	int rowSize = m_IEDList.size() + 8;

	for (int i = 0; i < rowSize; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			sheetICDInfo->setCellFormat(i, j, contextFormat);
		}
		sheetICDInfo->setRow(i, 25);
	}

	sheetICDInfo->setCol(0, 6, 12);
	sheetICDInfo->setRow(0, 28.8);
	sheetICDInfo->setMerge(0, 0, 0, 6);
	sheetICDInfo->writeStr(0, 0, L"ICD�汾��Ϣ", headerFormat);

	sheetICDInfo->setMerge(1, 1, 0, 1);
	sheetICDInfo->setMerge(1, 1, 2, 6);
	sheetICDInfo->writeStr(1, 0, L"�ʱ��", blockFormat);
	QString date = (QDate::currentDate()).toString("yyyy/MM/dd");
	sheetICDInfo->writeStr(1, 2, reinterpret_cast<const wchar_t*>(date.utf16()), contextFormat);

	QString subStationName = QString::fromStdString(m_NameTranslator.GetFileNameFromKey(m_SubStationKey.toStdString()));
	sheetICDInfo->setMerge(2, 2, 0, 1);
	sheetICDInfo->setMerge(2, 2, 2, 6);
	sheetICDInfo->writeStr(2, 0, L"��繤������", blockFormat);
	sheetICDInfo->writeStr(2, 2, reinterpret_cast<const wchar_t*>(subStationName.utf16()), contextFormat);

	sheetICDInfo->setMerge(3, 3, 0, 1);
	sheetICDInfo->setMerge(3, 3, 2, 6);
	sheetICDInfo->writeStr(3, 0, L"���������", blockFormat);
	sheetICDInfo->writeStr(3, 2, L"", contextFormat);

	sheetICDInfo->setMerge(4, 4, 0, 1);
	sheetICDInfo->setMerge(4, 4, 2, 6);
	sheetICDInfo->writeStr(4, 0, L"ҵ����λ", blockFormat);
	sheetICDInfo->writeStr(4, 2, L"", contextFormat);

	sheetICDInfo->setMerge(5, 5, 0, 1);
	sheetICDInfo->setMerge(5, 5, 2, 4);
	sheetICDInfo->writeStr(5, 0, L"��Ƶ�λ����", blockFormat);
	sheetICDInfo->writeStr(5, 2, L"", contextFormat);
	sheetICDInfo->writeStr(5, 5, L"��Ƶ�λ���", blockFormat);
	sheetICDInfo->writeStr(5, 6, L"", contextFormat);

	sheetICDInfo->setMerge(6, 6, 0, 1);
	sheetICDInfo->setRow(6, 65.5);
	sheetICDInfo->setMerge(6, 6, 2, 4);
	sheetICDInfo->writeStr(6, 0, L"SCD�ļ�����", blockFormat);
	sheetICDInfo->writeStr(6, 2, reinterpret_cast<const wchar_t*>(subStationInfo.fileName.utf16()), contextFormat);
	sheetICDInfo->writeStr(6, 5, L"SCD�ļ��汾��", blockFormat);
	sheetICDInfo->writeStr(6, 6, reinterpret_cast<const wchar_t*>(subStationInfo.version.utf16()), contextFormat);
}


/** 
  * @brief    ExportICDVersionInfo
  * 
  * ����ICD�汾��Ϣ
  * 
  * @class    CExportICDInfo
  * @return   void
  * @author   zhang-xch
  * @date     2014/9/9
  * @remarks  
  */ 
void CExportICDInfo::ExportICDVersionInfo()
{
	sheetICDInfo = book->addSheet(L"ICD�汾��Ϣ");
	SetFormat();
	WriteBasicInfo();
	CreatICDVerInfoMap();
	WriteICDVersionInfo();
	SaveICDVerInfoFile();
}

/** 
  * @brief    SaveIEDVerInfoFile
  * 
  * ����ICD�汾��Ϣ
  * 
  * @class    CExportICDInfo
  * @return   void
  * @author   zhang-xch
  * @date     2014/9/9
  * @remarks  
  */ 
void CExportICDInfo::SaveICDVerInfoFile()
{
	QString fileName = QFileDialog::getSaveFileName(NULL, "����", "ICD�汾��Ϣ.xls", "*.xls");  
	if (!fileName.isEmpty())  
	{  
		if (book->save(reinterpret_cast<const wchar_t*>(fileName.utf16())))
		{
			QMessageBox::about(NULL, "�����ļ�", "�ļ�����ɹ���");
		}else {
			QMessageBox::warning(NULL, "�����ļ�", "�ļ�����ʧ��,�����ļ��Ƿ������������д�!");
		}
	} 
}

/** 
  * @brief    WriteIEDVersionInfo
  * 
  * ��дICD�汾��Ϣ
  * 
  * @class    CExportICDInfo
  * @return   void
  * @author   zhang-xch
  * @date     2014/9/11
  * @remarks  
  */ 
void CExportICDInfo::WriteICDVersionInfo()
{
	sheetICDInfo->setMerge(7, 7, 5, 6);
	sheetICDInfo->writeStr(7, 0, L"���", blockFormat);
	sheetICDInfo->writeStr(7, 1, L"װ���ͺ�", blockFormat);
	sheetICDInfo->writeStr(7, 2, L"װ������", blockFormat);
	sheetICDInfo->writeStr(7, 3, L"��������", blockFormat);
	sheetICDInfo->writeStr(7, 4, L"ICD�ļ��汾��", blockFormat);
	sheetICDInfo->writeStr(7, 5, L"װ������", blockFormat);

	int rowFirst = 8;
	for (QMap<QString, int>::iterator it = m_ICDVerInfoKeyMap.begin(); it != m_ICDVerInfoKeyMap.end(); it++)
	{
		int count = it.value();
		sheetICDInfo->setMerge(rowFirst, rowFirst+count-1, 1, 1);
		sheetICDInfo->writeStr(rowFirst, 1, reinterpret_cast<const wchar_t*>(it.key().utf16()), contextFormat);

		int keyCount = 0;
		for (QMap<QString, QIEDInfo>::iterator it1 = m_ICDVerInfoMap.begin(); it1 != m_ICDVerInfoMap.end(); it1++)
		{
			QIEDInfo IEDInfo = it1.value();
			QList<QString> key = it1.key().split(":");
			if (key[1] == it.key())
			{
				sheetICDInfo->setMerge(rowFirst+keyCount, rowFirst+keyCount, 5, 6);

				sheetICDInfo->writeNum(rowFirst+keyCount, 0, rowFirst+keyCount-7, contextFormat);
				
				sheetICDInfo->writeStr(rowFirst+keyCount, 2, reinterpret_cast<const wchar_t*>(IEDInfo.name.utf16()), contextFormat);
				sheetICDInfo->writeStr(rowFirst+keyCount, 3, reinterpret_cast<const wchar_t*>(IEDInfo.manu.utf16()), contextFormat);
				sheetICDInfo->writeStr(rowFirst+keyCount, 4, reinterpret_cast<const wchar_t*>(IEDInfo.configVer.utf16()), contextFormat);
				sheetICDInfo->writeStr(rowFirst+keyCount, 5, reinterpret_cast<const wchar_t*>(IEDInfo.desc.utf16()), contextFormat);
				keyCount++;
			}
		}
		rowFirst += count;
	}	
}

/** 
  * @brief    CreatICDVerInfoMap
  * 
  * ��������
  * 
  * @class    CExportICDInfo
  * @return   void
  * @author   zhang-xch
  * @date     2014/9/11
  * @remarks  
  */ 
void CExportICDInfo::CreatICDVerInfoMap()
{
	m_ICDVerInfoMap.clear();
	m_ICDVerInfoKeyMap.clear();

	if (m_IEDList.size() > 0)
	{
		for (int i = 0; i < m_IEDList.size(); i++)
		{
			QIEDInfo IEDInfo;
			SCLModel->GetIEDInfo(m_IEDList.at(i), IEDInfo);
			QMap<QString, int>::iterator it = m_ICDVerInfoKeyMap.find(IEDInfo.type);
			if (it != m_ICDVerInfoKeyMap.end())
			{
				int count = it.value() + 1;
				m_ICDVerInfoKeyMap.insert(IEDInfo.type, count);
			}else
			{
				m_ICDVerInfoKeyMap.insert(IEDInfo.type, 1);
			}
			QString ICDVerInfoKey = IEDInfo.name + ":" + IEDInfo.type;
			m_ICDVerInfoMap.insert(ICDVerInfoKey, IEDInfo);
		}
	}
}