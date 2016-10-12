#include "HisGraphicSvrSliceAdapter.h"
#include "StreamTransformer.h"
#include "TicketTranslator.h"

HisGraphicSvrSliceAdapter *HisGraphicSvrSliceAdapter::m_pInstance = NULL;

bool HisGraphicSvrSliceAdapter::AddGraphic(QString graphicName, QString SCDVersion,  QString graphicType,QByteArray& fileData)
{
	if(HisGraphicSvrAdapter!=NULL)
	{
		::SCLModel::ByteSeq buf;
		for(int i=0;i<fileData.size();i++)
		{
			buf.push_back(fileData[i]);
		}
		if(HisGraphicSvrAdapter->AddGraphic(graphicName.toLocal8Bit().constData(),SCDVersion.toLocal8Bit().constData(),graphicType.toLocal8Bit().constData(),buf))
		{
			for(int i=0;i<buf.size();i++)
			{
				fileData.push_back(buf[i]);
			}
			return true;
		}
	}
	return false;
}

bool HisGraphicSvrSliceAdapter::DeleteGraphic(QString graphicName, QString SCDVersion)
{
	if(HisGraphicSvrAdapter!=NULL)
	{
		if(HisGraphicSvrAdapter->DeleteGraphic(graphicName.toLocal8Bit().constData(),SCDVersion.toLocal8Bit().constData()))
		{
			return true;
		}
	}
	return false;
}

bool HisGraphicSvrSliceAdapter::GetGraphicData(QString graphicName, QString SCDVersion, QByteArray& fileData)
{
	::SCLModel::ByteSeq buf;
	if(HisGraphicSvrAdapter->GetGraphicData(graphicName.toLocal8Bit().constData(),SCDVersion.toLocal8Bit().constData(),buf))
	{
		fileData.clear();
		for(int i=0;i<buf.size();i++)
		{
			fileData.push_back(buf[i]);
		}
		return true;
	}
	return false;
}

bool HisGraphicSvrSliceAdapter::IsGraphicExist(QString graphicName, QString SCDVersion)
{
	if(HisGraphicSvrAdapter!=NULL)
	{
		if(HisGraphicSvrAdapter->IsGraphicExist(graphicName.toLocal8Bit().constData(),SCDVersion.toLocal8Bit().constData()))
		{
			return true;
		}
	}
	return false;
}

bool HisGraphicSvrSliceAdapter::ModifyGraphic(QString graphicName, QString SCDVersion, QByteArray& fileData)
{
	if(HisGraphicSvrAdapter!=NULL)
	{
		::SCLModel::ByteSeq buf;
		for(int i=0;i<fileData.size();i++)
		{
			buf.push_back(fileData[i]);
		}
		if(HisGraphicSvrAdapter->ModifyGraphic(graphicName.toLocal8Bit().constData(),SCDVersion.toLocal8Bit().constData(),buf))
		{
			for(int i=0;i<buf.size();i++)
			{
				fileData.push_back(buf[i]);
			}
			return true;
		}
	}
	return false;
}





