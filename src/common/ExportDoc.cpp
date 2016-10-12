#include "ExportDoc.h"
#include <qdatetime.h>
#include "SCLNameTranslator.h"
#include <QMessageBox>
#include <QFileDialog>
#include <qfile.h> 
#include <qtextstream.h> 

CWordCreat::CWordCreat()
{
	m_SCDDiffCompare = SCDDiffCompare::GetInstance();

	m_IEDCount = 0;
	m_SVCBCount = 0;
	m_GOOSECBCount = 0;
	m_InputCount = 0;
	m_PhyPortCount = 0;
	m_InSigCount = 0;
	m_OutSigCount = 0;

	m_Ord = 1;
}

CWordCreat::~CWordCreat()
{

}

void CWordCreat::CreatHtml()
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
void CWordCreat::AddAttachInfo()
{
	if (!QFile::exists("../cfg/ExportWordStart.mht"))
	{
		QMessageBox::warning(NULL, "导出Word文档", "添加模板失败，请检查是否存在模板文件。");
		return;
	}

	QFile file("../cfg/ExportWordStart.mht");

	if (file.open(QIODevice::ReadOnly)) 
	{
		QTextStream stream(&file);
		m_FileStream = stream.readAll();
		file.close();
	}

}
void CWordCreat::AddSummaryInfo()
{
	m_SummaryInfo += "<table class=3DMsoTableGrid border=3D1 cellspacing=3D0 cellpadding=3D0 style=3D'border-collapse:collapse;border:none;mso-border-alt:solid windowt=ext .5pt;mso-yfti-tbllook:1184;mso-padding-alt:0cm 5.4pt 0cm 5.4pt'><tr style=3D'mso-yfti-irow:0;mso-yfti-firstrow:yes;mso-yfti-lastrow:yes'><td width=3D568 valign=3Dtop style=3D'width:426.1pt;border:solid windowte=xt 1.0pt;mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'>";
	QString baseFile = "基准文件：" + m_SCDDiffCompare->m_SCDDiffResult.sFileInfo.basefileName;
	QString baseFileTime = "基准文件生成时间：" + m_SCDDiffCompare->m_SCDDiffResult.sFileInfo.baseSubstationHitmTime;
	QString baseFileCRC = "基准文件CRC校验码：" + m_SCDDiffCompare->m_SCDDiffResult.sFileInfo.crc.baseAttributeName;
	QString otherFile  = "比较文件：" + m_SCDDiffCompare->m_SCDDiffResult.sFileInfo.otherfileName;
	QString otherFileTime = "比较文件生成时间：" + m_SCDDiffCompare->m_SCDDiffResult.sFileInfo.otherSubstationHitmTime;
	QString otherFileCRC = "比较文件CRC校验码：" + m_SCDDiffCompare->m_SCDDiffResult.sFileInfo.crc.otherAttributeName;
	
	QString SoftVersion;
	QFile versionSaver("../cfg/version");
	if (versionSaver.open(QIODevice::ReadOnly))
	{
		QTextStream in(&versionSaver);
		SoftVersion = in.readAll();
		versionSaver.close();
	}
	QString softVersion = "差异化比较工具版本：" + SoftVersion;
	QString exportTime = "报告生成时间：" + QDateTime::currentDateTime().toString("yyyy.MM.dd  hh:mm:ss");

	QString Range = "";
	QString Info = "";
	if (m_IEDCount > 0)
	{
		Range += "IED  ";
		Info += "IED变化数量：" + QString::number(m_IEDCount) + "；";
	}
	if (m_SVCBCount > 0)
	{
		Range += "SV控制块  ";
		Info += "SV控制块变化数量：" + QString::number(m_SVCBCount) + "；";
	}
	if (m_GOOSECBCount > 0)
	{
		Range += "GOOSE控制块  ";
		Info += "GOOSE控制块变化数量：" + QString::number(m_GOOSECBCount) + "；";
	}
	if (m_OutSigCount > 0)
	{
		Range += "输出信号  ";
		Info += "输出信号变化数量：" + QString::number(m_OutSigCount) + "；";
	}
	if (m_InputCount > 0)
	{
		Range += "虚回路  ";
		Info += "虚回路变化数量：" + QString::number(m_InputCount) + "；";
	}
	if (m_InSigCount > 0)
	{
		Range += "输入信号  ";
		Info += "输入信号变化数量：" + QString::number(m_InSigCount) + "；";
	}
	if (m_PhyPortCount > 0)
	{
		Range += "物理端口  ";
		Info += "物理端口变化数量：" + QString::number(m_PhyPortCount) + "；";
	}

	QString compareRange = "差异化比较范围：" + Range;
	QString exportRange = "报告生成范围： " + Range;
	QString SummaryInfo = "差异概述： " + Info;

	m_SummaryInfo += "<p class=3DMsoNormal>" + baseFile + "</p>";
	m_SummaryInfo += "<p class=3DMsoNormal>" + baseFileTime + "</p>";
	m_SummaryInfo += "<p class=3DMsoNormal>" + baseFileCRC + "</p>";
	m_SummaryInfo += "<p class=3DMsoNormal>" + otherFile + "</p>";
	m_SummaryInfo += "<p class=3DMsoNormal>" + otherFileTime + "</p>";
	m_SummaryInfo += "<p class=3DMsoNormal>" + otherFileCRC + "</p>";
	m_SummaryInfo += "<p class=3DMsoNormal>" + softVersion + "</p>";
	m_SummaryInfo += "<p class=3DMsoNormal>" + exportTime + "</p>";
	m_SummaryInfo += "<p class=3DMsoNormal>" + compareRange + "</p>";
	m_SummaryInfo += "<p class=3DMsoNormal>" + exportRange + "</p>";
	m_SummaryInfo += "<p class=3DMsoNormal>" + SummaryInfo + "</p>";

	m_SummaryInfo += "</td></tr></table>";
}

void CWordCreat::AddIEDDiff()
{
	if (m_SCDDiffCompare->m_IEDUnNormalDiffMap.size() > 0)
	{
		m_TableInfo += "<p class=3DMsoNormal><span lang=3DEN-US><o:p>&nbsp;</o:p></span></p>";
		CreatH1Title("IED差异");
		m_Ord  = 1;
		for (DifferMap::iterator it = m_SCDDiffCompare->m_IEDUnNormalDiffMap.begin(); it != m_SCDDiffCompare->m_IEDUnNormalDiffMap.end(); it++)
		{
			IEDStru *pIED = (IEDStru*)(it->second);
			if (pIED->state == DIFFSTATE_EDITED)
			{
				if (IsIEDAttrDiff(pIED))
				{
					m_IEDCount++;
					CreatH2Title(pIED->IEDName);
					m_Ord ++;
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
				m_Ord ++;
				AddVerifyInfo();

			}else if (pIED->state == DIFFSTATE_REMOVED)
			{
				m_IEDCount++;
				QString info = pIED->IEDName + "  删除";
				CreatH2Title(info);
				m_Ord ++;
				AddVerifyInfo();
			}

		}
	}
} 

void CWordCreat::AddSVCBDiff()
{
	if (m_SCDDiffCompare->m_SVCBUnNormalDiffMap.size() > 0)
	{
		m_TableInfo += "<p class=3DMsoNormal><span lang=3DEN-US><o:p>&nbsp;</o:p></span></p>";
		CreatH1Title("SV控制块差异");
		m_Ord = 1;;
		for (DifferMap::iterator it = m_SCDDiffCompare->m_SVCBUnNormalDiffMap.begin(); it != m_SCDDiffCompare->m_SVCBUnNormalDiffMap.end(); it++)
		{
			SVCBStru *pSV = (SVCBStru*)(it->second);
			QString H2Head = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pSV->ID.toStdString())) + "  " +
				QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pSV->ID.toStdString()));
			if (pSV->state == DIFFSTATE_EDITED)
			{
				if (IsSVCBAttrDiff(pSV))
				{
					m_SVCBCount++;
					CreatH2Title(H2Head);
					m_Ord ++;
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
				m_Ord ++;
				AddVerifyInfo();

			}else if (pSV->state == DIFFSTATE_REMOVED)
			{
				m_SVCBCount++;
				QString info = H2Head + "  删除";
				CreatH2Title(info);
				m_Ord ++;
				AddVerifyInfo();
			}

		}
	}
} 

void CWordCreat::AddGOOSECBDiff()
{
	if (m_SCDDiffCompare->m_GOOSECBUnNormalDiffMap.size() > 0)
	{
		m_TableInfo += "<p class=3DMsoNormal><span lang=3DEN-US><o:p>&nbsp;</o:p></span></p>";
		CreatH1Title("GOOSE控制块差异");
		m_Ord = 1;;
		for (DifferMap::iterator it = m_SCDDiffCompare->m_GOOSECBUnNormalDiffMap.begin(); it != m_SCDDiffCompare->m_GOOSECBUnNormalDiffMap.end(); it++)
		{
			GOOSECBStru *pGOOSE = (GOOSECBStru*)(it->second);
			QString H2Head = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pGOOSE->ID.toStdString())) + "  " +
				QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pGOOSE->ID.toStdString()));
			if (pGOOSE->state == DIFFSTATE_EDITED)
			{
				if (IsGOOSECBAttrDiff(pGOOSE))
				{
					m_GOOSECBCount++;
					CreatH2Title(H2Head);
					m_Ord ++;
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
				m_Ord ++;
				AddVerifyInfo();

			}else if (pGOOSE->state == DIFFSTATE_REMOVED)
			{
				m_GOOSECBCount++;
				QString info = H2Head + "  删除";
				CreatH2Title(info);
				m_Ord ++;
				AddVerifyInfo();
			}

		}
	}
} 

void CWordCreat::AddOutSignalDiff()
{
	if (m_SCDDiffCompare->m_OutSigUnNormalDiffMap.size() > 0)
	{
		m_TableInfo += "<p class=3DMsoNormal><span lang=3DEN-US><o:p>&nbsp;</o:p></span></p>";
		CreatH1Title("输出信号差异");
		m_Ord = 1;;
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
					m_Ord ++;
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
				m_Ord ++;
				AddVerifyInfo();

			}else if (pOutSig->state == DIFFSTATE_REMOVED)
			{
				m_OutSigCount++;
				QString info = H2Head + "  删除";
				CreatH2Title(info);
				m_Ord ++;
				AddVerifyInfo();
			}

		}
	}
} 

void CWordCreat::AddInSignalDiff()
{
	if (m_SCDDiffCompare->m_InSigUnNormalDiffMap.size() > 0)
	{
		m_TableInfo += "<p class=3DMsoNormal><span lang=3DEN-US><o:p>&nbsp;</o:p></span></p>";
		CreatH1Title("输入信号差异");
		m_Ord = 1;;
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
					m_Ord ++;
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
				m_Ord ++;
				AddVerifyInfo();

			}else if (pInSig->state == DIFFSTATE_REMOVED)
			{
				m_InSigCount++;
				QString info = H2Head + "  删除";
				CreatH2Title(info);
				m_Ord ++;
				AddVerifyInfo();
			}

		}
	}
} 

void CWordCreat::AddInputsDiff()
{
	if (m_SCDDiffCompare->m_InputsUnNormalDiffMap.size() > 0 || m_SCDDiffCompare->m_InputsOtherUnNormalDiffMap.size() > 0)
	{
		m_TableInfo += "<p class=3DMsoNormal><span lang=3DEN-US><o:p>&nbsp;</o:p></span></p>";
		CreatH1Title("虚回路差异");
		m_Ord = 1;
		if (m_SCDDiffCompare->m_InputsUnNormalDiffMap.size() > 0)
		{
			for (DifferMap::iterator it = m_SCDDiffCompare->m_InputsUnNormalDiffMap.begin(); it != m_SCDDiffCompare->m_InputsUnNormalDiffMap.end(); it++)
			{
				ExtRefStru *pInput = (ExtRefStru*)(it->second);
				QString H2Head = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pInput->ID.toStdString())) + "  " +
					QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pInput->ID.toStdString()));

				QString detailInfo = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pInput->srcSignalID.toStdString())) + "  " +
					QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pInput->srcSignalID.toStdString()))
					+ "  ---->  " + QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pInput->destSignalID.toStdString())) + "  " +
					QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pInput->destSignalID.toStdString())); 
				if (pInput->state == DIFFSTATE_EDITED)
				{
					if (IsInputAttrDiff(pInput))
					{
						m_InputCount++;
						CreatH2Title(H2Head);
						m_Ord ++;
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
					CreatH2TitleWithoutOrd(detailInfo);
					m_Ord ++;
					AddVerifyInfo();

				}else if (pInput->state == DIFFSTATE_REMOVED)
				{
					m_InputCount++;
					QString info = H2Head + "  删除";
					CreatH2Title(info);
					CreatH2TitleWithoutOrd(detailInfo);
					m_Ord ++;
					AddVerifyInfo();
				}

			}
		}
		if (m_SCDDiffCompare->m_InputsOtherUnNormalDiffMap.size() > 0)
		{
			for (DifferMap::iterator it = m_SCDDiffCompare->m_InputsOtherUnNormalDiffMap.begin(); it != m_SCDDiffCompare->m_InputsOtherUnNormalDiffMap.end(); it++)
			{
				ExtRefStru *pInput = (ExtRefStru*)(it->second);
				QString H2Head = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pInput->ID.toStdString())) + "  " +
					QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pInput->ID.toStdString()));

				QString detailInfo = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pInput->srcSignalID.toStdString())) + "  " +
					QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pInput->srcSignalID.toStdString()))
					 + "  ---->  " + QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pInput->destSignalID.toStdString())) + "  " +
					 QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pInput->destSignalID.toStdString())); 
				if (pInput->state == DIFFSTATE_EDITED)
				{
					if (IsInputAttrDiff(pInput))
					{
						m_InputCount++;
						CreatH2Title(H2Head);
						m_Ord ++;
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
					CreatH2TitleWithoutOrd(detailInfo);
					m_Ord ++;
					AddVerifyInfo();

				}else if (pInput->state == DIFFSTATE_REMOVED)
				{
					m_InputCount++;
					QString info = H2Head + "  删除";
					CreatH2Title(info);
					CreatH2TitleWithoutOrd(detailInfo);
					m_Ord ++;
					AddVerifyInfo();
				}

			}
		}		
	}
} 

void CWordCreat::AddPhyPortDiff()
{
	if (m_SCDDiffCompare->m_PhyPortUnNormalDiffMap.size() > 0)
	{
		m_TableInfo += "<p class=3DMsoNormal><span lang=3DEN-US><o:p>&nbsp;</o:p></span></p>";
		CreatH1Title("物理端口差异");
		m_Ord = 1;;
		for (DifferMap::iterator it = m_SCDDiffCompare->m_PhyPortUnNormalDiffMap.begin(); it != m_SCDDiffCompare->m_PhyPortUnNormalDiffMap.end(); it++)
		{
			PhysicsPortStru *pPhy = (PhysicsPortStru*)(it->second);
			QString H2Head = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pPhy->ID.toStdString())) + "  " +
				QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pPhy->ID.toStdString()));
			if (pPhy->state == DIFFSTATE_EDITED)
			{
				if (IsPhyPortAttrDiff(pPhy))
				{
					m_PhyPortCount++;
					CreatH2Title(H2Head);
					m_Ord ++;
					StartTable();
					AddTrInfo(pPhy->transMedia);
					AddTrInfo(pPhy->cable);
					AddTrInfo(pPhy->pIndex);
					AddTrInfo(pPhy->plug);
					AddTrInfo(pPhy->type);
					EndTable();
					AddVerifyInfo();
				}				
			}else if (pPhy->state == DIFFSTATE_ADDED)
			{
				m_PhyPortCount++;
				QString info = H2Head + "  新增"; 
				CreatH2Title(info);
				m_Ord ++;
				AddVerifyInfo();

			}else if (pPhy->state == DIFFSTATE_REMOVED)
			{
				m_PhyPortCount++;
				QString info = H2Head + "  删除";
				CreatH2Title(info);
				m_Ord ++;
				AddVerifyInfo();
			}

		}
	}
}

bool CWordCreat::IsIEDAttrDiff(IEDStru *pIED)
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

bool CWordCreat::IsSVCBAttrDiff(SVCBStru *pSV)
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

bool CWordCreat::IsGOOSECBAttrDiff(GOOSECBStru *pGOOSE)
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

bool CWordCreat::IsSignalAttrDiff(SignalStru *pSig)
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

bool CWordCreat::IsInputAttrDiff(ExtRefStru *pInput)
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

bool CWordCreat::IsPhyPortAttrDiff(PhysicsPortStru *pPhy)
{
	if (pPhy->transMedia.state != DIFFSTATE_NORMAL)
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
	if (pPhy->cable.state != DIFFSTATE_NORMAL)
	{
		return true;
	}
	return false;
}


void CWordCreat::CreatH1Title(QString H1Title)
{
	m_TableInfo += "<h2><span class=3DMsoBookTitle><span lang=3DEN-US style=3D'font-size:14.0pt;line-height:173%'>" 
		+ H1Title + "</span></span>	<span class=3DMsoBookTitle><span lang=3DEN-US style=3D'font-size:14.0pt;line-height:173%'><o:p></o:p></span></span></h2>"; 
}
void CWordCreat::CreatH2Title(QString H2Title)
{
	m_TableInfo += "<p class=3DMsoNormal style=3D'margin-left:21.0pt'><span lang=3DEN-US>" 
		+ QString::number(m_Ord) + "，" + H2Title + "</p>"; 

}

void CWordCreat::CreatH2TitleWithoutOrd(QString H2Title)
{
	m_TableInfo += "<p class=3DMsoNormal style=3D'margin-left:21.0pt'><span lang=3DEN-US>" 
		+  H2Title + "</p>"; 

}


void CWordCreat::StartTable()
{
	m_TableInfo += "<table class=3DMsoTableGrid border=3D1 cellspacing=3D0 cellpadding=3D0 style=3D'margin-left:21.0pt;border-collapse:collapse;border:none;mso-borde=r-alt:solid windowtext .5pt;mso-yfti-tbllook:1184;mso-padding-alt:0cm 5.4pt 0cm =	5.4pt'>	<tr style=3D'mso-yfti-irow:0;mso-yfti-firstrow:yes'><td width=3D189 valign=3Dtop style=3D'width:142.0pt;border:solid windowtext 1.0pt;	mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'><p class=3DMsoNormal>变更对象</p>	</td><td width=3D189 valign=3Dtop style=3D'width:142.05pt;border:solid windowt=	ext 1.0pt;border-left:none;mso-border-left-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'><p class=3DMsoNormal>基准文件值</p></td><td width=3D189 valign=3Dtop style=3D'width:142.05pt;border:solid windowt=ext 1.0pt;	border-left:none;mso-border-left-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'><p class=3DMsoNormal>比较文件值</p></td></tr>";
}

void CWordCreat::EndTable()
{
	m_TableInfo += "</table>";
}

void CWordCreat::AddVerifyInfo()
{
	m_TableInfo += "<p class=3DMsoNormal style=3D'margin-left:21.0pt'><span lang=3DEN-US><span style=3D'mso-tab-count:12'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><span style=3D'mso-spacerun:yes'>&nbsp; </span></span>审核意见：</p>";
}

void CWordCreat::AddTrInfo(BasicIntStru basicStr)
{
	if (basicStr.state != DIFFSTATE_NORMAL)
	{
		m_TableInfo += " <tr style=3D'mso-yfti-irow:1;mso-yfti-lastrow:yes'><td width=3D189 valign=3Dtop style=3D'width:142.0pt;border:solid windowte=xt 1.0pt;	border-top:none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:s=olid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'><p class=3DMsoNormal><span lang=3DEN-US>"
			+ basicStr.AttributeDesc + "</p></td><td width=3D189 valign=3Dtop style=3D'width:142.05pt;border-top:none;bord=	er-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;	mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'><p class=3DMsoNormal><span lang=3DEN-US>"
			+ QString::number(basicStr.baseAttributeName) + "</p></td><td width=3D189 valign=3Dtop style=3D'width:142.05pt;border-top:none;bord=er-left:	none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1=.0pt;	mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid window=text .5pt;mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'><p class=3DMsoNormal><span lang=3DEN-US>"
			+ QString::number(basicStr.otherAttributeName) + "</p></td></tr>";
	}
}

void CWordCreat::AddTrInfo(BasicStru basicStr)
{
	if (basicStr.state != DIFFSTATE_NORMAL)
	{
		m_TableInfo += " <tr style=3D'mso-yfti-irow:1;mso-yfti-lastrow:yes'><td width=3D189 valign=3Dtop style=3D'width:142.0pt;border:solid windowte=xt 1.0pt;	border-top:none;mso-border-top-alt:solid windowtext .5pt;mso-border-alt:s=olid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'><p class=3DMsoNormal><span lang=3DEN-US>"
			+ basicStr.AttributeDesc + "</p></td><td width=3D189 valign=3Dtop style=3D'width:142.05pt;border-top:none;bord=	er-left:none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;	mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid windowtext .5pt;mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'><p class=3DMsoNormal><span lang=3DEN-US>"
			+ basicStr.baseAttributeName + "</p></td><td width=3D189 valign=3Dtop style=3D'width:142.05pt;border-top:none;bord=er-left:	none;border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1=.0pt;	mso-border-top-alt:solid windowtext .5pt;mso-border-left-alt:solid window=text .5pt;mso-border-alt:solid windowtext .5pt;padding:0cm 5.4pt 0cm 5.4pt'><p class=3DMsoNormal><span lang=3DEN-US>"
			+ basicStr.otherAttributeName + "</p></td></tr>";
	}
}

void CWordCreat::AddEndInfo()
{
	m_FileStream =  m_FileStream + "</div></body></html>";
}

void CWordCreat::SaveFile()
{
	if (!QFile::exists("../cfg/ExportWordEnd.mht"))
	{
		QMessageBox::warning(NULL, "导出Word文档", "添加模板失败，请检查是否存在模板文件。");
		return;
	}
	QFile file("../cfg/ExportWordEnd.mht");

	if (file.open(QIODevice::ReadOnly)) 
	{
		QTextStream stream(&file);
		m_FileStream += stream.readAll();
		file.close();
	}


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

