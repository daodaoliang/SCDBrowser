#ifndef ROLESVR_INTERFACE_H
#define ROLESVR_INTERFACE_H

#include "UserRoleDefine.h"

#include <string>
#include <vector>

using namespace std;

class ROLE_INFO
{
public:
	string roleName;
	bool   privileges[MAX_PRIVILEGES_NUM];
};

class RoleSvrInterface
{
public:
	virtual int  GetRoleCount() = 0;//��ý�ɫ����

	virtual bool LoadRoleInfo(vector<ROLE_INFO> *lstRoles) = 0;//װ�ؽ�ɫ��Ϣ

	virtual bool IsRoleExist(string strRoleName) = 0;//��ɫ�Ƿ����

	virtual bool AddRole(string strRoleName) = 0;//���ӽ�ɫ
	virtual bool ModifyRolePrivileges(string strRoleName,int nPrivNo,bool bUseFlag) = 0;//�޸Ľ�ɫȨ��
	virtual bool DeleteRole(string strRoleName) = 0; //ɾ����ɫ

	virtual bool LoadPrivileges(vector<string> *lstPrivilegesLists) = 0;//װ��Ȩ���б�
	virtual bool IsPrivilegesValid(string roleName, int priviNo) = 0;//��ɫ��Ȩ���Ƿ���Ч

};

#endif