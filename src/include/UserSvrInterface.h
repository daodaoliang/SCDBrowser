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
	virtual int  GetUserCount() = 0;//����û�����
	virtual int  GetRoleCount() = 0;//��ý�ɫ����

	virtual bool LoadUserInfo(vector<USER_INFO> *lstUserInfos) = 0;//װ���û���Ϣ
	virtual bool LoadRoleInfo(vector<string> *lstRoles) = 0;//װ�ؽ�ɫ��Ϣ
	virtual bool GetUserNameList(vector<string> *lstUserNames) = 0;//����û����б�

	virtual bool IsUserExist(string strUserName) = 0;//�û��Ƿ����

	virtual bool AddUser(string strUserName,string strPassword,string roleName) = 0;//�����û�

	virtual bool ModifyRole(string strUserName,string roleName) = 0;//�޸��û�Ȩ��

    virtual bool GetUserPassword(string strUserName,string *pPassword) = 0;//����û�����(����)
	virtual bool GetCiphertext(string strPwd,string *pStrCiphPwd) = 0;     //���������
	virtual bool ModifyPassword(string strUserName,string strNewPassword) = 0;//�޸Ŀ���

	virtual bool DeleteUser(string strUserName) = 0; //ɾ���û�
	virtual bool DeleteUserWithRole(string strRoleName) = 0; //ɾ��ĳ��ɫ�������û�

	virtual string GetUserRole(string strUserName) = 0;//����û��Ľ�ɫ
};

#endif