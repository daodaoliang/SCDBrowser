#include "XLCreat.h"
#include <QMessageBox>
#include <QFileDialog>
#include "SCLNameTranslator.h"

CXLCreat::CXLCreat(void)
{
	book = xlCreateBook();
	if (book)
	{
		book->setKey(L"济南容弗科技有限公司", L"windows-26292c0705cde60766bc6667a2hce3l9");
		
	}
	CreatFormat();
	SetFormat();
	m_SCDDiffCompare = SCDDiffCompare::GetInstance();
	iCount = 1;
}


CXLCreat::~CXLCreat(void)
{
	book->release();
}

void CXLCreat::Run()
{
	if (m_SCDDiffCompare->GetFileInfoSelectedState())
	{
		CreatDocInfoSheet();
	}
	if (m_SCDDiffCompare->GetIEDSelectedState())
	{
		CreatIEDDiffSheet();
		CreatIEDEditDiffSheet();
	}
	
	SaveFile();
}

void CXLCreat::CreatDocInfoSheet()
{
	InitPoint();
	sheetDocInfo = book->addSheet(L"版本信息");
	AddFileInfoTableHead(sheetDocInfo);
	ExportFileInFoDiff();
}
void CXLCreat::AddFileInfoTableHead(Sheet* pSheet)
{
	pSheet->setCol(0, 0, 10);
	pSheet->setCol(1, 4, 25);
	pSheet->writeStr(0, 0, L"序号", headerFormat);
	pSheet->writeStr(0, 1, L"属性名称", headerFormat);
	pSheet->writeStr(0, 2, L"变更类型", headerFormat);
	pSheet->writeStr(0, 3, L"基准文件中的内容", headerFormat);
	pSheet->writeStr(0, 4, L"比较文件中的内容", headerFormat);
}
/** 
  * @brief    ExportFileInFoDiff
  * 
  * 导出文件信息差异
  * 
  * @class    CXLCreat
  * @return   void
  * @author   zhang-xch
  * @date     2014/5/22
  * @remarks  
  */ 
void CXLCreat::ExportFileInFoDiff()
{
	SetFileInfoDiffAttributeValue(m_SCDDiffCompare->m_SCDDiffResult.sFileInfo.version, "版本信息");
	SetFileInfoDiffAttributeValue(m_SCDDiffCompare->m_SCDDiffResult.sFileInfo.reVersion, "修订版本信息");
	SetFileInfoDiffAttributeValue(m_SCDDiffCompare->m_SCDDiffResult.sFileInfo.crc, "CRC校验码");
}

void CXLCreat::CreatIEDDiffSheet()
{
	InitPoint();
	sheetIEDDiff = book->addSheet(L"增加或删除的IED");
	AddIEDDiffTableHead(sheetIEDDiff);
	for (uint i = 0; i < m_SCDDiffCompare->m_SCDDiffResult.vIEDStru.size(); i++)
	{
		IEDStru *pIED = &m_SCDDiffCompare->m_SCDDiffResult.vIEDStru.at(i);
		if ((pIED->state == DIFFSTATE_ADDED) || (pIED->state == DIFFSTATE_REMOVED))
		{
			SetIEDDiffAttributeValue(pIED);
		}			
	}
}
void CXLCreat::AddIEDDiffTableHead(Sheet* pSheet)
{
	pSheet->setCol(0, 0, 10);
	pSheet->setCol(1, 2, 25);
	pSheet->writeStr(0, 0, L"序号", headerFormat);
	pSheet->writeStr(0, 1, L"变更对象", headerFormat);
	pSheet->writeStr(0, 2, L"变更类型", headerFormat);
}

void CXLCreat::CreatIEDEditDiffSheet()
{
	InitPoint();
	sheetIEDEdit = book->addSheet(L"修改的IED");
	AddIEDEditTableHead(sheetIEDEdit);
	ExportIEDDiff();
}
void CXLCreat::AddIEDEditTableHead(Sheet* pSheet)
{
	pSheet->setCol(0, 0, 10);
	pSheet->setCol(1, 8, 20);
	pSheet->writeStr(0, 0, L"序号", headerFormat);
	pSheet->writeStr(0, 1, L"IED名称", headerFormat);
	pSheet->writeStr(0, 2, L"类型", headerFormat);
	pSheet->writeStr(0, 3, L"变更对象", headerFormat);
	pSheet->writeStr(0, 4, L"变更类型", headerFormat);
	pSheet->writeStr(0, 5, L"对象路径", headerFormat);
	pSheet->writeStr(0, 6, L"属性名称", headerFormat);
	pSheet->writeStr(0, 7, L"基准文件中的内容", headerFormat);
	pSheet->writeStr(0, 8, L"比较文件中的内容", headerFormat);
}
/** 
  * @brief    ExportIEDDiff
  * 
  * 导出IED差异
  * 
  * @class    CXLCreat
  * @return   void
  * @author   zhang-xch
  * @date     2014/5/22
  * @remarks  
  */ 
void CXLCreat::ExportIEDDiff()
{
	int iCount = 1;
	for (uint i = 0; i < m_SCDDiffCompare->m_SCDDiffResult.vIEDStru.size(); i++)
	{
		IEDStru *pIED = &m_SCDDiffCompare->m_SCDDiffResult.vIEDStru.at(i);
		if (pIED->state == DIFFSTATE_EDITED)
		{
			
			if (iCount%2)
			{
				pFormat = attrabute1ValueFormat;
			}
			else
			{
				pFormat = attrabute2ValueFormat;
			}
			iCount++;
			SetIEDEditDiffAttributeValue(pIED->IEDName, "IED", pIED->IEDName, pIED->IEDName, "IED描述", pIED->IEDDesc);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "IED", pIED->IEDName, pIED->IEDName, "IED类型", pIED->IEDType);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "IED", pIED->IEDName, pIED->IEDName, "IED厂家", pIED->IEDManu);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "IED", pIED->IEDName, pIED->IEDName, "IED CRC校验码", pIED->IEDCRC);
			ExportSVDiff(pIED);
			ExportGooseDiff(pIED);
			ExportInputsDiff(pIED);
			ExportPhyPortDiff(pIED);
			
		} 
	}
}

/** 
  * @brief    ExportSVDiff
  * 
  * 导出SV控制块差异
  * 
  * @class    CXLCreat
  * @param    IEDStru *pIED : IED指针
  * @return   void
  * @author   zhang-xch
  * @date     2014/5/22
  * @remarks  
  */ 
void CXLCreat::ExportSVDiff(IEDStru *pIED)
{
	for (uint i = 0; i < pIED->vSVCB.size(); i++)
	{
		SVCBStru *pSV = &pIED->vSVCB.at(i);
		QString ObjName = NameTranslator_GetObjNameFromSCDDiffResult(pSV->ID);
		QString Path = pIED->IEDName + "/" + ObjName;
		if (pSV->state == DIFFSTATE_EDITED)
		{
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV控制块", ObjName, Path, "AppID", pSV->AppID);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV控制块", ObjName, Path, "VLanID", pSV->VLANID);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV控制块", ObjName, Path, "VLan优先级", pSV->VLANPriority);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV控制块", ObjName, Path, "Mac地址", pSV->MACAddr);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV控制块", ObjName, Path, "配置版本信息", pSV->ConfVersion);
			SetIEDEditDiffAttributeIntValue(pIED->IEDName, "SV控制块", ObjName, Path, "ASDU数目", pSV->ASDUCount);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV控制块", ObjName, Path, "采样率", pSV->smpRate);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV控制块", ObjName, Path, "控制块名称", pSV->CBName);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV控制块", ObjName, Path, "控制块描述", pSV->CBDesc);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV控制块", ObjName, Path, "数据集名称", pSV->DataSetName);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV控制块", ObjName, Path, "数据集描述", pSV->DataSetDesc);
			SetIEDEditDiffAttributeIntValue(pIED->IEDName, "SV控制块", ObjName, Path, "数据集信号数目", pSV->DataSetCount);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV控制块", ObjName, Path, "数据集地址", pSV->DataSetAddr);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV控制块", ObjName, Path, "控制块Ap地址", pSV->apAppID);	
			ExportSignaleDiff(pIED->IEDName ,pSV);
		}else if (pSV->state == DIFFSTATE_ADDED || pSV->state == DIFFSTATE_REMOVED)
		{
			BasicStru tempBasicStr;
			tempBasicStr.state = pSV->state;
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV控制块", ObjName, Path, "", tempBasicStr);
		}	
	}
}
/** 
  * @brief    ExportGooseDiff
  * 
  * 导出Goose控制块差异
  * 
  * @class    CXLCreat
  * @param    IEDStru *pIED : IED指针
  * @return   void
  * @author   zhang-xch
  * @date     2014/5/22
  * @remarks  
  */ 
void CXLCreat::ExportGooseDiff(IEDStru *pIED)
{
	for (uint i = 0; i < pIED->vGOOSECB.size(); i++)
	{
		GOOSECBStru *pGoose = &pIED->vGOOSECB.at(i);
		QString ObjName = NameTranslator_GetObjNameFromSCDDiffResult(pGoose->ID);
		QString Path = pIED->IEDName + "/" + ObjName;
		if (pGoose->state == DIFFSTATE_EDITED)
		{
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose控制块", ObjName, Path, "AppID", pGoose->AppID);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose控制块", ObjName, Path, "VLanID", pGoose->VLANID);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose控制块", ObjName, Path, "VLan优先级", pGoose->VLANPriority);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose控制块", ObjName, Path, "Mac地址", pGoose->MACAddr);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose控制块", ObjName, Path, "配置版本信息", pGoose->ConfVersion);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose控制块", ObjName, Path, "MaxTime", pGoose->MaxTime);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose控制块", ObjName, Path, "MinTime", pGoose->MinTime);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose控制块", ObjName, Path, "控制块名称", pGoose->CBName);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose控制块", ObjName, Path, "控制块描述", pGoose->CBDesc);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose控制块", ObjName, Path, "数据集名称", pGoose->DataSetName);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose控制块", ObjName, Path, "数据集描述", pGoose->DataSetDesc);
			SetIEDEditDiffAttributeIntValue(pIED->IEDName, "Goose控制块", ObjName, Path, "数据集信号数目", pGoose->DataSetCount);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose控制块", ObjName, Path, "gocbRef", pGoose->GocbRef);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose控制块", ObjName, Path, "数据集地址", pGoose->DataSetAddr);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose控制块", ObjName, Path, "控制块Ap地址", pGoose->apAppID);	
			ExportSignaleDiff(pIED->IEDName ,pGoose);
		}else if (pGoose->state == DIFFSTATE_ADDED || pGoose->state == DIFFSTATE_REMOVED)
		{
			BasicStru tempBasicStr;
			tempBasicStr.state = pGoose->state;
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose控制块", ObjName, Path, "", tempBasicStr);
		}	
	}
}
/** 
  * @brief    ExportInputsDiff
  * 
  * 导出Inputs差异
  * 
  * @class    CXLCreat
  * @param    IEDStru *pIED : 当前IED
  * @return   void
  * @author   zhang-xch
  * @date     2014/5/22
  * @remarks  
  */ 
void CXLCreat::ExportInputsDiff(IEDStru *pIED)
{
	for (uint i = 0; i < pIED->vInput.size(); i++)
	{
		InputStru *pInput = &pIED->vInput.at(i);
		QString ObjName = "虚链接数目";
		SetIEDEditDiffAttributeIntValue(pIED->IEDName, "虚回路", ObjName, pIED->IEDName, "虚链接数目", pInput->ExtRefCount);
		QString IEDName = pIED->IEDName;
		ExportExtRefDiff(IEDName, pInput);
	}
}
/** 
  * @brief    ExportPhyPortDiff
  * 
  * 导出物理端口差异
  * 
  * @class    CXLCreat
  * @param    IEDStru *pIED : 当前IED
  * @return   void
  * @author   zhang-xch
  * @date     2014/5/22
  * @remarks  
  */ 
void CXLCreat::ExportPhyPortDiff(IEDStru *pIED)
{
	for (uint i = 0; i < pIED->vPortStru.size(); i++)
	{
		PhysicsPortStru *pPhyPort = &pIED->vPortStru.at(i);
		QString ObjName = NameTranslator_GetObjNameFromSCDDiffResult(pPhyPort->ID);
		QString Path = pIED->IEDName + "/" + ObjName;
		if (pPhyPort->state == DIFFSTATE_EDITED)
		{
			SetIEDEditDiffAttributeValue(pIED->IEDName, "物理端口", ObjName, Path, "物理端口类型", pPhyPort->transMedia);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "物理端口", ObjName, Path, "物理端口序号", pPhyPort->pIndex);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "物理端口", ObjName, Path, "物理端口插头类型", pPhyPort->plug);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "物理端口", ObjName, Path, "物理端口接口类型", pPhyPort->type);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "物理端口", ObjName, Path, "物理光缆类型", pPhyPort->cable);	
		} else if (pPhyPort->state == DIFFSTATE_ADDED || pPhyPort->state == DIFFSTATE_REMOVED)
		{
			BasicStru tempBasicStr;
			tempBasicStr.state = pPhyPort->state;
			SetIEDEditDiffAttributeValue(pIED->IEDName, "物理端口", ObjName, Path, "", tempBasicStr);
		}
	}
}

/** 
  * @brief    ExportSignaleDiff
  * 
  * 导出Goose输出信号
  * 
  * @class    CXLCreat
  * @param    GOOSECBStru *pGoose : Goose控制块
  * @return   void
  * @author   zhang-xch
  * @date     2014/5/22
  * @remarks  
  */ 
void CXLCreat::ExportSignaleDiff(QString IEDName, GOOSECBStru *pGoose)
{
	for (uint i = 0; i < pGoose->vSignal.size(); i++)
	{
		SignalStru *pSig = &pGoose->vSignal.at(i);
		QString ObjName = NameTranslator_GetObjNameFromSCDDiffResult(pSig->ID);
		QString Path = IEDName + "/" + ObjName;
		if (pSig->state == DIFFSTATE_EDITED)
		{
			SetIEDEditDiffAttributeIntValue(IEDName, "Goose输出信号",ObjName, Path, "序号", pSig->Ord);
			SetIEDEditDiffAttributeValue(IEDName, "Goose输出信号", ObjName, Path, "dU值", pSig->DOIdU);
			SetIEDEditDiffAttributeValue(IEDName, "Goose输出信号",ObjName, Path, "DO描述", pSig->DODesc);
			SetIEDEditDiffAttributeValue(IEDName, "Goose输出信号",ObjName, Path, "LN描述", pSig->LNDesc);
			SetIEDEditDiffAttributeValue(IEDName, "Goose输出信号",ObjName, Path, "功能约束", pSig->FC);
			SetIEDEditDiffAttributeValue(IEDName, "Goose输出信号",ObjName, Path, "BDA类型", pSig->BDAType);
			SetIEDEditDiffAttributeValue(IEDName, "Goose输出信号",ObjName, Path, "压板名称", pSig->TripName);
			SetIEDEditDiffAttributeValue(IEDName, "Goose输出信号",ObjName, Path, "信号描述", pSig->Desc);
			SetIEDEditDiffAttributeValue(IEDName, "Goose输出信号",ObjName, Path, "物理端口索引", pSig->PortIndex);	
		}else  if (pSig->state == DIFFSTATE_ADDED || pSig->state == DIFFSTATE_REMOVED)
		{
			BasicStru tempBasicStr;
			tempBasicStr.state = pSig->state;
			SetIEDEditDiffAttributeValue(IEDName, "Goose输出信号",ObjName, Path, "", tempBasicStr);
		}	
	}
}

/** 
  * @brief    ExportSignaleDiff
  * 
  * 导出SV输出信号
  * 
  * @class    CXLCreat
  * @param    SVCBStru *pSV : SV控制块
  * @return   void
  * @author   zhang-xch
  * @date     2014/5/22
  * @remarks  
  */ 
void CXLCreat::ExportSignaleDiff(QString IEDName, SVCBStru *pSV)
{
	for (uint i = 0; i < pSV->vSignal.size(); i++)
	{
		SignalStru *pSig = &pSV->vSignal.at(i);
		QString ObjName = NameTranslator_GetObjNameFromSCDDiffResult(pSig->ID);
		QString Path = IEDName + "/" + ObjName;
		if (pSig->state == DIFFSTATE_EDITED)
		{
			SetIEDEditDiffAttributeIntValue(IEDName, "SV输出信号", ObjName, Path, "序号", pSig->Ord);
			SetIEDEditDiffAttributeValue(IEDName, "SV输出信号", ObjName, Path, "dU值", pSig->DOIdU);
			SetIEDEditDiffAttributeValue(IEDName, "SV输出信号", ObjName, Path, "DO描述", pSig->DODesc);
			SetIEDEditDiffAttributeValue(IEDName, "SV输出信号", ObjName, Path, "LN描述", pSig->LNDesc);
			SetIEDEditDiffAttributeValue(IEDName, "SV输出信号", ObjName, Path, "功能约束", pSig->FC);
			SetIEDEditDiffAttributeValue(IEDName, "SV输出信号", ObjName, Path, "BDA类型", pSig->BDAType);
			SetIEDEditDiffAttributeValue(IEDName, "SV输出信号", ObjName, Path, "压板名称", pSig->TripName);
			SetIEDEditDiffAttributeValue(IEDName, "SV输出信号", ObjName, Path, "信号描述", pSig->Desc);
			SetIEDEditDiffAttributeValue(IEDName, "SV输出信号", ObjName, Path, "物理端口索引", pSig->PortIndex);	
		}else  if (pSig->state == DIFFSTATE_ADDED || pSig->state == DIFFSTATE_REMOVED)
		{
			BasicStru tempBasicStr;
			tempBasicStr.state = pSig->state;
			SetIEDEditDiffAttributeValue(IEDName, "SV输出信号", ObjName, Path, "", tempBasicStr);
		}	
	}
}

/** 
  * @brief    ExportExtRefDiff
  * 
  * 导出虚回路信息
  * 
  * @class    CXLCreat
  * @param    InputStru *pInput : 当前Input
  * @return   void
  * @author   zhang-xch
  * @date     2014/5/22
  * @remarks  
  */ 
void CXLCreat::ExportExtRefDiff(QString IEDName, InputStru *pInput)
{
	for (uint i = 0; i < pInput->vExtRef.size(); i++)
	{
		ExtRefStru *pExtRef = &pInput->vExtRef.at(i);
		QString ObjName = NameTranslator_GetObjNameFromSCDDiffResult(pExtRef->ID);
		QString Path = IEDName + "/" + ObjName;
		if (pExtRef->state == DIFFSTATE_EDITED)
		{
			SetIEDEditDiffAttributeIntValue(IEDName, "虚回路", ObjName, Path, "序号", pExtRef->Ord);
			SetIEDEditDiffAttributeValue(IEDName, "虚回路", ObjName, Path, "描述", pExtRef->desc);
			SetIEDEditDiffAttributeValue(IEDName, "虚回路", ObjName, Path, "内部地址", pExtRef->IntAddr);
			SetIEDEditDiffAttributeValue(IEDName, "虚回路", ObjName, Path, "外部地址", pExtRef->OutAddr);
			SetIEDEditDiffAttributeValue(IEDName, "虚回路", ObjName, Path, "物理端口名称", pExtRef->PortName);

			SignalStru *pSig = &pExtRef->InSigInfo;
			SetIEDEditDiffAttributeValue(IEDName, "输入信号", ObjName, Path, "dU值", pSig->DOIdU);
			SetIEDEditDiffAttributeValue(IEDName, "输入信号", ObjName, Path, "DO描述", pSig->DODesc);
			SetIEDEditDiffAttributeValue(IEDName, "输入信号", ObjName, Path, "LN描述", pSig->LNDesc);
			SetIEDEditDiffAttributeValue(IEDName, "输入信号", ObjName, Path, "功能约束", pSig->FC);
			SetIEDEditDiffAttributeValue(IEDName, "输入信号", ObjName, Path, "类型", pSig->BDAType);


		}
		else if (pExtRef->state == DIFFSTATE_ADDED)
		{
			BasicStru tempBasicStr;
			tempBasicStr.state = pExtRef->state;
			tempBasicStr.otherAttributeName = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pExtRef->srcSignalID.toStdString())) + "  " +
				QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pExtRef->srcSignalID.toStdString()))
				+ " ----> " + QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pExtRef->destSignalID.toStdString())) + "  " +
				QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pExtRef->destSignalID.toStdString())); 
			SetIEDEditDiffAttributeValue(IEDName, "虚回路", ObjName, Path, "", tempBasicStr);
		}	
		else if (pExtRef->state == DIFFSTATE_REMOVED)
		{
			BasicStru tempBasicStr;
			tempBasicStr.state = pExtRef->state;
			tempBasicStr.baseAttributeName = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pExtRef->srcSignalID.toStdString())) + "  " +
				QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pExtRef->srcSignalID.toStdString()))
				+ " ----> " + QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pExtRef->destSignalID.toStdString())) + "  " +
				QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pExtRef->destSignalID.toStdString())); 

			SetIEDEditDiffAttributeValue(IEDName, "虚回路", ObjName, Path, "", tempBasicStr);
		}
	}
}

/** 
  * @brief    CreatFormat
  * 
  * 创建导出格式
  * 
  * @class    CXLCreat
  * @return   void
  * @author   zhang-xch
  * @date     2014/5/22
  * @remarks  
  */ 
void CXLCreat::CreatFormat()
{
	attrabuteValueFont = book->addFont();
	headerFont = book->addFont();

	attrabuteValueFormat = book->addFormat();
	attrabute1ValueFormat = book->addFormat();
	attrabute2ValueFormat = book->addFormat();
	headerFormat = book->addFormat();
}

/** 
  * @brief    SetFormat
  * 
  * 设置导出格式
  * 
  * @class    CXLCreat
  * @return   void
  * @author   zhang-xch
  * @date     2014/5/22
  * @remarks  
  */ 
void CXLCreat::SetFormat()
{
	attrabuteValueFont->setName(L"宋体");
	attrabuteValueFont->setSize(10);

	headerFont->setBold();		
	headerFont->setName(L"宋体");
	headerFont->setSize(12);

	attrabuteValueFormat->setFont(attrabuteValueFont);
	attrabuteValueFormat->setBorder(BORDERSTYLE_THIN);
	attrabuteValueFormat->setAlignH(ALIGNH_LEFT);
	attrabuteValueFormat->setWrap(true);

	attrabute1ValueFormat->setFont(attrabuteValueFont);
	attrabute1ValueFormat->setBorder(BORDERSTYLE_THIN);
	attrabute1ValueFormat->setAlignH(ALIGNH_LEFT);
	attrabute1ValueFormat->setPatternForegroundColor(COLOR_WHITE);
	attrabute1ValueFormat->setFillPattern(FILLPATTERN_SOLID);
	attrabute1ValueFormat->setWrap(true);

	attrabute2ValueFormat->setFont(attrabuteValueFont);
	attrabute2ValueFormat->setBorder(BORDERSTYLE_THIN);
	attrabute2ValueFormat->setAlignH(ALIGNH_LEFT);
	attrabute2ValueFormat->setPatternForegroundColor(COLOR_GRAY25);
	attrabute2ValueFormat->setFillPattern(FILLPATTERN_SOLID);
	attrabute2ValueFormat->setWrap(true);

	headerFormat->setFont(headerFont);
	headerFormat->setBorder(BORDERSTYLE_THIN);
	headerFormat->setAlignH(ALIGNH_LEFT);
	headerFormat->setAlignV(ALIGNV_CENTER);
	headerFormat->setFillPattern(FILLPATTERN_SOLID);
	headerFormat->setPatternForegroundColor(COLOR_TAN);
	headerFormat->setWrap(true);
}

/** 
  * @brief    InitPoint
  * 
  * 初始化起始点
  * 
  * @class    CXLCreat
  * @return   void
  * @author   zhang-xch
  * @date     2014/5/22
  * @remarks  
  */ 
void CXLCreat::InitPoint()
{
	m_CurentPoint.row = 1;
	m_CurentPoint.col = 0;
	iOrd = 1;
}

/** 
  * @brief    SetAttributeValue
  * 
  * 设置属性
  * 
  * @class    CXLCreat
  * @param    QString attribute         : 属性名称
  * @param    BasicStru attributeValue  : 属性
  * @return   void
  * @author   zhang-xch
  * @date     2014/5/22
  * @remarks  
  */ 
void CXLCreat::SetIEDDiffAttributeValue(IEDStru* pIED)
{
	QString IEDState;
	if (pIED->state == DIFFSTATE_ADDED)
	{
		IEDState = "增加";
	}else if (pIED->state == DIFFSTATE_REMOVED)
	{
		IEDState = "删除";
	}
	 
	sheetIEDDiff->writeNum(m_CurentPoint.row, m_CurentPoint.col, iOrd, attrabuteValueFormat);
	sheetIEDDiff->writeStr(m_CurentPoint.row, m_CurentPoint.col + 1, reinterpret_cast<const wchar_t*>(pIED->IEDName.utf16()), attrabuteValueFormat);
	sheetIEDDiff->writeStr(m_CurentPoint.row, m_CurentPoint.col + 2, reinterpret_cast<const wchar_t*>(IEDState.utf16()), attrabuteValueFormat);
	iOrd++;
	m_CurentPoint.row++;
}

void CXLCreat::SetFileInfoDiffAttributeValue(BasicStru pBasicStr, QString AttributeName)
{
	QString AttributeState;
	if (pBasicStr.state == DIFFSTATE_ADDED)
	{
		AttributeState = "增加";
	}else if (pBasicStr.state == DIFFSTATE_REMOVED)
	{
		AttributeState = "删除";
	}else if (pBasicStr.state == DIFFSTATE_EDITED)
	{
		AttributeState = "修改";
	}else
	{
		return;
	}
	sheetDocInfo->writeNum(m_CurentPoint.row, m_CurentPoint.col, iOrd, attrabuteValueFormat);
	sheetDocInfo->writeStr(m_CurentPoint.row, m_CurentPoint.col + 1, reinterpret_cast<const wchar_t*>(AttributeName.utf16()), attrabuteValueFormat);
	sheetDocInfo->writeStr(m_CurentPoint.row, m_CurentPoint.col + 2, reinterpret_cast<const wchar_t*>(AttributeState.utf16()), attrabuteValueFormat);
	sheetDocInfo->writeStr(m_CurentPoint.row, m_CurentPoint.col + 3, reinterpret_cast<const wchar_t*>(pBasicStr.baseAttributeName.utf16()), attrabuteValueFormat);
	sheetDocInfo->writeStr(m_CurentPoint.row, m_CurentPoint.col + 4, reinterpret_cast<const wchar_t*>(pBasicStr.otherAttributeName.utf16()), attrabuteValueFormat);
	iOrd++;
	m_CurentPoint.row++;
}

void CXLCreat::SetIEDEditDiffAttributeValue(QString IEDName, QString DiffType, QString qObject, QString Path, QString AttributeName, BasicStru pBasicStr)
{
	QString AttributeState;
	if (pBasicStr.state == DIFFSTATE_ADDED)
	{
		AttributeState = "增加";
	}else if (pBasicStr.state == DIFFSTATE_REMOVED)
	{
		AttributeState = "删除";
	}else if (pBasicStr.state == DIFFSTATE_EDITED)
	{
		AttributeState = "修改";
	}else
	{
		return;
	}

	sheetIEDEdit->writeNum(m_CurentPoint.row, m_CurentPoint.col, iOrd, pFormat);
	sheetIEDEdit->writeStr(m_CurentPoint.row, m_CurentPoint.col + 1, reinterpret_cast<const wchar_t*>(IEDName.utf16()), pFormat);
	sheetIEDEdit->writeStr(m_CurentPoint.row, m_CurentPoint.col + 2, reinterpret_cast<const wchar_t*>(DiffType.utf16()), pFormat);
	sheetIEDEdit->writeStr(m_CurentPoint.row, m_CurentPoint.col + 3, reinterpret_cast<const wchar_t*>(qObject.utf16()), pFormat);
	sheetIEDEdit->writeStr(m_CurentPoint.row, m_CurentPoint.col + 4, reinterpret_cast<const wchar_t*>(AttributeState.utf16()), pFormat);
	sheetIEDEdit->writeStr(m_CurentPoint.row, m_CurentPoint.col + 5, reinterpret_cast<const wchar_t*>(Path.utf16()), pFormat);
	sheetIEDEdit->writeStr(m_CurentPoint.row, m_CurentPoint.col + 6, reinterpret_cast<const wchar_t*>(AttributeName.utf16()), pFormat);
	sheetIEDEdit->writeStr(m_CurentPoint.row, m_CurentPoint.col + 7, reinterpret_cast<const wchar_t*>(pBasicStr.baseAttributeName.utf16()), pFormat);
	sheetIEDEdit->writeStr(m_CurentPoint.row, m_CurentPoint.col + 8, reinterpret_cast<const wchar_t*>(pBasicStr.otherAttributeName.utf16()), pFormat);
	iOrd++;
	m_CurentPoint.row++;
}
void CXLCreat::SetIEDEditDiffAttributeIntValue(QString IEDName, QString DiffType, QString qObject, QString Path, QString AttributeName, BasicIntStru pBasicStr)
{
	QString AttributeState;
	if (pBasicStr.state == DIFFSTATE_ADDED)
	{
		AttributeState = "增加";
	}else if (pBasicStr.state == DIFFSTATE_REMOVED)
	{
		AttributeState = "删除";
	}else if (pBasicStr.state == DIFFSTATE_EDITED)
	{
		AttributeState = "修改";
	}else
	{
		return;
	}


	sheetIEDEdit->writeNum(m_CurentPoint.row, m_CurentPoint.col, iOrd, pFormat);
	sheetIEDEdit->writeStr(m_CurentPoint.row, m_CurentPoint.col + 1, reinterpret_cast<const wchar_t*>(IEDName.utf16()), pFormat);
	sheetIEDEdit->writeStr(m_CurentPoint.row, m_CurentPoint.col + 2, reinterpret_cast<const wchar_t*>(DiffType.utf16()), pFormat);
	sheetIEDEdit->writeStr(m_CurentPoint.row, m_CurentPoint.col + 3, reinterpret_cast<const wchar_t*>(qObject.utf16()), pFormat);
	sheetIEDEdit->writeStr(m_CurentPoint.row, m_CurentPoint.col + 4, reinterpret_cast<const wchar_t*>(AttributeState.utf16()), pFormat);
	sheetIEDEdit->writeStr(m_CurentPoint.row, m_CurentPoint.col + 5, reinterpret_cast<const wchar_t*>(Path.utf16()), pFormat);
	sheetIEDEdit->writeStr(m_CurentPoint.row, m_CurentPoint.col + 6, reinterpret_cast<const wchar_t*>(AttributeName.utf16()), pFormat);
	if (pBasicStr.baseAttributeName >0)
	{
		sheetIEDEdit->writeNum(m_CurentPoint.row, m_CurentPoint.col + 7, pBasicStr.baseAttributeName, pFormat);
	}
	if (pBasicStr.otherAttributeName >0)
	{
		sheetIEDEdit->writeNum(m_CurentPoint.row, m_CurentPoint.col + 8, pBasicStr.otherAttributeName, pFormat);
	}
	iOrd++;
	m_CurentPoint.row++;
}

void CXLCreat::SaveFile()
{
	QString fileName = QFileDialog::getSaveFileName(NULL, "保存差异结果", "差异化比较结果.xls", "*.xls");  
	if (!fileName.isEmpty())  
	{  
		if (book->save(reinterpret_cast<const wchar_t*>(fileName.utf16())))
		{
			QMessageBox::about(NULL, "保存文件", "文件保存成功！");
		}else {
			QMessageBox::warning(NULL, "保存文件", "文件保存失败,请检查文件是否在其他程序中打开!");
		}
	} 
}

QString CXLCreat::NameTranslator_GetObjNameFromSCDDiffResult(QString qString)
{
	return QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(qString.toStdString()));
}