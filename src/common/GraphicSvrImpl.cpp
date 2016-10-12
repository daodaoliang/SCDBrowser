#include "GraphicSvrImpl.h"
#include <string>
#include <vector>
#include "StreamTransformer.h"
#include "HmiGraphicsScene.h"
#include <QMessageBox>
// #include "LogRecorder.h"
#include "SCDDifferCompare.h"

bool GraphicSvrImpl::GetSignalIEDGraphic(const ::std::string& IEDKey, std::vector<unsigned char>& fileData)
{
	QPrinter *printer1 = new QPrinter;
	QSizeF size1 = printer1->paperSize(QPrinter::DevicePixel);
	HmiGraphicsScene* mainScene = new HmiGraphicsScene(QRectF(0, 0, size1.width(), size1.height()),NULL, NULL);
	// mainScene->DrawSelfProc(IEDKey.c_str());
	QBuffer buf;    
	buf.open(QIODevice::ReadWrite);    
	QDataStream dbSrteam(&buf);    
	mainScene->save(dbSrteam);
	for(int i=0;i<buf.buffer().size();i++)
	{
		fileData.push_back(buf.buffer()[i]);
	}

	return true;
}

bool GraphicSvrImpl::GetTwoIEDGraphic(const ::std::string& IEDKeyPri, const ::std::string& IEDKeySec, std::vector<unsigned char>& fileData)
{
	QPrinter *printer1 = new QPrinter;
	QSizeF size1 = printer1->paperSize(QPrinter::DevicePixel);
	HmiGraphicsScene* mainScene = new HmiGraphicsScene(NULL,QRectF(0, 0, size1.width(), size1.height()),NULL);
	//mainScene->DrawTwoProc(IEDKeyPri.c_str(),IEDKeySec.c_str());
	mainScene->DrawVirtualLoopProc(IEDKeyPri.c_str(),IEDKeySec.c_str(),typeAll_);
	QBuffer buf;    
	buf.open(QIODevice::ReadWrite);    
	QDataStream dbSrteam(&buf);    
	mainScene->save(dbSrteam);
	for(int i=0;i<buf.buffer().size();i++)
	{
		fileData.push_back(buf.buffer()[i]);
	}

	return true;
}

bool GraphicSvrImpl::GetIEDIndexGraphic(const ::std::string& IEDKey, std::vector<unsigned char>& fileData)
{
	QPrinter *printer1 = new QPrinter;
	QSizeF size1 = printer1->paperSize(QPrinter::DevicePixel);
	HmiGraphicsScene* mainScene = new HmiGraphicsScene(NULL,QRectF(0, 0, size1.width(), size1.height()),NULL);
	// mainScene->DrawAllProc(IEDKey.c_str());
	QBuffer buf;    
	buf.open(QIODevice::ReadWrite);    
	QDataStream dbSrteam(&buf);    
	mainScene->save(dbSrteam);

	for(int i=0;i<buf.buffer().size();i++)
	{
		fileData.push_back(buf.buffer()[i]);
	}

	return true;
}

GraphicSvrImpl::GraphicSvrImpl()
{
	;
}

GraphicSvrImpl::~GraphicSvrImpl()
{
	;
}

//niuziming add 2013-06-14
bool GraphicSvrImpl::GetMsgFlowGraphic(const ::std::string &IEDKey, std::vector<unsigned char>& fileData)
{
	QPrinter *printer1 = new QPrinter;
	QSizeF size1 = printer1->paperSize(QPrinter::DevicePixel);
	HmiGraphicsScene* mainScene = new HmiGraphicsScene(NULL,QRectF(0, 0, size1.width(), size1.height()),NULL);
	mainScene->DrawMsgFlowProc(IEDKey.c_str());
	QBuffer buf;    
	buf.open(QIODevice::ReadWrite);    
	QDataStream dbSrteam(&buf);    
	mainScene->save(dbSrteam);

	for(int i=0;i<buf.buffer().size();i++)
	{
		fileData.push_back(buf.buffer()[i]);
	}
	return true;
}

bool GraphicSvrImpl::GetSubnetGraphic(const ::std::string &subnetKey, std::vector<unsigned char>& fileData)
{
	QPrinter *printer1 = new QPrinter;
	QSizeF size1 = printer1->paperSize(QPrinter::DevicePixel);
	HmiGraphicsScene* mainScene = new HmiGraphicsScene(NULL,QRectF(0, 0, size1.width(), size1.height()),NULL);
	mainScene->DrawSubnetProc(subnetKey.c_str());
	QBuffer buf;    
	buf.open(QIODevice::ReadWrite);    
	QDataStream dbSrteam(&buf);    
	mainScene->save(dbSrteam);

	for(int i=0;i<buf.buffer().size();i++)
	{
		fileData.push_back(buf.buffer()[i]);
	}
	return true;
}

bool GraphicSvrImpl::GetTerminalGraphic(const ::std::string &IEDKey, std::vector<unsigned char>& fileData)
{
	QPrinter *printer1 = new QPrinter;
	QSizeF size1 = printer1->paperSize(QPrinter::DevicePixel);
	HmiGraphicsScene* mainScene = new HmiGraphicsScene(NULL,QRectF(0, 0, size1.width(), size1.height()),NULL);
	mainScene->DrawTerminalProc(IEDKey.c_str());
	QBuffer buf;    
	buf.open(QIODevice::ReadWrite);    
	QDataStream dbSrteam(&buf);    
	mainScene->save(dbSrteam);

	for(int i=0;i<buf.buffer().size();i++)
	{
		fileData.push_back(buf.buffer()[i]);
	}
	return true;
}

bool GraphicSvrImpl::GetVirtualLoopGraphic(const ::std::string& IEDKeyPri, const ::std::string &IEDKeySec, int sigType, std::vector<unsigned char>& fileData)
{
	QPrinter *printer1 = new QPrinter;
	QSizeF size1 = printer1->paperSize(QPrinter::DevicePixel);
	HmiGraphicsScene* mainScene = new HmiGraphicsScene(NULL,QRectF(0, 0, size1.width(), size1.height()),NULL);
	switch (sigType)
	{
	case 1:
		mainScene->DrawVirtualLoopProc(IEDKeyPri.c_str(), IEDKeySec.c_str(),typeGoose_);
		break;
	case 2:
		mainScene->DrawVirtualLoopProc(IEDKeyPri.c_str(), IEDKeySec.c_str(),typeSV_);
		break;
	default:
		mainScene->DrawVirtualLoopProc(IEDKeyPri.c_str(), IEDKeySec.c_str(),typeAll_);
		break;
	}
	QBuffer buf;
	buf.open(QIODevice::ReadWrite);
	QDataStream dbSrteam(&buf);
	mainScene->save(dbSrteam);

	for(int i=0;i<buf.buffer().size();i++)
	{
		fileData.push_back(buf.buffer()[i]);
	}
	return true;
}

bool GraphicSvrImpl::GetMsgFlowDiffGraphic(const ::std::string &IEDKey, const ::std::string &otherIEDKey, std::vector<unsigned char>& fileData)
{
	QPrinter *printer1 = new QPrinter;
	QSizeF size1 = printer1->paperSize(QPrinter::DevicePixel);
	SCDDiffCompare *SCDDiffer = SCDDiffCompare::GetInstance();
	HmiGraphicsScene* mainScene = new HmiGraphicsScene(QRectF(0, 0, size1.width(), size1.height()),NULL, SCDDiffer, true);
	
	mainScene->DrawMsgFlowDiffProc(IEDKey.c_str(), otherIEDKey.c_str());
	QBuffer buf;    
	buf.open(QIODevice::ReadWrite);    
	QDataStream dbSrteam(&buf);    
	mainScene->save(dbSrteam);

	for(int i=0;i<buf.buffer().size();i++)
	{
		fileData.push_back(buf.buffer()[i]);
	}
	return true;
}

bool GraphicSvrImpl::GetVirtualLoopDiffGraphic(const ::std::string& IEDKeyPri, const ::std::string &IEDKeySec, int sigType, std::vector<unsigned char>& fileData)
{
	QPrinter *printer1 = new QPrinter;
	QSizeF size1 = printer1->paperSize(QPrinter::DevicePixel);
	SCDDiffCompare *SCDDiffer = SCDDiffCompare::GetInstance();
	HmiGraphicsScene* mainScene = new HmiGraphicsScene(QRectF(0, 0, size1.width(), size1.height()),NULL, SCDDiffer, true);
	//HmiGraphicsScene* mainScene = new HmiGraphicsScene(NULL,QRectF(0, 0, size1.width(), size1.height()),NULL, true);
	switch (sigType)
	{
	case 1:
		mainScene->DrawVirtualLoopDiffProc(IEDKeyPri.c_str(), IEDKeySec.c_str(),typeGoose_);
		break;
	case 2:
		mainScene->DrawVirtualLoopDiffProc(IEDKeyPri.c_str(), IEDKeySec.c_str(),typeSV_);
		break;
	default:
		mainScene->DrawVirtualLoopDiffProc(IEDKeyPri.c_str(), IEDKeySec.c_str(),typeAll_);
		break;
	}
	QBuffer buf;    
	buf.open(QIODevice::ReadWrite);    
	QDataStream dbSrteam(&buf);    
	mainScene->save(dbSrteam);

	for(int i=0;i<buf.buffer().size();i++)
	{
		fileData.push_back(buf.buffer()[i]);
	}
	return true;
}

bool GraphicSvrImpl::GetTerminalDiffGraphic(const ::std::string& IEDKey, const ::std::string &otherIEDKey, std::vector<unsigned char>& fileData)
{
	QPrinter *printer1 = new QPrinter;
	QSizeF size1 = printer1->paperSize(QPrinter::DevicePixel);
	SCDDiffCompare *SCDDiffer = SCDDiffCompare::GetInstance();
	HmiGraphicsScene* mainScene = new HmiGraphicsScene(QRectF(0, 0, size1.width(), size1.height()),NULL, SCDDiffer, true);
	//HmiGraphicsScene* mainScene = new HmiGraphicsScene(NULL,QRectF(0, 0, size1.width(), size1.height()),NULL, true);
	mainScene->DrawTerminalDiffProc(IEDKey.c_str(), otherIEDKey.c_str());
	QBuffer buf;    
	buf.open(QIODevice::ReadWrite);    
	QDataStream dbSrteam(&buf);    
	mainScene->save(dbSrteam);

	for(int i=0;i<buf.buffer().size();i++)
	{
		fileData.push_back(buf.buffer()[i]);
	}
	return true;
}