#include "GraphicSvr.h"
#include <string>
#include <vector>
#include "StreamTransformer.h"
#include "HmiGraphicsScene.h"
#include <QMessageBox>

bool GraphicSvr::GetSignalIEDGraphic(const ::std::string& IEDKey, ::SCLModel::ByteSeq& fileData, const ::Ice::Current&)
{
	/*fstream graphicXml;
	if(!GetSignalIEDGraphic(IEDKey,graphicXml))
	{
	graphicXml.close();
	return false;
	}
	StreamTransformer transform;
	transform.FromStreamToByteSeq(graphicXml,fileData);
	graphicXml.close();*/

	QPrinter *printer1 = new QPrinter;
	QSizeF size1 = printer1->paperSize(QPrinter::DevicePixel);
	HmiGraphicsScene* mainScene = new HmiGraphicsScene(QRectF(0, 0, size1.width(), size1.height()),NULL);

	//mainScene->DrawSelfProc(IEDKey.c_str());
	/*QFile file("C:\\grpaph1.pic");
	if (!file.open(QIODevice::WriteOnly)) {
	return false;
	} 
	QDataStream stream(&file);
	mainScene->save(stream);
	file.close();*/
	//QBuffer buf;    
	//buf.open(QIODevice::ReadWrite);    
	//QDataStream dbSrteam(&buf);    
	//mainScene->save(dbSrteam);
	//for(int i=0;i<buf.buffer().size();i++)
	//{
	//	fileData.push_back(buf.buffer()[i]);
	//}

	return true;
}

bool GraphicSvr::GetTwoIEDGraphic(const ::std::string& IEDKeyPri, const ::std::string& IEDKeySec, ::SCLModel::ByteSeq& fileData, const ::Ice::Current&)
{
	/*fstream graphicXml;
	if(!GetTwoIEDGraphic(IEDKeyPri,IEDKeySec,graphicXml))
	{
	graphicXml.close();
	return false;
	}
	StreamTransformer transform;
	transform.FromStreamToByteSeq(graphicXml,fileData);
	graphicXml.close();*/
	QPrinter *printer1 = new QPrinter;
	QSizeF size1 = printer1->paperSize(QPrinter::DevicePixel);
	HmiGraphicsScene* mainScene = new HmiGraphicsScene(NULL,QRectF(0, 0, size1.width(), size1.height()),NULL);
	mainScene->DrawTwoProc(IEDKeyPri.c_str(),IEDKeySec.c_str());
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

bool GraphicSvr::GetIEDIndexGraphic(const ::std::string& IEDKey, ::SCLModel::ByteSeq& fileData, const ::Ice::Current&)
{
	return true;
}

bool GraphicSvr::GetSubnetGraphic(const ::std::string& subNetKey, ::SCLModel::ByteSeq& fileData, const ::Ice::Current& )
{
	QPrinter *printer1 = new QPrinter;
	QSizeF size1 = printer1->paperSize(QPrinter::DevicePixel);
	HmiGraphicsScene* mainScene = new HmiGraphicsScene(NULL,QRectF(0, 0, size1.width(), size1.height()),NULL);
	mainScene->DrawSubnetProc(subNetKey.c_str());
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

bool GraphicSvr::GetMsgFlowGraphic(const ::std::string& IEDKey, ::SCLModel::ByteSeq& fileData, const ::Ice::Current& )
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

bool GraphicSvr::GetVirtualLoopGraphic(const ::std::string& IEDKeyPri, const ::std::string& IEDKeySec, ::SCLModel::ByteSeq& fileData, const ::Ice::Current& )
{
	QPrinter *printer1 = new QPrinter;
	QSizeF size1 = printer1->paperSize(QPrinter::DevicePixel);
	HmiGraphicsScene* mainScene = new HmiGraphicsScene(NULL,QRectF(0, 0, size1.width(), size1.height()),NULL);
	mainScene->DrawVirtualLoopProc(IEDKeyPri.c_str(),IEDKeySec.c_str(),QSignalTypeEnum::typeAll_);
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

bool GraphicSvr::GetTerminalGraphic(const ::std::string& IEDKey, ::SCLModel::ByteSeq& fileData, const ::Ice::Current& )
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

GraphicSvr::GraphicSvr(void)
{
	;
}

GraphicSvr::~GraphicSvr(void)
{
	;
}

bool GraphicSvr::GetGraphicByGraphicName(const ::std::string&, ::SCLModel::ByteSeq&, const ::Ice::Current&)
{
	return false;
}

bool GraphicSvr::GetGraphicNamesOfMaintenanceIEDs(const ::SCLModel::NameSeq&, ::SCLModel::GraphicInfoSeq&, const ::Ice::Current&)
{
	return false;
}

bool GraphicSvr::GetGraphicNamesOfIED(const ::std::string&, ::SCLModel::GraphicInfoSeq&, const ::Ice::Current&)
{
	return false;
}

