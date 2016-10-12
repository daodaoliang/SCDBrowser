#include "SCDValidator.h"

SCDValidator::SCDValidator()
{

}

SCDValidator::~SCDValidator()
{
}

//void SCDValidator::loadErrorVec(PVALIDINFOSTRUA pErro)
//{
//	errorNameVec.clear();
//	errorValueVec.clear();
//	while (pErro != NULL)
//	{
//		errorNameVec.push_back(pErro->chName);
//		errorValueVec.push_back(pErro->chValue);
//		pErro = pErro->pNext;
//	}
//}

//void SCDValidator::loadWarningVec(PVALIDINFOSTRUA pWarn)
//{
//	warningNameVec.clear();
//	warningValueVec.clear();
//	while (pWarn != NULL)
//	{
//		warningNameVec.push_back(pWarn->chName);
//		warningValueVec.push_back(pWarn->chValue);
//		pWarn = pWarn->pNext;
//	}
//}

int SCDValidator::getErrorSize()
{
	return errorSize;
}

int SCDValidator::getWarningSize()
{
	return warningSize;
}

QString SCDValidator::getVersion()
{
	return m_version;
}

QString SCDValidator::getReversion()
{
	return m_reVersion;
}

QString SCDValidator::getName()
{
	return m_Name;
}

QVector<QString> SCDValidator::getErrorNameVec()
{
	return errorNameVec;
}

QVector<QString> SCDValidator::getErrorValueVec()
{
	return errorValueVec;
}

QVector<QString> SCDValidator::getWarningNameVec()
{
	return warningNameVec;
}

QVector<QString> SCDValidator::getWarningValueVec()
{
	return warningValueVec;
}

void SCDValidator::SCDValidate(QString path)
{
	/*PSCDVALIDINFOSTRUA pScdValidInfoStrua;
	
	bool isSuc = SAXSCDValidateA(path.toLocal8Bit().constData(), &pScdValidInfoStrua);

	if (!isSuc)
	{
		return;
	}
	
	m_Name = pScdValidInfoStrua->chName;
	m_version = pScdValidInfoStrua->chVersion;
	m_reVersion = pScdValidInfoStrua->chRevision;
	
	errorSize = pScdValidInfoStrua->iErroSize;
	warningSize = pScdValidInfoStrua->iWarnSize;

	loadErrorVec(pScdValidInfoStrua->pErro);
	loadWarningVec(pScdValidInfoStrua->pWarn);

    SAXFreeA(pScdValidInfoStrua);*/
}