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
//  * 构造函数
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
//		book->setKey(L"济南容弗科技有限公司", L"windows-26292c0705cde60766bc6667a2hce3l9");
//	}
//}
//
///** 
//  * @brief    ~CExportValidateExcel
//  * 
//  * 析构函数
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
//  * 导出检测结果报表
//  * 
//  * @class    CExportValidateExcel
//  * @return   void
//  * @author   zhang-xch
//  * @date     2015/1/16
//  * @remarks  
//  */ 
//void CExportValidateExcel::run()
//{
//	sheetValidateInfo = book->addSheet(L"校验结果");
//	SetFormat();
//	WriteValidateInfo();
//	WriteBasicInfo();
//	SaveValidateInfoToFile();
//}
//
///** 
//  * @brief    SetFormat
//  * 
//  * 设置报表中的内容的格式
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
//	headerFont->setName(L"宋体");
//	headerFont->setSize(11);
//
//	contextFont->setName(L"宋体");
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
//  * 在EXCEL中填充标题和表头
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
//	sheetValidateInfo->writeStr(0, 0, reinterpret_cast<const wchar_t*>(L"检测报告"), headerFormat);
//
//	
//	basicInfo = "文件名称：" + m_subStationInfo.fileName + "\n文件路径：" +m_subStationInfo.filePath + "\n\n" + basicInfo; 
//	
//	for (int i = 0; i < 6; i++)
//	{
//		sheetValidateInfo->setCellFormat(1, i, contextFormat);
//	}
//	sheetValidateInfo->setRow(1, 160);
//	sheetValidateInfo->setMerge(1, 1, 0, 5);	
//	sheetValidateInfo->writeStr(1, 0, reinterpret_cast<const wchar_t*>(basicInfo.utf16()), contextFormat);
//
//	sheetValidateInfo->writeStr(2, 0, L"序号", headerFormat);
//	sheetValidateInfo->writeStr(2, 1, L"校验类型", headerFormat);
//	sheetValidateInfo->writeStr(2, 2, L"位置", headerFormat);
//	sheetValidateInfo->writeStr(2, 3, L"校验结果", headerFormat);
//	sheetValidateInfo->writeStr(2, 4, L"检测项目", headerFormat);
//	sheetValidateInfo->writeStr(2, 5, L"参考规范", headerFormat);
//}
//
///** 
//  * @brief    WriteValidateInfo
//  * 
//  * 读取校验结果文件到Excel中
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
//		{"语法语义检查", "错误", "./Validate/RL_SCHEMA_Temp", 0}, 
//		{"属性规范检查","错误", "./Validate/RL_PROPERTIES_Temp", 0}, 
//		{"唯一性检查","错误", "./Validate/RL_UNIQUENESS_Temp", 0}, 
//		{"模型完整性校验","错误", "./Validate/RL_MODELINTEGRITY_Temp", 0}, 
//		{"引用有效性校验","错误", "./Validate/RL_REFERENCE_Temp", 0}, 
//		{"结构规范性校验","错误", "./Validate/RL_STRUCT_Temp", 0}, 
//
//		{"语法语义检查","警告", "./Validate/RL_SCHEMA_Warn_Temp", 0}, 
//		{"属性规范检查","警告", "./Validate/RL_PROPERTIES_Warn_Temp", 0}, 
//		{"唯一性检查","警告", "./Validate/RL_UNIQUENESS_Warn_Temp", 0}, 
//		{"模型完整性校验","警告", "./Validate/RL_MODELINTEGRITY_Warn_Temp", 0}, 
//		{"引用有效性校验","警告", "./Validate/RL_REFERENCE_Warn_Temp", 0}, 
//		{"结构规范性校验","警告", "./Validate/RL_STRUCT_Warn_Temp", 0}, 
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
//			Error = Error + m_ItemInfoList[i].name +"共计：" + QString::number(m_ItemInfoList[i].count) + "项;\n";
//			ErrorCount += m_ItemInfoList[i].count;
//		}
//
//		if (i > 5 && m_ItemInfoList[i].count > 0)
//		{
//			Warn = Warn + m_ItemInfoList[i].name +"共计：" + QString::number(m_ItemInfoList[i].count) + "项;\n";
//			WarnCount += m_ItemInfoList[i].count;
//		}
//	}
//
//	basicInfo = "统计结果：\n错误：" + QString::number(ErrorCount) + "\n" + Error
//		+ "警告：" + QString::number(WarnCount) + "\n" + Warn;
//}
//
///** 
//  * @brief    OpenFile
//  * 
//  * 读取硬盘上的校验文件
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
//		QMessageBox::warning(NULL, "打开失败", "Open File "+ item.fileName + " Error!");
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
//  * 将校验文件中的内容一条条写入Excel文档中
//  * 
//  * @class    CExportValidateExcel
//  * @param    FILE *fp      :
//  * @param    QString type  :
//  * @return   当前项的数目
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
//  * 将结果保存到本地文件中
//  * 
//  * @class    CExportValidateExcel
//  * @return   void
//  * @author   zhang-xch
//  * @date     2015/1/20
//  * @remarks  
//  */ 
//void CExportValidateExcel::SaveValidateInfoToFile()
//{
//	QString fileName = QFileDialog::getSaveFileName(NULL, "保存", "检测报告", "*.xls");  
//	if (!fileName.isEmpty())  
//	{  
//		if (book->save(reinterpret_cast<const wchar_t*>(fileName.utf16())))
//		{
//			QMessageBox::about(NULL, "保存文件", "文件保存成功！");
//		}else {
//			QMessageBox::warning(NULL, "保存文件", "文件保存失败,请检查文件是否在其他程序中打开!");
//		}
//	} 
//}