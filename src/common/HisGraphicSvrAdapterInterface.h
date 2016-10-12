#ifndef HisGraphicSvrAdapterInterface_h
#define HisGraphicSvrAdapterInterface_h

#include <qstring>
#include <qvector>
#include <fstream>
#include "TicketSvrInterface.h"
#include "SecurityMeasuresTicket.h"

using namespace TicketSvr;
using namespace std;

//�ӿ�����HisGraphicSvrAdapterInterface
//���ܣ�Ӧ�ò����ʷͼ�η���ĳ���
//�ص㣺������

class HisGraphicSvrAdapterInterface
{
public:
	virtual bool   GetGraphicData(QString graphicName, QString SCDVersion,QByteArray& fileData) = 0;//���ͼ���ļ���
	virtual bool   AddGraphic(QString graphicName, QString SCDVersion,  QString graphicType, QByteArray& fileData) = 0;//����ͼ��
	virtual bool   DeleteGraphic(QString graphicName, QString SCDVersion) = 0; //ɾ��ͼ��
	virtual bool   IsGraphicExist(QString graphicName, QString SCDVersion) = 0;//ͼ���Ƿ����
	virtual bool   ModifyGraphic(QString graphicName, QString SCDVersion, QByteArray& fileData) = 0;//�޸�ͼ��
};

#endif
