#pragma once

//#include <WinBase.h>

class CTrivalVersionCheck
{
public:
	CTrivalVersionCheck(void);
	~CTrivalVersionCheck(void);

public:
	void SetConfigInfo();
	bool CheckDate();
	bool AddDate();
	int MakeHostID(char *hostid);
	void GetLastHostID();
	int CheckHostID();

	int times;

	int iHostID;


	//int GetHostId(CHAR *p);
	//int CheckHostId(CHAR *hostid);
};

