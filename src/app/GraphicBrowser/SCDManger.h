#ifndef SCDMANGER_H
#define SCDMANGER_H

#include <Windows.h>
#include "QtCore\QFile.h"
#include "QtCore\QDataStream.h"
#include "SCLModelSvrAdapter.h"
#include <QDebug>
#include "SCLModelAdapterForClient.h"//chenxin add 2014.10.17

//SCD管理器
class SCDManger
{
public:
	static SCDManger * GetInstance() 
	{  
		if (m_pInstance == NULL)
			m_pInstance = new SCDManger();  
		return m_pInstance; 
	};

	//static DWORD WINAPI threadFunc(LPVOID lpParamter)
	//{
	//	SCLModelAdapter::GetInstance()->AddSubstation(m_pInstance->curVoltageLevelName,m_pInstance->curTransSubName,m_pInstance->fileData);
	//	return 0;
	//}

	QString GetSubstationImportTime()
	{
		return sclModelSvrAdapter->GetSubstationImportTime(curTransSubName);//chenxin modify 2014.10.17
	}

	int  AddCurVoltageLevel()
	{
		if(sclModelSvrAdapter->IsVoltageLevelExist(curVoltageLevelName))
		{
			return -1;//已经存在
		}
		return sclModelSvrAdapter->AddVoltageLevel(curVoltageLevelName);
	}

	int  DeleteCurVoltageLevel()
	{
		//if(!modelInterface->IsVoltageLevelExist(curVoltageLevelName))
		//{
		//	return -1;//不存在
		//}
		return sclModelSvrAdapter->DeleteVoltageLevel(curVoltageLevelName);
	}

	int  AddTransformerSubstation(QString transSubName,QString SCDFilePath)
	{
		//QString substationKey = sclModelSvrAdapter->GetChildKeyFromParentKeyAndChildName(curVoltageLevelName,transSubName);
		//if(modelInterface->IsSubstationExist(substationKey))
		//{
		//	return -1;//已经存在
		//}

		//读取文件信息
		if (SCDFilePath.isEmpty())
			return 0;

		qDebug()<<::GetCurrentProcessId()<<"CMyLoadingThread::run	AddTransformerSubstation	1";
		QFile file(SCDFilePath);
		if (!file.open(QIODevice::ReadOnly)) 
		{
			qDebug()<<::GetCurrentProcessId()<<"CMyLoadingThread::run	AddTransformerSubstation	1	false";
			return 0;
		}

		//QDataStream stream(&file);
		//curTransSubName = transSubName;
		//fileData = stream.device()->read(100000000);

		//HANDLE hThread = CreateThread(NULL,0,threadFunc,NULL,0,NULL);
		//CloseHandle(hThread);
		qDebug()<<::GetCurrentProcessId()<<"CMyLoadingThread::run	AddTransformerSubstation	2";

		//chenxin add 2014.10.20 ---> 装载SCD前先将已经装载的卸载
		SclDacLib sclDacLib;
		for(uint i=0;i<sclDacLib.GetLoadedFileMarks().size();i++)
		{
			sclDacLib.UnloadScdFile(sclDacLib.GetLoadedFileMarks().at(i));
		}
		//chenxin add 2014.10.20 <---

		bool isSuc = sclModelSvrAdapter->AddSubstation(curVoltageLevelName,SCDFilePath);//,stream.device()->read(100000000));
		if (!isSuc)
		{
			qDebug()<<::GetCurrentProcessId()<<"CMyLoadingThread::run	AddTransformerSubstation	2	false";
			return false;
		}
		qDebug()<<::GetCurrentProcessId()<<"CMyLoadingThread::run	AddTransformerSubstation	3";
		return true;

		//return modelInterface->AddSubstation(curVoltageLevelName,transSubName,stream.device()->read(100000000));
	}

	int  DeleteCurTransformerSubstation()
	{
		QString substationKey = sclModelSvrAdapter->GetChildKeyFromParentKeyAndChildName(curVoltageLevelName,curTransSubName);
		//if(!modelInterface->IsSubstationExist(substationKey))
		//{
		//	return -1;//不存在
		//}
		return sclModelSvrAdapter->DeleteSubstation(substationKey);
	}

	int	 DeleteSubstation(QString substationKey)
	{
		return sclModelSvrAdapter->DeleteSubstation(substationKey);
	}

	int  UpdateTransformerSubstationModel(QString transSubName,QString SCDFilePath)
	{
		return sclModelSvrAdapter->UpdateSubstationModel(curVoltageLevelName,transSubName,SCDFilePath);
	}

	int  GetCurSubStationProcProgress()
	{
		return sclModelSvrAdapter->GetSubstationAddedProgress();
	}

public:
	SCDManger()
	{
		sclModelSvrAdapter = SCLModelAdapter::GetInstance();
		curVoltageLevelName = "220kV";
	};

	~SCDManger(){};

private:
	static SCDManger  *m_pInstance;

	class CGarbo
	{  
	public:  ~CGarbo()  
			 {  
				 if (SCDManger::m_pInstance) 
				 {
					 delete SCDManger::m_pInstance;  
				 }
			 }
	};
	static CGarbo Garbo;

	QString curVoltageLevelName;
	QString curTransSubName;
	//QByteArray fileData;

    SCLModelSvrAdapterInterface *sclModelSvrAdapter;
};

#endif