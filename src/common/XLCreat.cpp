#include "XLCreat.h"
#include <QMessageBox>
#include <QFileDialog>
#include "SCLNameTranslator.h"

CXLCreat::CXLCreat(void)
{
	book = xlCreateBook();
	if (book)
	{
		book->setKey(L"�����ݸ��Ƽ����޹�˾", L"windows-26292c0705cde60766bc6667a2hce3l9");
		
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
	sheetDocInfo = book->addSheet(L"�汾��Ϣ");
	AddFileInfoTableHead(sheetDocInfo);
	ExportFileInFoDiff();
}
void CXLCreat::AddFileInfoTableHead(Sheet* pSheet)
{
	pSheet->setCol(0, 0, 10);
	pSheet->setCol(1, 4, 25);
	pSheet->writeStr(0, 0, L"���", headerFormat);
	pSheet->writeStr(0, 1, L"��������", headerFormat);
	pSheet->writeStr(0, 2, L"�������", headerFormat);
	pSheet->writeStr(0, 3, L"��׼�ļ��е�����", headerFormat);
	pSheet->writeStr(0, 4, L"�Ƚ��ļ��е�����", headerFormat);
}
/** 
  * @brief    ExportFileInFoDiff
  * 
  * �����ļ���Ϣ����
  * 
  * @class    CXLCreat
  * @return   void
  * @author   zhang-xch
  * @date     2014/5/22
  * @remarks  
  */ 
void CXLCreat::ExportFileInFoDiff()
{
	SetFileInfoDiffAttributeValue(m_SCDDiffCompare->m_SCDDiffResult.sFileInfo.version, "�汾��Ϣ");
	SetFileInfoDiffAttributeValue(m_SCDDiffCompare->m_SCDDiffResult.sFileInfo.reVersion, "�޶��汾��Ϣ");
	SetFileInfoDiffAttributeValue(m_SCDDiffCompare->m_SCDDiffResult.sFileInfo.crc, "CRCУ����");
}

void CXLCreat::CreatIEDDiffSheet()
{
	InitPoint();
	sheetIEDDiff = book->addSheet(L"���ӻ�ɾ����IED");
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
	pSheet->writeStr(0, 0, L"���", headerFormat);
	pSheet->writeStr(0, 1, L"�������", headerFormat);
	pSheet->writeStr(0, 2, L"�������", headerFormat);
}

void CXLCreat::CreatIEDEditDiffSheet()
{
	InitPoint();
	sheetIEDEdit = book->addSheet(L"�޸ĵ�IED");
	AddIEDEditTableHead(sheetIEDEdit);
	ExportIEDDiff();
}
void CXLCreat::AddIEDEditTableHead(Sheet* pSheet)
{
	pSheet->setCol(0, 0, 10);
	pSheet->setCol(1, 8, 20);
	pSheet->writeStr(0, 0, L"���", headerFormat);
	pSheet->writeStr(0, 1, L"IED����", headerFormat);
	pSheet->writeStr(0, 2, L"����", headerFormat);
	pSheet->writeStr(0, 3, L"�������", headerFormat);
	pSheet->writeStr(0, 4, L"�������", headerFormat);
	pSheet->writeStr(0, 5, L"����·��", headerFormat);
	pSheet->writeStr(0, 6, L"��������", headerFormat);
	pSheet->writeStr(0, 7, L"��׼�ļ��е�����", headerFormat);
	pSheet->writeStr(0, 8, L"�Ƚ��ļ��е�����", headerFormat);
}
/** 
  * @brief    ExportIEDDiff
  * 
  * ����IED����
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
			SetIEDEditDiffAttributeValue(pIED->IEDName, "IED", pIED->IEDName, pIED->IEDName, "IED����", pIED->IEDDesc);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "IED", pIED->IEDName, pIED->IEDName, "IED����", pIED->IEDType);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "IED", pIED->IEDName, pIED->IEDName, "IED����", pIED->IEDManu);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "IED", pIED->IEDName, pIED->IEDName, "IED CRCУ����", pIED->IEDCRC);
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
  * ����SV���ƿ����
  * 
  * @class    CXLCreat
  * @param    IEDStru *pIED : IEDָ��
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
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV���ƿ�", ObjName, Path, "AppID", pSV->AppID);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV���ƿ�", ObjName, Path, "VLanID", pSV->VLANID);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV���ƿ�", ObjName, Path, "VLan���ȼ�", pSV->VLANPriority);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV���ƿ�", ObjName, Path, "Mac��ַ", pSV->MACAddr);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV���ƿ�", ObjName, Path, "���ð汾��Ϣ", pSV->ConfVersion);
			SetIEDEditDiffAttributeIntValue(pIED->IEDName, "SV���ƿ�", ObjName, Path, "ASDU��Ŀ", pSV->ASDUCount);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV���ƿ�", ObjName, Path, "������", pSV->smpRate);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV���ƿ�", ObjName, Path, "���ƿ�����", pSV->CBName);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV���ƿ�", ObjName, Path, "���ƿ�����", pSV->CBDesc);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV���ƿ�", ObjName, Path, "���ݼ�����", pSV->DataSetName);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV���ƿ�", ObjName, Path, "���ݼ�����", pSV->DataSetDesc);
			SetIEDEditDiffAttributeIntValue(pIED->IEDName, "SV���ƿ�", ObjName, Path, "���ݼ��ź���Ŀ", pSV->DataSetCount);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV���ƿ�", ObjName, Path, "���ݼ���ַ", pSV->DataSetAddr);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV���ƿ�", ObjName, Path, "���ƿ�Ap��ַ", pSV->apAppID);	
			ExportSignaleDiff(pIED->IEDName ,pSV);
		}else if (pSV->state == DIFFSTATE_ADDED || pSV->state == DIFFSTATE_REMOVED)
		{
			BasicStru tempBasicStr;
			tempBasicStr.state = pSV->state;
			SetIEDEditDiffAttributeValue(pIED->IEDName, "SV���ƿ�", ObjName, Path, "", tempBasicStr);
		}	
	}
}
/** 
  * @brief    ExportGooseDiff
  * 
  * ����Goose���ƿ����
  * 
  * @class    CXLCreat
  * @param    IEDStru *pIED : IEDָ��
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
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose���ƿ�", ObjName, Path, "AppID", pGoose->AppID);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose���ƿ�", ObjName, Path, "VLanID", pGoose->VLANID);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose���ƿ�", ObjName, Path, "VLan���ȼ�", pGoose->VLANPriority);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose���ƿ�", ObjName, Path, "Mac��ַ", pGoose->MACAddr);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose���ƿ�", ObjName, Path, "���ð汾��Ϣ", pGoose->ConfVersion);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose���ƿ�", ObjName, Path, "MaxTime", pGoose->MaxTime);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose���ƿ�", ObjName, Path, "MinTime", pGoose->MinTime);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose���ƿ�", ObjName, Path, "���ƿ�����", pGoose->CBName);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose���ƿ�", ObjName, Path, "���ƿ�����", pGoose->CBDesc);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose���ƿ�", ObjName, Path, "���ݼ�����", pGoose->DataSetName);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose���ƿ�", ObjName, Path, "���ݼ�����", pGoose->DataSetDesc);
			SetIEDEditDiffAttributeIntValue(pIED->IEDName, "Goose���ƿ�", ObjName, Path, "���ݼ��ź���Ŀ", pGoose->DataSetCount);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose���ƿ�", ObjName, Path, "gocbRef", pGoose->GocbRef);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose���ƿ�", ObjName, Path, "���ݼ���ַ", pGoose->DataSetAddr);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose���ƿ�", ObjName, Path, "���ƿ�Ap��ַ", pGoose->apAppID);	
			ExportSignaleDiff(pIED->IEDName ,pGoose);
		}else if (pGoose->state == DIFFSTATE_ADDED || pGoose->state == DIFFSTATE_REMOVED)
		{
			BasicStru tempBasicStr;
			tempBasicStr.state = pGoose->state;
			SetIEDEditDiffAttributeValue(pIED->IEDName, "Goose���ƿ�", ObjName, Path, "", tempBasicStr);
		}	
	}
}
/** 
  * @brief    ExportInputsDiff
  * 
  * ����Inputs����
  * 
  * @class    CXLCreat
  * @param    IEDStru *pIED : ��ǰIED
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
		QString ObjName = "��������Ŀ";
		SetIEDEditDiffAttributeIntValue(pIED->IEDName, "���·", ObjName, pIED->IEDName, "��������Ŀ", pInput->ExtRefCount);
		QString IEDName = pIED->IEDName;
		ExportExtRefDiff(IEDName, pInput);
	}
}
/** 
  * @brief    ExportPhyPortDiff
  * 
  * ��������˿ڲ���
  * 
  * @class    CXLCreat
  * @param    IEDStru *pIED : ��ǰIED
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
			SetIEDEditDiffAttributeValue(pIED->IEDName, "����˿�", ObjName, Path, "����˿�����", pPhyPort->transMedia);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "����˿�", ObjName, Path, "����˿����", pPhyPort->pIndex);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "����˿�", ObjName, Path, "����˿ڲ�ͷ����", pPhyPort->plug);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "����˿�", ObjName, Path, "����˿ڽӿ�����", pPhyPort->type);
			SetIEDEditDiffAttributeValue(pIED->IEDName, "����˿�", ObjName, Path, "�����������", pPhyPort->cable);	
		} else if (pPhyPort->state == DIFFSTATE_ADDED || pPhyPort->state == DIFFSTATE_REMOVED)
		{
			BasicStru tempBasicStr;
			tempBasicStr.state = pPhyPort->state;
			SetIEDEditDiffAttributeValue(pIED->IEDName, "����˿�", ObjName, Path, "", tempBasicStr);
		}
	}
}

/** 
  * @brief    ExportSignaleDiff
  * 
  * ����Goose����ź�
  * 
  * @class    CXLCreat
  * @param    GOOSECBStru *pGoose : Goose���ƿ�
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
			SetIEDEditDiffAttributeIntValue(IEDName, "Goose����ź�",ObjName, Path, "���", pSig->Ord);
			SetIEDEditDiffAttributeValue(IEDName, "Goose����ź�", ObjName, Path, "dUֵ", pSig->DOIdU);
			SetIEDEditDiffAttributeValue(IEDName, "Goose����ź�",ObjName, Path, "DO����", pSig->DODesc);
			SetIEDEditDiffAttributeValue(IEDName, "Goose����ź�",ObjName, Path, "LN����", pSig->LNDesc);
			SetIEDEditDiffAttributeValue(IEDName, "Goose����ź�",ObjName, Path, "����Լ��", pSig->FC);
			SetIEDEditDiffAttributeValue(IEDName, "Goose����ź�",ObjName, Path, "BDA����", pSig->BDAType);
			SetIEDEditDiffAttributeValue(IEDName, "Goose����ź�",ObjName, Path, "ѹ������", pSig->TripName);
			SetIEDEditDiffAttributeValue(IEDName, "Goose����ź�",ObjName, Path, "�ź�����", pSig->Desc);
			SetIEDEditDiffAttributeValue(IEDName, "Goose����ź�",ObjName, Path, "����˿�����", pSig->PortIndex);	
		}else  if (pSig->state == DIFFSTATE_ADDED || pSig->state == DIFFSTATE_REMOVED)
		{
			BasicStru tempBasicStr;
			tempBasicStr.state = pSig->state;
			SetIEDEditDiffAttributeValue(IEDName, "Goose����ź�",ObjName, Path, "", tempBasicStr);
		}	
	}
}

/** 
  * @brief    ExportSignaleDiff
  * 
  * ����SV����ź�
  * 
  * @class    CXLCreat
  * @param    SVCBStru *pSV : SV���ƿ�
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
			SetIEDEditDiffAttributeIntValue(IEDName, "SV����ź�", ObjName, Path, "���", pSig->Ord);
			SetIEDEditDiffAttributeValue(IEDName, "SV����ź�", ObjName, Path, "dUֵ", pSig->DOIdU);
			SetIEDEditDiffAttributeValue(IEDName, "SV����ź�", ObjName, Path, "DO����", pSig->DODesc);
			SetIEDEditDiffAttributeValue(IEDName, "SV����ź�", ObjName, Path, "LN����", pSig->LNDesc);
			SetIEDEditDiffAttributeValue(IEDName, "SV����ź�", ObjName, Path, "����Լ��", pSig->FC);
			SetIEDEditDiffAttributeValue(IEDName, "SV����ź�", ObjName, Path, "BDA����", pSig->BDAType);
			SetIEDEditDiffAttributeValue(IEDName, "SV����ź�", ObjName, Path, "ѹ������", pSig->TripName);
			SetIEDEditDiffAttributeValue(IEDName, "SV����ź�", ObjName, Path, "�ź�����", pSig->Desc);
			SetIEDEditDiffAttributeValue(IEDName, "SV����ź�", ObjName, Path, "����˿�����", pSig->PortIndex);	
		}else  if (pSig->state == DIFFSTATE_ADDED || pSig->state == DIFFSTATE_REMOVED)
		{
			BasicStru tempBasicStr;
			tempBasicStr.state = pSig->state;
			SetIEDEditDiffAttributeValue(IEDName, "SV����ź�", ObjName, Path, "", tempBasicStr);
		}	
	}
}

/** 
  * @brief    ExportExtRefDiff
  * 
  * �������·��Ϣ
  * 
  * @class    CXLCreat
  * @param    InputStru *pInput : ��ǰInput
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
			SetIEDEditDiffAttributeIntValue(IEDName, "���·", ObjName, Path, "���", pExtRef->Ord);
			SetIEDEditDiffAttributeValue(IEDName, "���·", ObjName, Path, "����", pExtRef->desc);
			SetIEDEditDiffAttributeValue(IEDName, "���·", ObjName, Path, "�ڲ���ַ", pExtRef->IntAddr);
			SetIEDEditDiffAttributeValue(IEDName, "���·", ObjName, Path, "�ⲿ��ַ", pExtRef->OutAddr);
			SetIEDEditDiffAttributeValue(IEDName, "���·", ObjName, Path, "����˿�����", pExtRef->PortName);

			SignalStru *pSig = &pExtRef->InSigInfo;
			SetIEDEditDiffAttributeValue(IEDName, "�����ź�", ObjName, Path, "dUֵ", pSig->DOIdU);
			SetIEDEditDiffAttributeValue(IEDName, "�����ź�", ObjName, Path, "DO����", pSig->DODesc);
			SetIEDEditDiffAttributeValue(IEDName, "�����ź�", ObjName, Path, "LN����", pSig->LNDesc);
			SetIEDEditDiffAttributeValue(IEDName, "�����ź�", ObjName, Path, "����Լ��", pSig->FC);
			SetIEDEditDiffAttributeValue(IEDName, "�����ź�", ObjName, Path, "����", pSig->BDAType);


		}
		else if (pExtRef->state == DIFFSTATE_ADDED)
		{
			BasicStru tempBasicStr;
			tempBasicStr.state = pExtRef->state;
			tempBasicStr.otherAttributeName = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pExtRef->srcSignalID.toStdString())) + "  " +
				QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pExtRef->srcSignalID.toStdString()))
				+ " ----> " + QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pExtRef->destSignalID.toStdString())) + "  " +
				QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pExtRef->destSignalID.toStdString())); 
			SetIEDEditDiffAttributeValue(IEDName, "���·", ObjName, Path, "", tempBasicStr);
		}	
		else if (pExtRef->state == DIFFSTATE_REMOVED)
		{
			BasicStru tempBasicStr;
			tempBasicStr.state = pExtRef->state;
			tempBasicStr.baseAttributeName = QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pExtRef->srcSignalID.toStdString())) + "  " +
				QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pExtRef->srcSignalID.toStdString()))
				+ " ----> " + QString::fromStdString(m_NameTranslator.GetIEDNameFromSCDDiffResult(pExtRef->destSignalID.toStdString())) + "  " +
				QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(pExtRef->destSignalID.toStdString())); 

			SetIEDEditDiffAttributeValue(IEDName, "���·", ObjName, Path, "", tempBasicStr);
		}
	}
}

/** 
  * @brief    CreatFormat
  * 
  * ����������ʽ
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
  * ���õ�����ʽ
  * 
  * @class    CXLCreat
  * @return   void
  * @author   zhang-xch
  * @date     2014/5/22
  * @remarks  
  */ 
void CXLCreat::SetFormat()
{
	attrabuteValueFont->setName(L"����");
	attrabuteValueFont->setSize(10);

	headerFont->setBold();		
	headerFont->setName(L"����");
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
  * ��ʼ����ʼ��
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
  * ��������
  * 
  * @class    CXLCreat
  * @param    QString attribute         : ��������
  * @param    BasicStru attributeValue  : ����
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
		IEDState = "����";
	}else if (pIED->state == DIFFSTATE_REMOVED)
	{
		IEDState = "ɾ��";
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
		AttributeState = "����";
	}else if (pBasicStr.state == DIFFSTATE_REMOVED)
	{
		AttributeState = "ɾ��";
	}else if (pBasicStr.state == DIFFSTATE_EDITED)
	{
		AttributeState = "�޸�";
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
		AttributeState = "����";
	}else if (pBasicStr.state == DIFFSTATE_REMOVED)
	{
		AttributeState = "ɾ��";
	}else if (pBasicStr.state == DIFFSTATE_EDITED)
	{
		AttributeState = "�޸�";
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
		AttributeState = "����";
	}else if (pBasicStr.state == DIFFSTATE_REMOVED)
	{
		AttributeState = "ɾ��";
	}else if (pBasicStr.state == DIFFSTATE_EDITED)
	{
		AttributeState = "�޸�";
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
	QString fileName = QFileDialog::getSaveFileName(NULL, "���������", "���컯�ȽϽ��.xls", "*.xls");  
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

QString CXLCreat::NameTranslator_GetObjNameFromSCDDiffResult(QString qString)
{
	return QString::fromStdString(m_NameTranslator.GetObjNameFromSCDDiffResult(qString.toStdString()));
}