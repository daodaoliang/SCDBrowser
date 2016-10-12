#include "DOCCreat.h"
#include <QTime>
#include "SCLNameTranslator.h"
#include <QMessageBox>
#include <QFileDialog>
#include <qfile.h> 
#include <qtextstream.h> 

CDOCCreat::CDOCCreat()
{
	m_SCDDiffCompare = SCDDiffCompare::GetInstance();

	m_IEDCount = 0;
	m_SVCBCount = 0;
	m_GOOSECBCount = 0;
	m_InputCount = 0;
	m_PhyPortCount = 0;
	m_InSigCount = 0;
	m_OutSigCount = 0;
}

CDOCCreat::~CDOCCreat()
{

}

//void CDOCCreat::Run()
//{
//	::CoInitialize (NULL);
//	COfficeWord wd;
//	wd.CreateApp();
//	wd.CreateDocumtent();
//
//	for(int i=0;i<200;i++)
//	{
//		wd.CreateTable(10,10);
//
//		for(int iRow=1;iRow<=10;iRow++)
//		{
//			for(int iCol=1;iCol<=10;iCol++)
//			{
//				wd.WriteCellText(iRow,iCol,L"陈");
//			}
//		}
//		wd.MoveDown(4);
//		wd.NewLine(4);
//	}
//
//	wd.ShowApp();
//
//	return;
//
//	// COfficeWord wd;
//	wd.Create();
//	wd.SetFont(16, L"宋体"); //设置字体(字号，字体名，颜色)
//	wd.SetFont(1,0,0); //设置字体为粗体，不是斜体，没有下划线
//	wd.SetParaphFormat(1); //设置文字为居中对齐
//	wd.WriteText(L"SCD差异化报告"); //写文字
//	wd.NewLine(1);
//	wd.SetParaphFormat(0);
//	
//	WriteDiffSummary(wd);
//	wd.NewLine(1);
//
//	/*for (int i = 0; i < 10; i++)
//	{
//		wd.CreateTable(3 + 1, 3);
//		wd.WriteCellText(1, 1, L"变更对象");
//		wd.WriteCellText(1, 2, L"基准文件内容");
//		wd.WriteCellText(1, 3, L"比较文件内容");
//		int iCurrentPoint = 2;
//		BasicStru temp;
//		temp.AttributeDesc = "变更对象";
//		temp.baseAttributeName = "基准";
//		temp.otherAttributeName = "比较";
//		WriteCellDiffText(wd, iCurrentPoint, temp);
//		WriteCellDiffText(wd, iCurrentPoint, temp);
//		WriteCellDiffText(wd, iCurrentPoint, temp);
//		wd.MoveDown(1);
//		wd.MoveDown(1);
//		wd.SetFont(TRUE);
//		wd.WriteText(L"审核意见：");
//		wd.NewLine(2);
//	}*/
//	
//
//	//WriteIEDDiff(wd);
//	//WriteSVCBDiff(wd);
//
//	wd.ShowApp();
//}
//
//void CDOCCreat::WriteDiffSummary(COfficeWord &wd)
//{
//	wd.CreateTable(1,1);
//	wd.SetFont(10.5, L"宋体");
//	wd.NewLine(1);
//	wd.SetFont(TRUE);
//	wd.WriteText(L"基准文件：");
//	wd.SetFont(FALSE);
//	wd.WriteText((LPCTSTR)(m_SCDDiffCompare->m_SCDDiffResult.sFileInfo.basefileName.utf16()));
//
//	wd.NewLine(1);
//	wd.SetFont(TRUE);
//	wd.WriteText(L"基准文件生成时间：");
//	wd.SetFont(FALSE);
//	wd.WriteText(L"");
//
//	wd.NewLine(1);
//	wd.SetFont(TRUE);
//	wd.WriteText(L"比较文件：");
//	wd.SetFont(FALSE);
//	wd.WriteText((LPCTSTR)(m_SCDDiffCompare->m_SCDDiffResult.sFileInfo.otherfileName.utf16()));
//
//	wd.NewLine(1);
//	wd.SetFont(TRUE);
//	wd.WriteText(L"比较文件生成时间：");
//	wd.SetFont(FALSE);
//	wd.WriteText((LPCTSTR)(m_SCDDiffCompare->m_SCDDiffResult.sFileInfo.basefileTime.utf16()));
//
//	wd.NewLine(1);
//	wd.SetFont(TRUE);
//	wd.WriteText(L"差异化比较工具版本：");
//	wd.SetFont(FALSE);
//	wd.WriteText((LPCTSTR)(m_SCDDiffCompare->m_SCDDiffResult.sFileInfo.otherfileTime.utf16()));
//
//	wd.NewLine(1);
//	wd.SetFont(TRUE);
//	wd.WriteText(L"报告生成时间：");
//	wd.SetFont(FALSE);
//	wd.WriteText((LPCTSTR)(QTime::currentTime().toString().utf16()));
//
//	wd.NewLine(1);
//	wd.SetFont(TRUE);
//	wd.WriteText(L"比较差异限定范围：");
//	wd.SetFont(FALSE);
//	wd.WriteText(L"");
//
//	wd.NewLine(1);
//	wd.SetFont(TRUE);
//	wd.WriteText(L"报告生成范围：");
//	wd.SetFont(FALSE);
//	wd.WriteText(L"");
//
//	wd.NewLine(1);
//	wd.SetFont(TRUE);
//	wd.WriteText(L"差异内容概述：");
//	wd.SetFont(FALSE);
//	wd.WriteText(L"");
//
//	wd.MoveDown(1);
//	wd.MoveDown(1);
//}
//
//
//void CDOCCreat::WriteIEDDiff(COfficeWord &wd)
//{
//	wd.SetFont(TRUE);
//	wd.SetFont(16, L"宋体");
//	wd.WriteText(L"一，IED变化");
//	wd.NewLine(1);
//	wd.SetFont(FALSE);
//	wd.SetFont(10.5, L"宋体");
//	DifferMap *tempIEDMap = &m_SCDDiffCompare->m_IEDUnNormalDiffMap;
//
//	int iOrd = 1;
//	for (DifferMap::iterator it = tempIEDMap->begin(); it != tempIEDMap->end(); it++)
//	{
//		IEDStru *pIED = (IEDStru*)(it->second);
//		wd.SetFont(TRUE);
//		QString object = QString::number(iOrd) + "  " + pIED->IEDName;
//		wd.WriteText((LPCTSTR)(object.utf16()));
//		wd.NewLine(1);
//		int iRow = GetIEDAttributeDiffCount(pIED);
//		if (iRow > 0)
//		{
//			wd.CreateTable(iRow + 1, 3);
//			wd.WriteCellText(1, 1, L"变更对象");
//			wd.WriteCellText(1, 2, L"基准文件内容");
//			wd.WriteCellText(1, 3, L"比较文件内容");
//			int iCurrentPoint = 2;
//			WriteCellDiffText(wd, iCurrentPoint, pIED->IEDDesc);
//			WriteCellDiffText(wd, iCurrentPoint, pIED->IEDType);
//			WriteCellDiffText(wd, iCurrentPoint, pIED->IEDManu);
//			wd.MoveDown(1);
//			wd.SetFont(TRUE);
//			wd.WriteText(L"审核意见：");
//			wd.NewLine(2);
//		}
//		iOrd++;
//	}
//}
//
//int CDOCCreat::GetIEDAttributeDiffCount(IEDStru* pIED)
//{
//	int iRow = 0;
//	if (pIED->IEDType.state != DIFFSTATE_NORMAL)
//	{
//		iRow++;
//	}
//	if (pIED->IEDManu.state != DIFFSTATE_NORMAL)
//	{
//		iRow++;
//	}
//
//	if (pIED->IEDDesc.state != DIFFSTATE_NORMAL)
//	{
//		iRow++;
//	}
//	return iRow;
//}
//
//void CDOCCreat::WriteSVCBDiff(COfficeWord &wd)
//{
//	wd.SetFont(TRUE);
//	wd.SetFont(16, L"宋体");
//	wd.WriteText(L"二，SV控制块变化");
//	wd.NewLine(1);
//	wd.SetFont(FALSE);
//	wd.SetFont(10.5, L"宋体");
//	DifferMap *tempSVMap = &m_SCDDiffCompare->m_SVCBUnNormalDiffMap;
//
//	int iOrd = 1;
//	for (DifferMap::iterator it = tempSVMap->begin(); it != tempSVMap->end(); it++)
//	{
//		SVCBStru *pSV = (SVCBStru*)(it->second);
//		wd.SetFont(TRUE);
//		QString object = QString::number(iOrd) + "  " + QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pSV->ID.toStdString())) + "  " + pSV->CBName.baseAttributeName;
//		wd.WriteText((LPCTSTR)(object.utf16()));
//		wd.NewLine(1);
//		int iRow = GetSVCBAttributeDiffCount(pSV);
//		if (iRow > 0)
//		{
//			wd.CreateTable(iRow + 1, 3);
//			wd.WriteCellText(0, 0, L"变更对象");
//			wd.WriteCellText(0, 1, L"基准文件内容");
//			wd.WriteCellText(0, 2, L"比较文件内容");
//			int iCurrentPoint = 1;
//			WriteCellDiffText(wd, iCurrentPoint, pSV->AppID);
//			WriteCellDiffText(wd, iCurrentPoint, pSV->CBName);
//			WriteCellDiffText(wd, iCurrentPoint, pSV->ConfVersion);
//			wd.MoveDown(1);
//			wd.SetFont(TRUE);
//			wd.WriteText(L"审核意见：");
//			wd.NewLine(2);
//		}
//		iOrd++;
//	}
//}
//
//int CDOCCreat::GetSVCBAttributeDiffCount(SVCBStru* pSV)
//{
//	int iRow = 0;
//	if (pSV->apAppID.state != DIFFSTATE_NORMAL)
//	{
//		iRow++;
//	}
//	if (pSV->AppID.state != DIFFSTATE_NORMAL)
//	{
//		iRow++;
//	}
//	if (pSV->ASDUCount.state != DIFFSTATE_NORMAL)
//	{
//		iRow++;
//	}
//	if (pSV->CBDesc.state != DIFFSTATE_NORMAL)
//	{
//		iRow++;
//	}
//	if (pSV->CBName.state != DIFFSTATE_NORMAL)
//	{
//		iRow++;
//	}
//	if (pSV->ConfVersion.state != DIFFSTATE_NORMAL)
//	{
//		iRow++;
//	}
//	if (pSV->DataSetAddr.state != DIFFSTATE_NORMAL)
//	{
//		iRow++;
//	}
//	if (pSV->DataSetCount.state != DIFFSTATE_NORMAL)
//	{
//		iRow++;
//	}
//	if (pSV->DataSetDesc.state != DIFFSTATE_NORMAL)
//	{
//		iRow++;
//	}
//	if (pSV->DataSetName.state != DIFFSTATE_NORMAL)
//	{
//		iRow++;
//	}
//	if (pSV->MACAddr.state != DIFFSTATE_NORMAL)
//	{
//		iRow++;
//	}
//	if (pSV->smpRate.state != DIFFSTATE_NORMAL)
//	{
//		iRow++;
//	}
//	if (pSV->VLANPriority.state != DIFFSTATE_NORMAL)
//	{
//		iRow++;
//	}
//	return iRow;
//}
//
//
//void CDOCCreat::WriteCellDiffText(COfficeWord &wd, int &iCurrentPoint, BasicStru basicStru)
//{
//	if (basicStru.state != DIFFSTATE_NORMAL)
//	{
//		wd.WriteCellText(iCurrentPoint, 1, (LPCTSTR)(basicStru.AttributeDesc.utf16()));
//		wd.WriteCellText(iCurrentPoint, 2, (LPCTSTR)(basicStru.baseAttributeName.utf16()));
//		wd.WriteCellText(iCurrentPoint, 3, (LPCTSTR)(basicStru.otherAttributeName.utf16()));
//		iCurrentPoint++;
//	}
//}



void CDOCCreat::CreatHtml()
{
	AddAttachInfo();

	
	AddIEDDiff();
	AddSVCBDiff();
	AddGOOSECBDiff();
	AddOutSignalDiff();
	
	AddInputsDiff();
	AddInSignalDiff();

	AddPhyPortDiff();

	AddSummaryInfo();

	m_FileStream = m_FileStream + m_SummaryInfo + m_TableInfo;

	AddEndInfo();
	SaveFile();
}
void CDOCCreat::AddAttachInfo()
{
	if (!QFile::exists("../cfg/ExportWord.Html"))
	{
		QMessageBox::warning(NULL, "导出Word文档", "添加模板失败，请检查是否存在模板文件。");
		return;
	}
	QFile file("../cfg/ExportWord.Html");

	if (file.open(QIODevice::ReadOnly)) 
	{
		QTextStream stream(&file);
		m_FileStream = stream.readAll();
		file.close();
	}

}
void CDOCCreat::AddSummaryInfo()
{
	m_SummaryInfo += "<table class=MsoTableGrid border=1 cellspacing=0 cellpadding=0	style='border-collapse:collapse;border:none;mso-border-alt:solid windowtext .5pt; mso-yfti-tbllook:1184;mso-padding-alt:0cm 5.4pt 0cm 5.4pt'> <tr style='mso-yfti-irow:0;mso-yfti-firstrow:yes;mso-yfti-lastrow:yes'> <td width=568 valign=top style='width:426.1pt;border:solid windowtext 1.0pt;mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>";

	QString baseFile = "基准文件：" + m_SCDDiffCompare->m_SCDDiffResult.sFileInfo.basefileName;
	QString baseFileTime = "基准文件生成时间：" + m_SCDDiffCompare->m_SCDDiffResult.sFileInfo.baseSubstationHitmTime;
	QString otherFile  = "比较文件：" + m_SCDDiffCompare->m_SCDDiffResult.sFileInfo.otherfileName;
	QString otherFileTime = "比较文件生成时间：" + m_SCDDiffCompare->m_SCDDiffResult.sFileInfo.otherSubstationHitmTime;
	QString softVersion = "差异化比较工具版本：" + SoftVersion;
	QString exportTime = "报告生成时间：" + QTime::currentTime().toString("yyyy.MM.dd  hh.mm.ss");

	QString Range = "";
	QString Info = "";
	if (m_IEDCount > 0)
	{
		Range += "IED  ";
		Info += "IED变化数量：" + QString::number(m_IEDCount) + ";";
	}
	if (m_SVCBCount > 0)
	{
		Range += "SV控制块  ";
		Info += "SV控制块变化数量：" + QString::number(m_SVCBCount) + ";";
	}
	if (m_GOOSECBCount > 0)
	{
		Range += "GOOSE控制块  ";
		Info += "GOOSE控制块变化数量：" + QString::number(m_GOOSECBCount) + ";";
	}
	if (m_OutSigCount > 0)
	{
		Range += "输出信号  ";
		Info += "输出信号变化数量：" + QString::number(m_OutSigCount) + ";";
	}
	if (m_InputCount > 0)
	{
		Range += "虚回路  ";
		Info += "虚回路变化数量：" + QString::number(m_InputCount) + ";";
	}
	if (m_InSigCount > 0)
	{
		Range += "输入信号  ";
		Info += "输入信号变化数量：" + QString::number(m_InSigCount) + ";";
	}
	if (m_PhyPortCount > 0)
	{
		Range += "物理端口  ";
		Info += "物理端口变化数量：" + QString::number(m_PhyPortCount) + ";";
	}

	QString compareRange = "差异化比较范围：" + Range;
	QString exportRange = "报告生成范围： " + Range;
	QString SummaryInfo = "差异概述： " + Info;

	m_SummaryInfo += "<p class=MsoNormal>" + baseFile + "</p>";
	m_SummaryInfo += "<p class=MsoNormal>" + baseFileTime + "</p>";
	m_SummaryInfo += "<p class=MsoNormal>" + otherFile + "</p>";
	m_SummaryInfo += "<p class=MsoNormal>" + otherFileTime + "</p>";
	m_SummaryInfo += "<p class=MsoNormal>" + softVersion + "</p>";
	m_SummaryInfo += "<p class=MsoNormal>" + exportTime + "</p>";
	m_SummaryInfo += "<p class=MsoNormal>" + compareRange + "</p>";
	m_SummaryInfo += "<p class=MsoNormal>" + exportRange + "</p>";
	m_SummaryInfo += "<p class=MsoNormal>" + SummaryInfo + "</p>";

	m_SummaryInfo += "</td></tr></table>";
}

void CDOCCreat::AddIEDDiff()
{
	if (m_SCDDiffCompare->m_IEDUnNormalDiffMap.size() > 0)
	{
		m_TableInfo += "<p class=MsoNormal><span lang=EN-US><o:p>&nbsp;</o:p></span></p>";
		CreatH1Title("IED差异");
		for (DifferMap::iterator it = m_SCDDiffCompare->m_IEDUnNormalDiffMap.begin(); it != m_SCDDiffCompare->m_IEDUnNormalDiffMap.end(); it++)
		{
			IEDStru *pIED = (IEDStru*)(it->second);
			if (pIED->state == DIFFSTATE_EDITED)
			{
				if (IsIEDAttrDiff(pIED))
				{
					m_IEDCount++;
					CreatH2Title(pIED->IEDName);
					StartTable();
					AddTrInfo(pIED->IEDDesc);
					AddTrInfo(pIED->IEDType);
					AddTrInfo(pIED->IEDManu);
					AddTrInfo(pIED->IEDCRC);
					EndTable();
					AddVerifyInfo();
				}				
			}else if (pIED->state == DIFFSTATE_ADDED)
			{
				m_IEDCount++;
				QString info = pIED->IEDName + "  新增"; 
				CreatH2Title(info);

			}else if (pIED->state == DIFFSTATE_REMOVED)
			{
				m_IEDCount++;
				QString info = pIED->IEDName + "  删除";
				CreatH2Title(info);
			}
			
		}
	}
} 

void CDOCCreat::AddSVCBDiff()
{
	if (m_SCDDiffCompare->m_SVCBUnNormalDiffMap.size() > 0)
	{
		m_TableInfo += "<p class=MsoNormal><span lang=EN-US><o:p>&nbsp;</o:p></span></p>";
		CreatH1Title("SV控制块差异");
		for (DifferMap::iterator it = m_SCDDiffCompare->m_SVCBUnNormalDiffMap.begin(); it != m_SCDDiffCompare->m_SVCBUnNormalDiffMap.end(); it++)
		{
			SVCBStru *pSV = (SVCBStru*)(it->second);
			QString H2Head = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pSV->ID.toStdString())) + 
				QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pSV->ID.toStdString()));
			if (pSV->state == DIFFSTATE_EDITED)
			{
				if (IsSVCBAttrDiff(pSV))
				{
					m_SVCBCount++;
					CreatH2Title(H2Head);
					StartTable();
					AddTrInfo(pSV->AppID);
					AddTrInfo(pSV->VLANID);
					AddTrInfo(pSV->VLANPriority);
					AddTrInfo(pSV->MACAddr);
					AddTrInfo(pSV->ConfVersion);
					AddTrInfo(pSV->ASDUCount);
					AddTrInfo(pSV->smpRate);
					AddTrInfo(pSV->CBName);
					// AddTrInfo(pSV->CBDesc);
					AddTrInfo(pSV->DataSetName);
					AddTrInfo(pSV->DataSetDesc);
					AddTrInfo(pSV->DataSetCount);
					AddTrInfo(pSV->apAppID);
					EndTable();
					AddVerifyInfo();
				}				
			}else if (pSV->state == DIFFSTATE_ADDED)
			{
				m_SVCBCount++;
				QString info = H2Head + "  新增"; 
				CreatH2Title(info);

			}else if (pSV->state == DIFFSTATE_REMOVED)
			{
				m_SVCBCount++;
				QString info = H2Head + "  删除";
				CreatH2Title(info);
			}

		}
	}
} 

void CDOCCreat::AddGOOSECBDiff()
{
	if (m_SCDDiffCompare->m_GOOSECBUnNormalDiffMap.size() > 0)
	{
		m_TableInfo += "<p class=MsoNormal><span lang=EN-US><o:p>&nbsp;</o:p></span></p>";
		CreatH1Title("GOOSE控制块差异");
		for (DifferMap::iterator it = m_SCDDiffCompare->m_GOOSECBUnNormalDiffMap.begin(); it != m_SCDDiffCompare->m_GOOSECBUnNormalDiffMap.end(); it++)
		{
			GOOSECBStru *pGOOSE = (GOOSECBStru*)(it->second);
			QString H2Head = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pGOOSE->ID.toStdString())) + 
				QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pGOOSE->ID.toStdString()));
			if (pGOOSE->state == DIFFSTATE_EDITED)
			{
				if (IsGOOSECBAttrDiff(pGOOSE))
				{
					m_GOOSECBCount++;
					CreatH2Title(H2Head);
					StartTable();
					AddTrInfo(pGOOSE->AppID);
					AddTrInfo(pGOOSE->VLANID);
					AddTrInfo(pGOOSE->VLANPriority);
					AddTrInfo(pGOOSE->MACAddr);
					AddTrInfo(pGOOSE->ConfVersion);
					AddTrInfo(pGOOSE->MaxTime);
					AddTrInfo(pGOOSE->MinTime);
					AddTrInfo(pGOOSE->CBName);
					AddTrInfo(pGOOSE->CBDesc);
					AddTrInfo(pGOOSE->DataSetName);
					AddTrInfo(pGOOSE->DataSetDesc);
					AddTrInfo(pGOOSE->DataSetCount);
					AddTrInfo(pGOOSE->GocbRef);
					AddTrInfo(pGOOSE->DataSetAddr);
					AddTrInfo(pGOOSE->apAppID);
					EndTable();
					AddVerifyInfo();
				}				
			}else if (pGOOSE->state == DIFFSTATE_ADDED)
			{
				m_GOOSECBCount++;
				QString info = H2Head + "  新增"; 
				CreatH2Title(info);

			}else if (pGOOSE->state == DIFFSTATE_REMOVED)
			{
				m_GOOSECBCount++;
				QString info = H2Head + "  删除";
				CreatH2Title(info);
			}

		}
	}
} 

void CDOCCreat::AddOutSignalDiff()
{
	if (m_SCDDiffCompare->m_OutSigUnNormalDiffMap.size() > 0)
	{
		m_TableInfo += "<p class=MsoNormal><span lang=EN-US><o:p>&nbsp;</o:p></span></p>";
		CreatH1Title("输出信号差异");
		for (DifferMap::iterator it = m_SCDDiffCompare->m_OutSigUnNormalDiffMap.begin(); it != m_SCDDiffCompare->m_OutSigUnNormalDiffMap.end(); it++)
		{
			SignalStru *pOutSig = (SignalStru*)(it->second);
			QString H2Head = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pOutSig->ID.toStdString())) + "  " +
				QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pOutSig->ID.toStdString()));
			if (pOutSig->state == DIFFSTATE_EDITED)
			{
				if (IsSignalAttrDiff(pOutSig) || (pOutSig->Ord.state != DIFFSTATE_NORMAL))
				{
					m_OutSigCount++;
					CreatH2Title(H2Head);
					StartTable();
					AddTrInfo(pOutSig->Ord);
					AddTrInfo(pOutSig->DOIdU);
					AddTrInfo(pOutSig->DODesc);
					AddTrInfo(pOutSig->LNDesc);
					AddTrInfo(pOutSig->FC);
					AddTrInfo(pOutSig->BDAType);
					AddTrInfo(pOutSig->TripName);
					AddTrInfo(pOutSig->Desc);
					AddTrInfo(pOutSig->PortIndex);
					EndTable();
					AddVerifyInfo();
				}				
			}else if (pOutSig->state == DIFFSTATE_ADDED)
			{
				m_OutSigCount++;
				QString info = H2Head + "  新增"; 
				CreatH2Title(info);

			}else if (pOutSig->state == DIFFSTATE_REMOVED)
			{
				m_OutSigCount++;
				QString info = H2Head + "  删除";
				CreatH2Title(info);
			}

		}
	}
} 

void CDOCCreat::AddInSignalDiff()
{
	if (m_SCDDiffCompare->m_InSigUnNormalDiffMap.size() > 0)
	{
		m_TableInfo += "<p class=MsoNormal><span lang=EN-US><o:p>&nbsp;</o:p></span></p>";
		CreatH1Title("输入信号差异");
		for (DifferMap::iterator it = m_SCDDiffCompare->m_InSigUnNormalDiffMap.begin(); it != m_SCDDiffCompare->m_InSigUnNormalDiffMap.end(); it++)
		{
			SignalStru *pInSig = (SignalStru*)(it->second);
			QString H2Head = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pInSig->ID.toStdString())) + "  " +
				QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pInSig->ID.toStdString()));
			if (pInSig->state == DIFFSTATE_EDITED)
			{
				if (IsSignalAttrDiff(pInSig))
				{
					m_InSigCount++;
					CreatH2Title(H2Head);
					StartTable();
					// AddTrInfo(pInSig->Ord);
					AddTrInfo(pInSig->DOIdU);
					AddTrInfo(pInSig->DODesc);
					AddTrInfo(pInSig->LNDesc);
					AddTrInfo(pInSig->FC);
					AddTrInfo(pInSig->BDAType);
					AddTrInfo(pInSig->TripName);
					AddTrInfo(pInSig->Desc);
					AddTrInfo(pInSig->PortIndex);
					EndTable();
					AddVerifyInfo();
				}				
			}else if (pInSig->state == DIFFSTATE_ADDED)
			{
				m_InSigCount++;
				QString info = H2Head + "  新增"; 
				CreatH2Title(info);

			}else if (pInSig->state == DIFFSTATE_REMOVED)
			{
				m_InSigCount++;
				QString info = H2Head + "  删除";
				CreatH2Title(info);
			}

		}
	}
} 

void CDOCCreat::AddInputsDiff()
{
	if (m_SCDDiffCompare->m_InputsUnNormalDiffMap.size() > 0)
	{
		m_TableInfo += "<p class=MsoNormal><span lang=EN-US><o:p>&nbsp;</o:p></span></p>";
		CreatH1Title("虚回路差异");
		for (DifferMap::iterator it = m_SCDDiffCompare->m_InputsUnNormalDiffMap.begin(); it != m_SCDDiffCompare->m_InputsUnNormalDiffMap.end(); it++)
		{
			ExtRefStru *pInput = (ExtRefStru*)(it->second);
			QString H2Head = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pInput->ID.toStdString())) + 
				QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pInput->ID.toStdString()));
			if (pInput->state == DIFFSTATE_EDITED)
			{
				if (IsInputAttrDiff(pInput))
				{
					m_InputCount++;
					CreatH2Title(H2Head);
					StartTable();
					AddTrInfo(pInput->Ord);
					AddTrInfo(pInput->desc);
					AddTrInfo(pInput->IntAddr);
					AddTrInfo(pInput->OutAddr);
					AddTrInfo(pInput->PortName);
					EndTable();
					AddVerifyInfo();
				}				
			}else if (pInput->state == DIFFSTATE_ADDED)
			{
				m_InputCount++;
				QString info = H2Head + "  新增"; 
				CreatH2Title(info);

			}else if (pInput->state == DIFFSTATE_REMOVED)
			{
				m_InputCount++;
				QString info = H2Head + "  删除";
				CreatH2Title(info);
			}

		}
	}
} 

void CDOCCreat::AddPhyPortDiff()
{
	if (m_SCDDiffCompare->m_PhyPortUnNormalDiffMap.size() > 0)
	{
		m_TableInfo += "<p class=MsoNormal><span lang=EN-US><o:p>&nbsp;</o:p></span></p>";
		CreatH1Title("物理端口差异");
		for (DifferMap::iterator it = m_SCDDiffCompare->m_PhyPortUnNormalDiffMap.begin(); it != m_SCDDiffCompare->m_PhyPortUnNormalDiffMap.end(); it++)
		{
			PhysicsPortStru *pPhy = (PhysicsPortStru*)(it->second);
			QString H2Head = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pPhy->ID.toStdString())) + 
				QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pPhy->ID.toStdString()));
			if (pPhy->state == DIFFSTATE_EDITED)
			{
				if (IsPhyPortAttrDiff(pPhy))
				{
					m_PhyPortCount++;
					CreatH2Title(H2Head);
					StartTable();
					AddTrInfo(pPhy->desc);
					AddTrInfo(pPhy->pIndex);
					AddTrInfo(pPhy->plug);
					AddTrInfo(pPhy->type);
					AddTrInfo(pPhy->rtType);
					EndTable();
					AddVerifyInfo();
				}				
			}else if (pPhy->state == DIFFSTATE_ADDED)
			{
				m_PhyPortCount++;
				QString info = H2Head + "  新增"; 
				CreatH2Title(info);

			}else if (pPhy->state == DIFFSTATE_REMOVED)
			{
				m_PhyPortCount++;
				QString info = H2Head + "  删除";
				CreatH2Title(info);
			}

		}
	}
}

bool CDOCCreat::IsIEDAttrDiff(IEDStru *pIED)
{
	if (pIED->IEDManu.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pIED->IEDType.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pIED->IEDDesc.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	return false;
}

bool CDOCCreat::IsSVCBAttrDiff(SVCBStru *pSV)
{
	if (pSV->AppID.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pSV->VLANID.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pSV->VLANPriority.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pSV->MACAddr.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pSV->ConfVersion.state != DIFFSTATE_NORMAL)
	{
		return true;
	}

	if (pSV->ASDUCount.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pSV->smpRate.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pSV->CBName.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pSV->CBDesc.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pSV->DataSetName.state != DIFFSTATE_NORMAL)
	{
		return true;
	}

	if (pSV->DataSetDesc.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pSV->DataSetCount.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pSV->DataSetAddr.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pSV->apAppID.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	return false;
}

bool CDOCCreat::IsGOOSECBAttrDiff(GOOSECBStru *pGOOSE)
{
	if (pGOOSE->AppID.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pGOOSE->VLANID.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pGOOSE->VLANPriority.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pGOOSE->MACAddr.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pGOOSE->ConfVersion.state != DIFFSTATE_NORMAL)
	{
		return true;
	}

	if (pGOOSE->MaxTime.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pGOOSE->MinTime.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pGOOSE->CBName.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pGOOSE->DataSetName.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pGOOSE->DataSetDesc.state != DIFFSTATE_NORMAL)
	{
		return true;
	}

	if (pGOOSE->DataSetCount.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pGOOSE->GocbRef.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pGOOSE->DataSetAddr.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pGOOSE->apAppID.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	return false;
}

bool CDOCCreat::IsSignalAttrDiff(SignalStru *pSig)
{
	if (pSig->DOIdU.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pSig->DODesc.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pSig->LNDesc.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pSig->FC.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pSig->BDAType.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pSig->TripName.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pSig->Desc.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pSig->PortIndex.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	return false;
}

bool CDOCCreat::IsInputAttrDiff(ExtRefStru *pInput)
{
	if (pInput->Ord.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pInput->desc.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pInput->IntAddr.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pInput->OutAddr.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pInput->PortName.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	return false;
}

bool CDOCCreat::IsPhyPortAttrDiff(PhysicsPortStru *pPhy)
{
	if (pPhy->desc.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pPhy->pIndex.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pPhy->plug.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pPhy->type.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	if (pPhy->rtType.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	return false;
}


void CDOCCreat::CreatH1Title(QString H1Title)
{
	m_TableInfo += "<p class=MsoListParagraph style='margin-left:24.0pt;text-indent:-24.0pt; mso-char-indent-count:0;mso-list:l0 level1 lfo1'>" 
		+ H1Title + "</p>"; 
}
void CDOCCreat::CreatH2Title(QString H2Title)
{
	m_TableInfo += "<p class=MsoListParagraph style='margin-left:39.0pt;text-indent:-18.0pt;mso-char-indent-count:0;mso-list:l1 level1 lfo3'>" 
		+ H2Title + "</p>"; 
}

void CDOCCreat::CreatTable()
{

}

void CDOCCreat::StartTable()
{
	m_TableInfo += "<table class=MsoTableGrid border=1 cellspacing=0 cellpadding=0 style='margin-left:21.0pt;border-collapse:collapse;border:none;mso-border-alt: solid windowtext .5pt;mso-yfti-tbllook:1184;mso-padding-alt:0cm 5.4pt 0cm 5.4pt'><tr style='mso-yfti-irow:0;mso-yfti-firstrow:yes'><td width=189 valign=top style='width:142.0pt;border:solid windowtext 1.0pt; mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'><p class=MsoNormal>变更对象</p></td><td width=189 valign=top style='width:142.05pt;border:solid windowtext 1.0pt; border-left:none;mso-border-left-alt:solid windowtext .5pt;mso-border-alt:  solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'><p class=MsoNormal>基准文件值</p></td><td width=189 valign=top style='width:142.05pt;border:solid windowtext 1.0pt;  border-left:none;mso-border-left-alt:solid windowtext .5pt;mso-border-alt:  solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'><p class=MsoNormal>比较文件值</p></td></tr>";
}

void CDOCCreat::EndTable()
{
	m_TableInfo += "</table>";
}

void CDOCCreat::AddVerifyInfo()
{
	m_TableInfo += "<p class=MsoNormal style='margin-left:21.0pt'><span lang=EN-US><span style='mso-tab-count:12'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><span style='mso-spacerun:yes'>&nbsp; </span></span>审核意见：</p>";
}

void CDOCCreat::AddTrInfo(BasicIntStru basicStr)
{
	if (basicStr.state != DIFFSTATE_NORMAL)
	{
		m_TableInfo += " <tr style='mso-yfti-irow:1'><td width=189 valign=top style='width:142.0pt;border:solid windowtext 1.0pt;  border-top:none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'><p class=MsoNormal>"
			+ basicStr.AttributeDesc + "</p></td><td width=189 valign=top style='width:142.05pt;border-top:none;border-left:  none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'><p class=MsoNormal>"
			+ QString::number(basicStr.baseAttributeName) + "</p></td><td width=189 valign=top style='width:142.05pt;border-top:none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;	mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'><p class=MsoNormal>"
			+ QString::number(basicStr.otherAttributeName) + "</p></td></tr>";
	}
}

void CDOCCreat::AddTrInfo(BasicStru basicStr)
{
	if (basicStr.state != DIFFSTATE_NORMAL)
	{
		m_TableInfo += " <tr style='mso-yfti-irow:1'><td width=189 valign=top style='width:142.0pt;border:solid windowtext 1.0pt;  border-top:none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'><p class=MsoNormal>"
			+ basicStr.AttributeDesc + "</p></td><td width=189 valign=top style='width:142.05pt;border-top:none;border-left:  none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;  mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;  mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'><p class=MsoNormal>"
			+ basicStr.baseAttributeName + "</p></td><td width=189 valign=top style='width:142.05pt;border-top:none;border-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;	mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'><p class=MsoNormal>"
			+ basicStr.otherAttributeName + "</p></td></tr>";
	}
}

void CDOCCreat::AddEndInfo()
{
	m_FileStream =  m_FileStream + "</div></body></html>";
}

void CDOCCreat::SaveFile()
{
	QString fileName = QFileDialog::getSaveFileName(NULL, "保存差异结果", "差异化比较结果.doc", "*.doc");  
	if (!fileName.isEmpty())  
	{  
		QFile file(fileName);
		if ( file.open(QIODevice::WriteOnly ) ) {
			QTextStream stream( &file );
			stream<< m_FileStream << "\n";
			file.close();
		}
		QMessageBox::information(NULL, "导出Word文档", "导出文档成功！");
	} 
}
