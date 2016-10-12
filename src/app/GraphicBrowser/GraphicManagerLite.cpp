#include "GraphicManagerLite.h"


CGraphicManagerLite::CGraphicManagerLite(void)
{
	sclModelSvrAdapter = SCLModelAdapter::GetInstance();
	graphicSvrAdapter  = GraphicSvrAdapter::GetInstance();
	m_IsGraphicExists = 0;
}


CGraphicManagerLite::~CGraphicManagerLite(void)
{
	sclModelSvrAdapter = NULL;
	graphicSvrAdapter = NULL;
}

bool CGraphicManagerLite::LoadGraphicDataFromGraphicSvr(QString mainDevKey,QString relateDevKey,int nGraphicType,QByteArray& tempByteArray)
{
	if(graphicSvrAdapter)
	{
		switch (nGraphicType)
		{
		case GRAPHIC_TYPE_NO_SUBNET:
			return graphicSvrAdapter->GetSubnetGraphic(mainDevKey,tempByteArray);
		case GRAPHIC_TYPE_NO_INFOMATION_FLOW:
			return graphicSvrAdapter->GetMsgFlowGraphic(mainDevKey,tempByteArray);
		case GRAPHIC_TYPE_NO_VIRTUAL_LINK:
			return graphicSvrAdapter->GetVirtualLoopGraphic(mainDevKey,relateDevKey,0,tempByteArray);
		case GRAPHIC_TYPE_NO_VIRTUAL_LINK_GOOSE:
			return graphicSvrAdapter->GetVirtualLoopGraphic(mainDevKey,relateDevKey,1,tempByteArray);
		case GRAPHIC_TYPE_NO_VIRTUAL_LINK_SV:
			return graphicSvrAdapter->GetVirtualLoopGraphic(mainDevKey,relateDevKey,2,tempByteArray);
		case GRAPHIC_TYPE_NO_VIRTUAL_SIGNAL:
			return graphicSvrAdapter->GetTerminalGraphic(mainDevKey,tempByteArray);
		case GRAPHIC_TYPE_NO_COMPARE:
			return graphicSvrAdapter->GetMsgFlowDiffGraphic(mainDevKey,relateDevKey,tempByteArray);
		case GRAPHIC_TYPE_NO_VIRTUAL_LINK_DIFF_GOOSE:
			return graphicSvrAdapter->GetVirtualLoopDiffGraphic(mainDevKey,relateDevKey,1,tempByteArray);
		case GRAPHIC_TYPE_NO_VIRTUAL_LINK_DIFF_SV:
			return graphicSvrAdapter->GetVirtualLoopDiffGraphic(mainDevKey,relateDevKey,2,tempByteArray);
		case GRAPHIC_TYPE_NO_VIRTUAL_SIGNAL_DIFF:
			return graphicSvrAdapter->GetTerminalDiffGraphic(mainDevKey,relateDevKey,tempByteArray);
		default:
			break;
		}
	}
	return false;
}

QByteArray CGraphicManagerLite::LoadGraphicData(QString mainDev,QString relateDev,int graphicType)
{
	QByteArray tempByteArray;
	tempByteArray.clear();
	IEDGraphicsInfo tempGraphicInfo;
	
	if(LoadGraphicDataFromGraphicSvr(mainDev,relateDev,graphicType,tempByteArray))
	{
		tempGraphicInfo.mainDev = mainDev;
		tempGraphicInfo.relateDev = relateDev;
		tempGraphicInfo.type = graphicType;
		GraphicNameGenerator translator;
		tempGraphicInfo.name = translator.FromMaintanceIEDInfoToGraphicName(mainDev,relateDev,graphicType);
		tempGraphicInfo.graphicData = tempByteArray;
		tempGraphicInfo.scaleValue = 1;
	}

	m_GraphicInfo = tempGraphicInfo;
	m_IsGraphicExists = 1;
	return tempByteArray;
}

int CGraphicManagerLite::IsGraphicExists()
{
	return m_IsGraphicExists;
}

double CGraphicManagerLite::GetGraphicScale()
{
	return m_GraphicInfo.scaleValue;
}

QByteArray CGraphicManagerLite::GetGraphicData()
{
	return m_GraphicInfo.graphicData;
}

SCLModelSvrAdapterInterface* CGraphicManagerLite::GetSCLModel()
{
	return sclModelSvrAdapter;
}
