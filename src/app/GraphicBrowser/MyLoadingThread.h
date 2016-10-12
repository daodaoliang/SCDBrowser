#pragma once
#include <QThread>
#include "SCDManger.h"

class CMyLoadingThread : public QThread
{
	Q_OBJECT
public:
	CMyLoadingThread(void);
	~CMyLoadingThread(void);

	void setFilePath(QString filePath);

signals:
	void addSubstationFinish(int loadResult);

protected:
	void run();

private:
	SCDManger	*scdManage;
	QString		m_FilePath; 
};