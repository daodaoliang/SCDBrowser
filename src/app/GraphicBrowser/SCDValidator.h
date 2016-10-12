#ifndef SCDVALIDATOR_H
#define SCDVALIDATOR_H

#include "qvector.h"
#include "qstring.h"
// #include "rcSCLOperator.h"

using namespace std;

class SCDValidator
{
private:
	int errorSize;
	int warningSize;

	QString m_version;
	QString m_reVersion;
	QString m_Name;

	QVector<QString> errorNameVec;
	QVector<QString> errorValueVec;

	QVector<QString> warningNameVec;
	QVector<QString> warningValueVec;
	
	//void loadErrorVec(PVALIDINFOSTRUA pErro);
	//void loadWarningVec(PVALIDINFOSTRUA pWarn);

public:
	SCDValidator();
	~SCDValidator();

	QString getVersion();
	QString getReversion();
	QString getName();

	int getErrorSize();
	int getWarningSize();

	QVector<QString> getErrorNameVec();
	QVector<QString> getErrorValueVec();

	QVector<QString> getWarningNameVec();
	QVector<QString> getWarningValueVec();
	
	void SCDValidate(QString path);
};
#endif