#ifndef GraphicSvrSliceAdapter_h
#define GraphicSvrSliceAdapter_h
#include "SliceSCLModelInterface.h"
#include "GraphicSvrAdapterInterface.h"
#include "ice/ice.h"
#include "QtCore/qcoreapplication.h"
#define CFG_FILE_PATH "/../cfg/ssmsgrid/db/registry/registry.grid"

using namespace std;
using namespace SCLModel;

//类名：GraphicSvrSliceAdapter
//功能：将ICE提供的图形服务转换成本地接口
//特点：单例模式、适配器模式

class GraphicSvrSliceAdapter :public GraphicSvrAdapterInterface
{
public:
	bool   GetSignalIEDGraphic(QString IEDKey,QByteArray& fileData);
	bool   GetTwoIEDGraphic(QString IEDKeyPri,QString IEDKeySec,QByteArray& fileData);
	bool   GetIEDIndexGraphic(QString IEDKey,QByteArray& fileData);

	//niuziming add
	bool   GetSubnetGraphic(QString subNetKey,QByteArray& fileData);
	bool   GetMsgFlowGraphic(QString IEDKey,QByteArray& fileData);
	bool   GetTerminalGraphic(QString IEDKey,QByteArray& fileData);
	bool   GetVirtualLoopGraphic(QString IEDKeyPri,QString IEDKeySec,QByteArray& fileData);

	static GraphicSvrSliceAdapter * GetInstance()  
	{  
		if (m_pInstance == NULL)
			m_pInstance = new GraphicSvrSliceAdapter();  
		return m_pInstance; 
	};

private:
	GraphicSvrSliceAdapter()
	{
		try 
		{
			int argc=0;
			char* a="";
			char** argv=&a;
			int status = 0;
			Ice::CommunicatorPtr ic;
			Ice::ObjectPrx base;
			Ice::InitializationData initData;
			initData.properties = Ice::createProperties();
			QString path;  
			path = QCoreApplication::applicationDirPath() + CFG_FILE_PATH;
			initData.properties->load(path.toLocal8Bit().constData());
			ic = Ice::initialize(initData);
			base = ic->stringToProxy("GraphicSvr@GraphicSvrAdapter");
			graphicSvrAdapter = SliceGraphicsInterfacePrx::checkedCast(base);
		}
		catch (const Ice::Exception& ex) 
		{
			graphicSvrAdapter = NULL;
		}
	};
	void FromByteSeqToStream(::SCLModel::ByteSeq fileData,fstream &fileStream);


	static GraphicSvrSliceAdapter    *m_pInstance;
	SliceGraphicsInterfacePrx      graphicSvrAdapter;

	class CGarbo
	{  
	public:  ~CGarbo()  
			 {  
				 if (GraphicSvrSliceAdapter::m_pInstance) 
				 {
					 delete GraphicSvrSliceAdapter::m_pInstance;  
				 }
			 }
	};
	static CGarbo Garbo;
};

#endif
