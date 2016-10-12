#include "TrivalVersionCheck.h"
#include <QSettings>
#include <QFile>
#include <QDate>
#include <QDataStream>
#include <QtGlobal>

typedef char CHAR;
extern int GetHostId(CHAR *p);
extern int CheckTriVerHostID(int hostID);

CTrivalVersionCheck::CTrivalVersionCheck(void)
{
	SetConfigInfo();
}

CTrivalVersionCheck::~CTrivalVersionCheck(void)
{

}

void CTrivalVersionCheck::SetConfigInfo()
{
	if (!QFile::exists("C:\\TrivalVersionInFo"))
	{
		QFile file("C:\\TrivalVersionInFo");
		if (file.open(QIODevice::OpenModeFlag::ReadWrite))
		{
			QDataStream s(&file);
			s<< (qint32)1;
			QDate StartDate = QDate::currentDate();
			s<< StartDate;
			QDate EndDate = QDate::currentDate().addMonths(6);
			s<< EndDate;
			file.close();
		}
	}	
}

bool CTrivalVersionCheck::CheckDate()
{
	if (!QFile::exists("C:\\TrivalVersionInFo"))
	{
		SetConfigInfo();
	}

	//QSettings *config = new QSettings("C:\\TrivalVersionInFo", QSettings::IniFormat);
	////uint iRegisterTime = config->value("config/RegisterTime").toUInt();
	////QDate dStartDate = config->value("config/StartDate").toDate();
	//QDate EndDate = config->value("config/EndDate").toDate();

	//QDate CurrentDate = QDate::currentDate();

	//if (CurrentDate < EndDate)
	//{
	//	isInDate = true;
	//	return isInDate;
	//}
	//return isInDate;
	QFile file("C:\\TrivalVersionInFo");
	if (file.open(QIODevice::OpenModeFlag::ReadWrite))
	{
		QDataStream s(&file);
		qint32 i;
		QDate StartDate, EndDate;
		s>>i>>StartDate>>EndDate;

		times = i;

		QString start = StartDate.toString("dd.MM.yyyy");
		QString end = EndDate.toString("dd.MM.yyyy");

		QDate CurrentDate = QDate::currentDate();
		QString current = CurrentDate.toString("dd.MM.yyyy");
		if (CurrentDate < EndDate)
		{
			return true;
		}
		return false;
	}
	return false;
}

bool CTrivalVersionCheck::AddDate()
{
	if (QFile::exists("C:\\TrivalVersionInFo"))
	{
		QFile file("C:\\TrivalVersionInFo");
		if (file.open(QIODevice::OpenModeFlag::ReadWrite))
		{
			QDataStream s(&file);
			times++;
			s<< (qint32)(times);
			QDate StartDate = QDate::currentDate();
			QString start = StartDate.toString("dd.MM.yyyy");
			s<< StartDate;
			QDate EndDate = QDate::currentDate().addMonths(3);
			QString end = EndDate.toString("dd.MM.yyyy");
			s<< EndDate;
			file.close();
			return true;
		}
		return true;
	}	
	return false;
}

int CTrivalVersionCheck::MakeHostID(char *hostid)
{
	char	buffer[16];
	int		kkk;

	buffer[0] = '0' ;
	buffer[1] = 'x' ;

	if (hostid[4] == '-')
	{
		buffer[2] = hostid[0] ;
		buffer[3] = hostid[5] ;
		buffer[4] = hostid[1] ;
		buffer[5] = hostid[6] ;

		buffer[6] = hostid[2] ;
		buffer[7] = hostid[7] ;
		buffer[8] = hostid[3] ;
		buffer[9] = hostid[8] ;

	}else
	{
		buffer[2] = hostid[0] ;
		buffer[3] = hostid[4] ;
		buffer[4] = hostid[1] ;
		buffer[5] = hostid[5] ;

		buffer[6] = hostid[2] ;
		buffer[7] = hostid[6] ;
		buffer[8] = hostid[3] ;
		buffer[9] = hostid[7] ;
	}

	buffer[10] = 0;
	sscanf(buffer,"%x",&kkk);

	if(kkk > 0xf9000000)  kkk = kkk - 0x10000000 ;
	kkk = kkk + 0x6135322;

	return kkk;
}

void CTrivalVersionCheck::GetLastHostID()
{
	char hostid[32];
	int retVal = GetHostId(hostid);
	for (int i = 0; i < times; i++)
	{
		iHostID = MakeHostID(hostid);
		sprintf(hostid, "%X", iHostID);
	}
}

int CTrivalVersionCheck::CheckHostID()
{
	GetLastHostID();
	return CheckTriVerHostID(iHostID);

}

//int CTrivalVersionCheck::GetHostId(CHAR *p)
//{
//	CHAR str[100];
//
//	if (WinExec((LPCSTR)"cmd.exe /c dir c:\\*.* > c:\\temp.wz",SW_MINIMIZE) < 32)
//		return 0;
//
//	string szChar =""; 
//	szChar = setlocale( LC_CTYPE, ("chs")); 
//	FILE *f = fopen("c:\\temp.wz","r");
//	if (f == NULL)   return 0;
//
//	fgets(str,100,f);
//	fgets(str,100,f);
//
//	strcpy(p,str + strlen(str)-10);
//	p[strlen(p) -1 ] = 0;
//	fclose(f);
//
//	f = fopen("c:\\temp.wz","w+");
//	if(f == NULL)  return 0;
//	fclose(f);
//	return 1;
//}          
//
//int	CTrivalVersionCheck::CheckHostId(CHAR *hostid)
//{
//
//	FILE    *fp;
//	CHAR	buffer[16];
//	int		retval;
//	int		key[2],kkk;
//
//	WCHAR filename[MAX_PATH];
//
//	WCHAR path[_MAX_PATH];
//	GetModuleFileName( GetModuleHandle(L"UIBasic.dll"), path, _MAX_PATH );
//	wstring sModulePath(path);
//
//	wstring sIniPath(sModulePath.begin(),sModulePath.begin()+sModulePath.rfind('\\'));
//
//
//	wsprintf( filename, L"%s\\Support\\hostid.sys", sIniPath.c_str());
//	string szChar =""; 
//	szChar = setlocale( LC_CTYPE, ("chs")); 
//	fp = _wfopen(filename, L"rb") ;
//	if(fp == NULL)  return 0;
//
//
//	buffer[0] = '0' ;
//	buffer[1] = 'x' ;
//
//	buffer[2] = hostid[0] ;
//	buffer[3] = hostid[5] ;
//	buffer[4] = hostid[1] ;
//	buffer[5] = hostid[6] ;
//
//	buffer[6] = hostid[2] ;
//	buffer[7] = hostid[7] ;
//	buffer[8] = hostid[3] ;
//	buffer[9] = hostid[8] ;
//
//	buffer[10] = 0;
//	sscanf(buffer,"%x",&kkk);
//
//	if(kkk > 0xf9000000)  kkk = kkk - 0x10000000 ;
//	kkk = kkk + 0x6135322;
//
//	while(!feof(fp))
//	{
//		retval = fread(key,1,2*sizeof(int),fp);
//		if(retval <= 0)  break;
//
//		if(kkk != key[0])  continue; 
//
//		fclose(fp);
//		return 1;
//	}
//	fclose(fp);
//	return  0;
//}

