#ifndef GraphicSvrAdapterInterface_h
#define GraphicSvrAdapterInterface_h

#include <fstream>
#include <QByteArray>

using namespace std;

//�ӿ�����GraphicSvrAdapterInterface
//���ܣ�Ӧ�ò��ͼ�η�����ʵĳ���
//�ص㣺������

class GraphicSvrAdapterInterface
{
public:
	virtual bool   GetSignalIEDGraphic(QString IEDKey,QByteArray& fileData) = 0;
	virtual bool   GetTwoIEDGraphic(QString IEDKeyPri,QString IEDKeySec,QByteArray& fileData) = 0;
	virtual bool   GetIEDIndexGraphic(QString IEDKey,QByteArray& fileData) = 0;	

	//niuziming add
	virtual bool   GetSubnetGraphic(QString subNetKey,QByteArray& fileData) = 0;
	virtual bool   GetMsgFlowGraphic(QString IEDKey,QByteArray& fileData) = 0;
	virtual bool   GetTerminalGraphic(QString IEDKey,QByteArray& fileData) = 0;
	virtual bool   GetVirtualLoopGraphic(QString IEDKeyPri,QString IEDKeySec,int sigType,QByteArray& fileData) = 0;

	virtual bool   GetMsgFlowDiffGraphic(QString IEDKey, QString otherIEDKey, QByteArray& fileData) = 0;
	virtual bool   GetVirtualLoopDiffGraphic(QString IEDKeyPri,QString IEDKeySec,int sigType,QByteArray& fileData) = 0;
	virtual bool   GetTerminalDiffGraphic(QString IEDKey, QString otherIEDKey, QByteArray& fileData) = 0;
};

#endif
