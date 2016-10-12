/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      GraphicSvrAdapter.h
  * Author:         zhang-xch
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   将QString转换为Std::String，供GraphicSvrImpl使用
  */ 
#ifndef GraphicSvrAdapter_h
#define GraphicSvrAdapter_h
#include "GraphicSvrAdapterInterface.h"
#include "QtCore/qcoreapplication.h"
// #include "../../src/svr/GraphicSvr/GraphicSvrImpl.h"
#include "GraphicSvrImpl.h"
using namespace std;

//类名：GraphicSvrAdapter
//功能：将GraphicSvrImpl提供的图形服务转换成本地接口
//特点：单例模式、适配器模式

class GraphicSvrAdapter :public GraphicSvrAdapterInterface
{
public:
	bool   GetSignalIEDGraphic(QString IEDKey,QByteArray& fileData);
	bool   GetTwoIEDGraphic(QString IEDKeyPri,QString IEDKeySec,QByteArray& fileData);
	bool   GetIEDIndexGraphic(QString IEDKey,QByteArray& fileData);
	
	//niuziming add
	bool   GetSubnetGraphic(QString subNetKey,QByteArray& fileData);
	bool   GetMsgFlowGraphic(QString IEDKey,QByteArray& fileData);
	bool   GetTerminalGraphic(QString IEDKey,QByteArray& fileData);
	bool   GetVirtualLoopGraphic(QString IEDKeyPri,QString IEDKeySec, int sigType, QByteArray& fileData);

	bool   GetMsgFlowDiffGraphic(QString IEDKey, QString otherIEDKey, QByteArray& fileData);
	bool   GetVirtualLoopDiffGraphic(QString IEDKeyPri,QString IEDKeySec,int sigType,QByteArray& fileData);
	bool   GetTerminalDiffGraphic(QString IEDKey, QString otherIEDKey, QByteArray& fileData);

	static GraphicSvrAdapter * GetInstance()  
	{  
		if (m_pInstance == NULL)
			m_pInstance = new GraphicSvrAdapter();  
		return m_pInstance; 
	};

private:
	GraphicSvrAdapter()
	{
		try 
		{
			graphicSvr = new GraphicSvrImpl();
		}
		catch (...) 
		{
			graphicSvr = NULL;
		}
	};

	static GraphicSvrAdapter    *m_pInstance;
	GraphicSvrImpl              *graphicSvr;

	class CGarbo
	{  
	public:  ~CGarbo()  
			 {  
				 if (GraphicSvrAdapter::m_pInstance) 
				 {
					 delete GraphicSvrAdapter::m_pInstance;  
				 }
			 }
	};
	static CGarbo Garbo;
};

#endif
