#include "GraphicSvrAdapter.h"
#include "StreamTransformer.h"
#include <QIcon>

GraphicSvrAdapter *GraphicSvrAdapter::m_pInstance = NULL;

bool GraphicSvrAdapter::GetSignalIEDGraphic(QString IEDKey,QByteArray& fileData)
{
	if(graphicSvr!=NULL)
	{
		std::vector<unsigned char> tempByteSeq;
		if(graphicSvr->GetSignalIEDGraphic(IEDKey.toLocal8Bit().constData(),tempByteSeq))
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

bool GraphicSvrAdapter::GetTwoIEDGraphic(QString IEDKeyPri,QString IEDKeySec,QByteArray& fileData)
{
	if(graphicSvr!=NULL)
	{
		std::vector<unsigned char> tempByteSeq;
		if(graphicSvr->GetTwoIEDGraphic(IEDKeyPri.toLocal8Bit().constData(),IEDKeySec.toLocal8Bit().constData(),tempByteSeq))
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

bool GraphicSvrAdapter::GetIEDIndexGraphic(QString IEDKey,QByteArray& fileData)
{
	if(graphicSvr!=NULL)
	{
		std::vector<unsigned char> tempByteSeq;
		if(graphicSvr->GetIEDIndexGraphic(IEDKey.toLocal8Bit().constData(),tempByteSeq))
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
bool GraphicSvrAdapter::GetSubnetGraphic(QString subNetKey,QByteArray& fileData)
{
	if(graphicSvr!=NULL)
	{
		std::vector<unsigned char> tempByteSeq;
		if(graphicSvr->GetSubnetGraphic(subNetKey.toLocal8Bit().constData(),tempByteSeq))
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

bool GraphicSvrAdapter::GetMsgFlowGraphic(QString IEDKey,QByteArray& fileData)
{
	if(graphicSvr!=NULL)
	{
		std::vector<unsigned char> tempByteSeq;
		if(graphicSvr->GetMsgFlowGraphic(IEDKey.toLocal8Bit().constData(),tempByteSeq))
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

bool GraphicSvrAdapter::GetTerminalGraphic(QString IEDKey,QByteArray& fileData)
{
	if(graphicSvr!=NULL)
	{
		std::vector<unsigned char> tempByteSeq;
		if(graphicSvr->GetTerminalGraphic(IEDKey.toLocal8Bit().constData(),tempByteSeq))
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

bool GraphicSvrAdapter::GetVirtualLoopGraphic(QString IEDKeyPri,QString IEDKeySec,int sigType,QByteArray& fileData)
{
	if(graphicSvr!=NULL)
	{
		std::vector<unsigned char> tempByteSeq;
		if(graphicSvr->GetVirtualLoopGraphic(IEDKeyPri.toLocal8Bit().constData(),IEDKeySec.toLocal8Bit().constData(),sigType,tempByteSeq))
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

bool GraphicSvrAdapter::GetMsgFlowDiffGraphic(QString IEDKey, QString otherIEDKey, QByteArray& fileData)
{
	if(graphicSvr!=NULL)
	{
		std::vector<unsigned char> tempByteSeq;
		if(graphicSvr->GetMsgFlowDiffGraphic(IEDKey.toLocal8Bit().constData(),otherIEDKey.toLocal8Bit().constData(), tempByteSeq))
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

bool GraphicSvrAdapter::GetVirtualLoopDiffGraphic(QString IEDKeyPri,QString IEDKeySec,int sigType,QByteArray& fileData)
{
	if(graphicSvr!=NULL)
	{
		std::vector<unsigned char> tempByteSeq;
		if(graphicSvr->GetVirtualLoopDiffGraphic(IEDKeyPri.toLocal8Bit().constData(), IEDKeySec.toLocal8Bit().constData(), sigType, tempByteSeq));
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

bool GraphicSvrAdapter::GetTerminalDiffGraphic(QString IEDKey, QString otherIEDKey, QByteArray& fileData)
{
	if(graphicSvr!=NULL)
	{
		std::vector<unsigned char> tempByteSeq;
		if(graphicSvr->GetTerminalDiffGraphic(IEDKey.toLocal8Bit().constData(), otherIEDKey.toLocal8Bit().constData(),tempByteSeq));
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