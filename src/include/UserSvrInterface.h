#ifndef USERSVR_INTERFACE_H
#define USERSVR_INTERFACE_H


#include <string>
#include <vector>

using namespace std;

class USER_INFO
{
public:
	string userName;
	string password;
	string roleName;
};

class UserSvrInterface
{
public:
	virtual int  GetUserCount() = 0;//获得用户个数
	virtual int  GetRoleCount() = 0;//获得角色个数

	virtual bool LoadUserInfo(vector<USER_INFO> *lstUserInfos) = 0;//装载用户信息
	virtual bool LoadRoleInfo(vector<string> *lstRoles) = 0;//装载角色信息
	virtual bool GetUserNameList(vector<string> *lstUserNames) = 0;//获得用户名列表

	virtual bool IsUserExist(string strUserName) = 0;//用户是否存在

	virtual bool AddUser(string strUserName,string strPassword,string roleName) = 0;//增加用户

	virtual bool ModifyRole(string strUserName,string roleName) = 0;//修改用户权限

    virtual bool GetUserPassword(string strUserName,string *pPassword) = 0;//获得用户口令(密文)
	virtual bool GetCiphertext(string strPwd,string *pStrCiphPwd) = 0;     //将口令加密
	virtual bool ModifyPassword(string strUserName,string strNewPassword) = 0;//修改口令

	virtual bool DeleteUser(string strUserName) = 0; //删除用户
	virtual bool DeleteUserWithRole(string strRoleName) = 0; //删除某角色的所有用户

	virtual string GetUserRole(string strUserName) = 0;//获得用户的角色
};

#endif