#ifndef COMPARETHREAD_H
#define COMPARETHREAD_H

#include <QThread>
#include "SCDDifferCompare.h"

class CompareThread : public QThread
{
	Q_OBJECT

public:
	CompareThread(SCDDiffCompare * scd);
	~CompareThread();
	void setKey(QString bKey,QString oKey);

signals:
	void compareFinish(bool result);
protected:
	void run();

private:
	SCDDiffCompare	   *scdDiffCompare;
	QString baseKey;
	QString otherKey;
};

#endif // COMPARETHREAD_H
