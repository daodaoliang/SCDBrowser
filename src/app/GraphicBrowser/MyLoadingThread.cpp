#include "MyLoadingThread.h"
#include <QDebug>

CMyLoadingThread::CMyLoadingThread(void)
{
	scdManage = SCDManger::GetInstance();
}


CMyLoadingThread::~CMyLoadingThread(void)
{
	
}

void CMyLoadingThread::setFilePath(QString filePath)
{
	m_FilePath = filePath;
}

void CMyLoadingThread::run()
{
	qDebug()<<::GetCurrentProcessId()<<"CMyLoadingThread::run	scdManage->AddTransformerSubstation	Start";
	int loadResult = scdManage->AddTransformerSubstation("",m_FilePath);
	qDebug()<<::GetCurrentProcessId()<<"CMyLoadingThread::run	scdManage->AddTransformerSubstation	End";
	emit addSubstationFinish(loadResult);
	qDebug()<<::GetCurrentProcessId()<<"CMyLoadingThread::run	emit addSubstationFinish(loadResult);	Ok";
}