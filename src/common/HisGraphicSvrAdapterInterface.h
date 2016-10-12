#ifndef HisGraphicSvrAdapterInterface_h
#define HisGraphicSvrAdapterInterface_h

#include <qstring>
#include <qvector>
#include <fstream>
#include "TicketSvrInterface.h"
#include "SecurityMeasuresTicket.h"

using namespace TicketSvr;
using namespace std;

//接口名：HisGraphicSvrAdapterInterface
//功能：应用层对历史图形服务的抽象
//特点：存虚类

class HisGraphicSvrAdapterInterface
{
public:
	virtual bool   GetGraphicData(QString graphicName, QString SCDVersion,QByteArray& fileData) = 0;//获得图形文件流
	virtual bool   AddGraphic(QString graphicName, QString SCDVersion,  QString graphicType, QByteArray& fileData) = 0;//增加图形
	virtual bool   DeleteGraphic(QString graphicName, QString SCDVersion) = 0; //删除图形
	virtual bool   IsGraphicExist(QString graphicName, QString SCDVersion) = 0;//图形是否存在
	virtual bool   ModifyGraphic(QString graphicName, QString SCDVersion, QByteArray& fileData) = 0;//修改图形
};

#endif
