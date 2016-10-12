#ifndef CHECKTHREAD_H
#define CHECKTHREAD_H

#include <QThread>
#include "SCLValid.h"

class CheckThread : public QThread
{
	Q_OBJECT

public:
	CheckThread(CSCLValid *sclValid);
	~CheckThread();
	void setKey(QString filePath);

signals:
	void checkFinish();
protected:
	void run();

private:
	CSCLValid *m_SCLValid;
};

#endif // CHECKTHREAD_H
