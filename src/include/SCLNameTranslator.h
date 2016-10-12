#ifndef SCLNameTranslator_H
#define SCLNameTranslator_H

#include "Spliter.h"
#include <QDebug>
#include <QStringList>

class SCLNameTranslator
{
public:
	::std::string GetParentKey(const ::std::string& childKey) //�����ӽڵ�Key��ø��ڵ�Key
	{
		std::vector<string> keyList = Split(childKey,".");
		int strNum = keyList.size();
		if(strNum>4)
		{
			string parentKey = keyList[0] + "." + keyList[1] + "." + keyList[2] + "." + keyList[3];
			return parentKey;
		}
		else
		{
			size_t pos = childKey.rfind('.');
			string parentKey = childKey.substr(0,pos);
			return parentKey;
		}
	}
	
	::std::string GetParentName(const ::std::string& childKey)//�����ӽڵ�Key��ø��ڵ��Name
	{
		std::vector<string> keyList = Split(childKey,".");
		int strNum = keyList.size();
		if(strNum>4)
		{
			return keyList[3];
		}
		else
		{
			size_t pos1 = childKey.rfind('.');
			string parentKey = childKey.substr(0,pos1);
			size_t pos2 = parentKey.rfind('.');
			string parentName = parentKey.substr(pos2+1);
			return parentName;
		}
	
	}
	
	::std::string GetNameFromKey(const ::std::string& key)    //����Key�������
	{
		std::vector<string> keyList = Split(key,".");
		int strNum = keyList.size();
		if(strNum>4)
		{
			string name = "";
			for(int i=4;i<strNum;i++)
			{
				name = name + keyList[i];
				if(i!=strNum-1)
				{
					name = name + ".";
				}
			}
			return name;
		}
		else
		{
			size_t pos1 = key.rfind('.');
			string name = key.substr(pos1+1);
			return name;
		}
	}
	
	::std::string GetChildKeyFromParentKeyAndChildName(const ::std::string& parentKey, const ::std::string& childName)//���ݸ��ڵ��Key���ӽڵ�Name����ӽڵ��Key 
	{
		return parentKey + "." + childName;
	}

	// �˺������ڲ��컯�Ƚϣ�����ʵʱ���Key��ȡ�ò��컯�Ƚ�ʱ���õ�Key
	// ȡ�õ�Key����������
	::std::string GetKeyNameWithoutSubStation(const ::std::string& Key) 
	{
		std::vector<string> keyList = Split(Key,".");
		int strNum = keyList.size();
		if (strNum > 0)
		{
			string name = "";
			for (int i = 2; i < strNum; i++)
			{
				name = name + keyList[i];
				if(i != strNum-1)
				{
					name = name + ".";
				}
			}
			return name;
		}
		return "";
	}

	// �˺������ڲ��컯�Ƚϣ�����SubStationKeyȡ���ļ���ʱ��
	// �磺��220kV.LanXi#2014-05-15#11:53:33��ȡ�õĽ��Ϊ��2014-05-15 11:53:33��
	::std::string GetFileTimeFromKey(const ::std::string &Key)
	{
		string fileTime = "";
		std::vector<string> keyList = Split(Key, ".");
		int strNum = keyList.size();
		if (strNum >= 2)
		{
			if (keyList[1].find('#') != string::npos)
			{
				std::vector<string> timeList = Split(keyList[1], "#");
				if(timeList.size() > 2)
				{
					fileTime = timeList[1]+ " " + timeList[2];
				}
			}
		}
		return fileTime;
	}

	// �˺������ڲ��컯�Ƚϣ�����SubStationKeyȡ���ļ�������
	// �磺��220kV.LanXi#2014-05-15#11:53:33��ȡ�õĽ��Ϊ��LanXi��
	::std::string GetFileNameFromKey(const ::std::string &Key)
	{
		std::vector<string> keyList = Split(Key, ".");
		int strNum = keyList.size();
		if (strNum >= 2)
		{
			string fileName = "";
			std::vector<string> timeList = Split(keyList[1], "#");
			fileName = timeList[0];
			return fileName;
		}
		return "";
	}

	// �˺������ڲ��컯�Ƚϣ��ӵ�ǰKeyȡ��IED��Key
	::std::string GetIEDKeyFromSCDDiffResult(const ::std::string& childKey)
	{
		std::vector<string> keyList = Split(childKey,".");
		int strNum = keyList.size();
		if(strNum>1)
		{
			return keyList[0] + "." + keyList[1];
		}
		return "";
	}

	::std::string GetIEDNameFromSCDDiffResult(const ::std::string& childKey)
	{
		std::vector<string> keyList = Split(childKey,".");
		int strNum = keyList.size();
		if(strNum>1)
		{
			return keyList[1];
		}
		return "";
	}

	// �˺������ڲ��컯�Ƚϣ��ӵ�ǰKeyȡ�ö�������
	::std::string GetObjNameFromSCDDiffResult(const ::std::string& Key)
	{
		std::vector<string> keyList = Split(Key,".");
		int strNum = keyList.size();
		if (strNum > 0)
		{
			string name = "";
			for (int i = 2; i < strNum; i++)
			{
				name = name + keyList[i];
				if(i != strNum-1)
				{
					name = name + ".";
				}
			}
			return name;
		}
		return "";
	}

	QString GetSignalPortQStringFromList(const QVector<QString> &PortIndexList)
	{
		QMap<QString, QString> portMap;
		for (uint i = 0; i < PortIndexList.size(); i++)
		{
			portMap.insert(PortIndexList[i], PortIndexList[i]);
		}

		QString PortString;
		QMap<QString, QString>::iterator it;
		for (it = portMap.begin(); it != portMap.end(); it++)
		{
			QString temp = it.value();
			if (PortString == "")
			{
				PortString += temp;
			}
			else
			{
				PortString = PortString + "/" + temp;
			}

		}
		return PortString;
	}

	QVector<QString> GetSignalPortListFromQString(const QString &PortQString)
	{
		QStringList list = PortQString.split("/");
		QVector<QString> PortList;
		for (uint i = 0; i < list.size(); i++)
		{
			PortList.push_back(list[i]);
		}
		return PortList;
	}

	//��Ӧ�ò��źŵ�Keyת����ģ�Ͳ���źŵ�ID
	QString GetSignalIntAddressFromSignalID(const QString signalID)
	{
		// Ϊ��ӦDO������.��������޸�ԭ�㷨--�����--2014.12.09
		QStringList IDList = signalID.split("$$");
		int strNum1 = IDList.size();

		QStringList nameList = IDList[0].split(".");
		int strNum2 = nameList.size();
		QString LDInst, LNName, DOIName,DAIName;

		QString temp;

		LDInst = nameList[4];

		if(strNum1>=3)
		{
			LNName = IDList[1];
			DOIName = IDList[2];
			temp = LDInst + "/" + LNName + "." + DOIName;
		}		
		
		if(IDList.size()>3)
		{
			DAIName = IDList[3];
			temp = LDInst + "/" + LNName + "." + DOIName + "." + DAIName;
		}

		return temp;
	}


};

#endif