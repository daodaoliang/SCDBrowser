#include "comparethread.h"

CompareThread::CompareThread(SCDDiffCompare * scd)
{
	scdDiffCompare=scd;
	baseKey="";
	otherKey="";
}

void CompareThread::run()
{
	 bool result = scdDiffCompare->GetTotalDiff(baseKey, otherKey);
	 emit compareFinish(result);
}
CompareThread::~CompareThread()
{
	 
}
void CompareThread::setKey(QString bKey,QString oKey)
{
	baseKey=bKey;
	otherKey=oKey;
}
