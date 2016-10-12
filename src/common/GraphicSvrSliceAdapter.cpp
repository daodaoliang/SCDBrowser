#include "GraphicSvrSliceAdapter.h"
#include "StreamTransformer.h"
#include <QIcon>

GraphicSvrSliceAdapter *GraphicSvrSliceAdapter::m_pInstance = NULL;

bool GraphicSvrSliceAdapter::GetSignalIEDGraphic(QString IEDKey,QByteArray& fileData)
{
	if(graphicSvrAdapter!=NULL)
	{
		::SCLModel::ByteSeq tempByteSeq;
		if(graphicSvrAdapter->GetSignalIEDGraphic(IEDKey.toLocal8Bit().constData(),tempByteSeq))
		{
			fileData.clear();
			for(int i=0;i<tempByteSeq.size();i++)
			{
				fileData.push_back(tempByteSeq[i]);
			}
			return true;
		}
	}
	return false;
}

bool GraphicSvrSliceAdapter::GetTwoIEDGraphic(QString IEDKeyPri,QString IEDKeySec,QByteArray& fileData)
{
	if(graphicSvrAdapter!=NULL)
	{
		::SCLModel::ByteSeq tempByteSeq;
		if(graphicSvrAdapter->GetTwoIEDGraphic(IEDKeyPri.toLocal8Bit().constData(),IEDKeySec.toLocal8Bit().constData(),tempByteSeq))
		{
			fileData.clear();
			for(int i=0;i<tempByteSeq.size();i++)
			{
				fileData.push_back(tempByteSeq[i]);
			}
			return true;
		}
	}
	return false;
}

bool GraphicSvrSliceAdapter::GetIEDIndexGraphic(QString IEDKey,QByteArray& fileData)
{
	if(graphicSvrAdapter!=NULL)
	{
		::SCLModel::ByteSeq tempByteSeq;
		if(graphicSvrAdapter->GetIEDIndexGraphic(IEDKey.toLocal8Bit().constData(),tempByteSeq))
		{
			fileData.clear();
			for(int i=0;i<tempByteSeq.size();i++)
			{
				fileData.push_back(tempByteSeq[i]);
			}
			return true;
		}
	}
	return false;
}

//niuziming add
bool GraphicSvrSliceAdapter::GetSubnetGraphic(QString subNetKey,QByteArray& fileData)
{
	if(graphicSvrAdapter!=NULL)
	{
		::SCLModel::ByteSeq tempByteSeq;
		if (graphicSvrAdapter->GetSubnetGraphic(subNetKey.toLocal8Bit().constData(),tempByteSeq))
		{
			fileData.clear();
			for(int i=0;i<tempByteSeq.size();i++)
			{
				fileData.push_back(tempByteSeq[i]);
			}
			return true;
		}
	}
	return false;
}

bool GraphicSvrSliceAdapter::GetMsgFlowGraphic(QString IEDKey,QByteArray& fileData)
{
	if(graphicSvrAdapter!=NULL)
	{
		::SCLModel::ByteSeq tempByteSeq;
		if (graphicSvrAdapter->GetMsgFlowGraphic(IEDKey.toLocal8Bit().constData(),tempByteSeq))
		{
			fileData.clear();
			for(int i=0;i<tempByteSeq.size();i++)
			{
				fileData.push_back(tempByteSeq[i]);
			}
			return true;
		}
	}
	return false;
}

bool GraphicSvrSliceAdapter::GetVirtualLoopGraphic(QString IEDKeyPri,QString IEDKeySec,QByteArray& fileData)
{
	if(graphicSvrAdapter!=NULL)
	{
		::SCLModel::ByteSeq tempByteSeq;
		//if(graphicSvrAdapter->GetTwoIEDGraphic(IEDKeyPri.toLocal8Bit().constData(),IEDKeySec.toLocal8Bit().constData(),tempByteSeq))
		if(graphicSvrAdapter->GetVirtualLoopGraphic(IEDKeyPri.toLocal8Bit().constData(),IEDKeySec.toLocal8Bit().constData(),tempByteSeq))
		{
			fileData.clear();
			for(int i=0;i<tempByteSeq.size();i++)
			{
				fileData.push_back(tempByteSeq[i]);
			}
			return true;
		}
	}
	return false;
}

bool GraphicSvrSliceAdapter::GetTerminalGraphic(QString IEDKey,QByteArray& fileData)
{
	if(graphicSvrAdapter!=NULL)
	{
		::SCLModel::ByteSeq tempByteSeq;
		//if(graphicSvrAdapter->GetSignalIEDGraphic(IEDKey.toLocal8Bit().constData(),tempByteSeq))
		if (graphicSvrAdapter->GetTerminalGraphic(IEDKey.toLocal8Bit().constData(),tempByteSeq))
		{
			fileData.clear();
			for(int i=0;i<tempByteSeq.size();i++)
			{
				fileData.push_back(tempByteSeq[i]);
			}
			return true;
		}
	}
	return false;
}