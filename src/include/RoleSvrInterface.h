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
	virtual int  GetRoleCount() = 0;//获得角色个数

	virtual bool LoadRoleInfo(vector<ROLE_INFO> *lstRoles) = 0;//装载角色信息

	virtual bool IsRoleExist(string strRoleName) = 0;//角色是否存在

	virtual bool AddRole(string strRoleName) = 0;//增加角色
	virtual bool ModifyRolePrivileges(string strRoleName,int nPrivNo,bool bUseFlag) = 0;//修改角色权限
	virtual bool DeleteRole(string strRoleName) = 0; //删除角色

	virtual bool LoadPrivileges(vector<string> *lstPrivilegesLists) = 0;//装载权限列表
	virtual bool IsPrivilegesValid(string roleName, int priviNo) = 0;//角色的权限是否有效

};

#endif