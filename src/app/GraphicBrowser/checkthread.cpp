#include "checkthread.h"

CheckThread::CheckThread(CSCLValid *sclValid)
{
	m_SCLValid = sclValid;
}

CheckThread::~CheckThread()
{

}

void CheckThread::run()
{
	m_SCLValid->StartCheck();
	emit checkFinish();
}

