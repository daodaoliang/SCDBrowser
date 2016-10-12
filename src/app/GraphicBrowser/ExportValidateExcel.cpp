//#include "ExportValidateExcel.h"
//#include "SCLModelSvrAdapter.h"
//#include <QFileDialog>
//#include <QMessageBox>
//#include "GraphicBrowserFrm.h"
//
//
///** 
//  * @brief    CExportValidateExcel
//  * 
//  * ���캯��
//  * 
//  * @class    CExportValidateExcel
//  * @param    void :
//  * @return   
//  * @author   zhang-xch
//  * @date     2015/1/16
//  * @remarks  
//  */ 
//CExportValidateExcel::CExportValidateExcel(QString SubStationKey)
//{
//	m_SubStationKey = SubStationKey;
//	SCLModel = SCLModelAdapter::GetInstance();
//	SCLModel->GetSubstationInfo(m_SubStationKey, m_subStationInfo);
//
//	itemCount = 1;
//
//	book = xlCreateBook();
//	if (book)
//	{
//		book->setKey(L"�����ݸ��Ƽ����޹�˾", L"windows-26292c0705cde60766bc6667a2hce3l9");
//	}
//}
//
///** 
//  * @brief    ~CExportValidateExcel
//  * 
//  * ��������
//  * 
//  * @class    CExportValidateExcel
//  * @param    void :
//  * @return   
//  * @author   zhang-xch
//  * @date     2015/1/16
//  * @remarks  
//  */ 
//CExportValidateExcel::~CExportValidateExcel(void)
//{
//	book->release();
//}
//
///** 
//  * @brief    run
//  * 
//  * �������������
//  * 
//  * @class    CExportValidateExcel
//  * @return   void
//  * @author   zhang-xch
//  * @date     2015/1/16
//  * @remarks  
//  */ 
//void CExportValidateExcel::run()
//{
//	sheetValidateInfo = book->addSheet(L"У����");
//	SetFormat();
//	WriteValidateInfo();
//	WriteBasicInfo();
//	SaveValidateInfoToFile();
//}
//
///** 
//  * @brief    SetFormat
//  * 
//  * ���ñ����е����ݵĸ�ʽ
//  * 
//  * @class    CExportValidateExcel
//  * @return   void
//  * @author   zhang-xch
//  * @date     2015/1/16
//  * @remarks  
//  */ 
//void CExportValidateExcel::SetFormat()
//{
//	headerFont = book->addFont();
//	headerFormat = book->addFormat();
//	contextFont = book->addFont();
//	contextFormat = book->addFormat();
//
//	headerFont->setBold(true);
//	headerFont->setName(L"����");
//	headerFont->setSize(11);
//
//	contextFont->setName(L"����");
//	contextFont->setSize(11);
//
//	headerFormat->setFont(headerFont);
//	headerFormat->setBorder(BORDERSTYLE_THIN);
//	headerFormat->setAlignV(ALIGNV_CENTER);
//	headerFormat->setAlignH(ALIGNH_CENTER);
//	headerFormat->setWrap(true);
//
//	contextFormat->setFont(contextFont);
//	contextFormat->setBorder(BORDERSTYLE_THIN);
//	contextFormat->setAlignH(ALIGNH_LEFT);
//	contextFormat->setAlignV(ALIGNV_CENTER);
//	contextFormat->setWrap(true);
//}
//
///** 
//  * @brief    WriteBasicInfo
//  * 
//  * ��EXCEL��������ͱ�ͷ
//  * 
//  * @class    CExportValidateExcel
//  * @return   void
//  * @author   zhang-xch
//  * @date     2015/1/20
//  * @remarks  
//  */ 
//void CExportValidateExcel::WriteBasicInfo()
//{
//	sheetValidateInfo->setCol(0, 5, 12);
//	sheetValidateInfo->setCol(3, 3, 80);
//	sheetValidateInfo->setMerge(0, 0, 0, 5);
//	sheetValidateInfo->writeStr(0, 0, reinterpret_cast<const wchar_t*>(L"��ⱨ��"), headerFormat);
//
//	
//	basicInfo = "�ļ����ƣ�" + m_subStationInfo.fileName + "\n�ļ�·����" +m_subStationInfo.filePath + "\n\n" + basicInfo; 
//	
//	for (int i = 0; i < 6; i++)
//	{
//		sheetValidateInfo->setCellFormat(1, i, contextFormat);
//	}
//	sheetValidateInfo->setRow(1, 160);
//	sheetValidateInfo->setMerge(1, 1, 0, 5);	
//	sheetValidateInfo->writeStr(1, 0, reinterpret_cast<const wchar_t*>(basicInfo.utf16()), contextFormat);
//
//	sheetValidateInfo->writeStr(2, 0, L"���", headerFormat);
//	sheetValidateInfo->writeStr(2, 1, L"У������", headerFormat);
//	sheetValidateInfo->writeStr(2, 2, L"λ��", headerFormat);
//	sheetValidateInfo->writeStr(2, 3, L"У����", headerFormat);
//	sheetValidateInfo->writeStr(2, 4, L"�����Ŀ", headerFormat);
//	sheetValidateInfo->writeStr(2, 5, L"�ο��淶", headerFormat);
//}
//
///** 
//  * @brief    WriteValidateInfo
//  * 
//  * ��ȡУ�����ļ���Excel��
//  * 
//  * @class    CExportValidateExcel
//  * @return   void
//  * @author   zhang-xch
//  * @date     2015/1/20
//  * @remarks  
//  */ 
//void CExportValidateExcel::WriteValidateInfo()
//{
//	ItemInfoStr m_ItemInfoList[12]= {
//		{"�﷨������", "����", "./Validate/RL_SCHEMA_Temp", 0}, 
//		{"���Թ淶���","����", "./Validate/RL_PROPERTIES_Temp", 0}, 
//		{"Ψһ�Լ��","����", "./Validate/RL_UNIQUENESS_Temp", 0}, 
//		{"ģ��������У��","����", "./Validate/RL_MODELINTEGRITY_Temp", 0}, 
//		{"������Ч��У��","����", "./Validate/RL_REFERENCE_Temp", 0}, 
//		{"�ṹ�淶��У��","����", "./Validate/RL_STRUCT_Temp", 0}, 
//
//		{"�﷨������","����", "./Validate/RL_SCHEMA_Warn_Temp", 0}, 
//		{"���Թ淶���","����", "./Validate/RL_PROPERTIES_Warn_Temp", 0}, 
//		{"Ψһ�Լ��","����", "./Validate/RL_UNIQUENESS_Warn_Temp", 0}, 
//		{"ģ��������У��","����", "./Validate/RL_MODELINTEGRITY_Warn_Temp", 0}, 
//		{"������Ч��У��","����", "./Validate/RL_REFERENCE_Warn_Temp", 0}, 
//		{"�ṹ�淶��У��","����", "./Validate/RL_STRUCT_Warn_Temp", 0}, 
//	};
//
//	QString Error, Warn;
//	int ErrorCount = 0;
//	int WarnCount = 0;
//
//	for (int i=0; i < sizeof(m_ItemInfoList)/sizeof(ItemInfoStr); i++)
//	{
//		OpenFile(m_ItemInfoList[i]);
//		if (i < 6 && m_ItemInfoList[i].count > 0)
//		{
//			Error = Error + m_ItemInfoList[i].name +"���ƣ�" + QString::number(m_ItemInfoList[i].count) + "��;\n";
//			ErrorCount += m_ItemInfoList[i].count;
//		}
//
//		if (i > 5 && m_ItemInfoList[i].count > 0)
//		{
//			Warn = Warn + m_ItemInfoList[i].name +"���ƣ�" + QString::number(m_ItemInfoList[i].count) + "��;\n";
//			WarnCount += m_ItemInfoList[i].count;
//		}
//	}
//
//	basicInfo = "ͳ�ƽ����\n����" + QString::number(ErrorCount) + "\n" + Error
//		+ "���棺" + QString::number(WarnCount) + "\n" + Warn;
//}
//
///** 
//  * @brief    OpenFile
//  * 
//  * ��ȡӲ���ϵ�У���ļ�
//  * 
//  * @class    CExportValidateExcel
//  * @return   void
//  * @author   zhang-xch
//  * @date     2015/1/20
//  * @remarks  
//  */ 
//void CExportValidateExcel::OpenFile(ItemInfoStr &item)
//{
//	FILE *fp = NULL;
//	fp = fopen(item.fileName.toStdString().c_str(), "rb");
//	if (NULL == fp)
//	{
//		QMessageBox::warning(NULL, "��ʧ��", "Open File "+ item.fileName + " Error!");
//		exit(0);
//	}
//	else
//	{
//		item.count = InsertItem(fp, item.type);
//		fclose(fp);
//		fp = NULL;
//	}
//}
//
///** 
//  * @brief    InsertItem
//  * 
//  * ��У���ļ��е�����һ����д��Excel�ĵ���
//  * 
//  * @class    CExportValidateExcel
//  * @param    FILE *fp      :
//  * @param    QString type  :
//  * @return   ��ǰ�����Ŀ
//  * @author   zhang-xch
//  * @date     2015/1/20
//  * @remarks  
//  */ 
//int CExportValidateExcel::InsertItem(FILE *fp, QString type)
//{
//	char wName[MAXSIZE];
//	char wValue[EXLSIZE];
//	char wLevel[MAXSIZE];
//	char wRef[MAXSIZE];
//
//	int number = 0;
//	while(0 == feof(fp))
//	{
//		if (fread(wName,sizeof(char), sizeof(wName)/sizeof(char), fp) == 0)
//		{
//			break;
//		}
//		fread(wValue,sizeof(char), sizeof(wValue)/sizeof(char), fp);
//		fread(wLevel,sizeof(char), sizeof(wLevel)/sizeof(char), fp);
//		fread(wRef,sizeof(char), sizeof(wRef)/sizeof(char), fp);
//
//		number++;
//		sheetValidateInfo->writeNum(itemCount+2, 0, itemCount, contextFormat);
//		sheetValidateInfo->writeStr(itemCount+2, 1, reinterpret_cast<const wchar_t*>(type.utf16()), contextFormat);
//		sheetValidateInfo->writeStr(itemCount+2, 2, reinterpret_cast<const wchar_t*>(QString::fromStdString(wName).utf16()), contextFormat);
//		sheetValidateInfo->writeStr(itemCount+2, 3, reinterpret_cast<const wchar_t*>(QString::fromStdString(wValue).utf16()), contextFormat);
//		sheetValidateInfo->writeStr(itemCount+2, 4, reinterpret_cast<const wchar_t*>(QString::fromStdString(wLevel).utf16()), contextFormat);
//		sheetValidateInfo->writeStr(itemCount+2, 5, reinterpret_cast<const wchar_t*>(QString::fromStdString(wRef).utf16()), contextFormat);
//
//		itemCount++;
//	}
//	return number;
//}
//
///** 
//  * @brief    SaveValidateInfoToFile
//  * 
//  * ��������浽�����ļ���
//  * 
//  * @class    CExportValidateExcel
//  * @return   void
//  * @author   zhang-xch
//  * @date     2015/1/20
//  * @remarks  
//  */ 
//void CExportValidateExcel::SaveValidateInfoToFile()
//{
//	QString fileName = QFileDialog::getSaveFileName(NULL, "����", "��ⱨ��", "*.xls");  
//	if (!fileName.isEmpty())  
//	{  
//		if (book->save(reinterpret_cast<const wchar_t*>(fileName.utf16())))
//		{
//			QMessageBox::about(NULL, "�����ļ�", "�ļ�����ɹ���");
//		}else {
//			QMessageBox::warning(NULL, "�����ļ�", "�ļ�����ʧ��,�����ļ��Ƿ������������д�!");
//		}
//	} 
//}