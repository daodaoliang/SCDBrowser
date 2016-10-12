#ifndef HisGraphicSvrSliceAdapter_H
#define HisGraphicSvrSliceAdapter_H

#include "HisGraphicSvrAdapterInterface.h"
#include "ice/ice.h"
#include "QtCore/qcoreapplication.h"
#define CFG_FILE_PATH "/../cfg/ssmsgrid/db/registry/registry.grid"

//类名：HisGraphicSvrSliceAdapter
//功能：将ICE提供的历史图形服务转换成本地接口
//特点：单例模式、适配器模式

class HisGraphicSvrSliceAdapter : public HisGraphicSvrAdapterInterface
{
public:
	virtual bool   GetGraphicData(QString graphicName, QString SCDVersion,QByteArray& fileData);//获得图形文件流
	virtual bool   AddGraphic(QString graphicName, QString SCDVersion,QString graphicType,QByteArray& fileData);//增加图形
	virtual bool   DeleteGraphic(QString graphicName, QString SCDVersion); //删除图形
	virtual bool   IsGraphicExist(QString graphicName, QString SCDVersion);//图形是否存在
	virtual bool   ModifyGraphic(QString graphicName, QString SCDVersion, QByteArray& fileData);//修改图形

  static HisGraphicSvrSliceAdapter * GetInstance()  
  {  
	  if (m_pInstance == NULL)
		  m_pInstance = new HisGraphicSvrSliceAdapter();  
	  return m_pInstance; 
  };

private:
	HisGraphicSvrSliceAdapter()
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
			base = ic->stringToProxy("HisGraphicSvr@HisDataSvrAdapter");
			HisGraphicSvrAdapter = SliceHisGraphicInterfacePrx::checkedCast(base);
		}
		catch (const Ice::Exception& ex) 
		{
			HisGraphicSvrAdapter = NULL;
		}
	};

	static HisGraphicSvrSliceAdapter  *m_pInstance;
	SliceHisGraphicInterfacePrx       HisGraphicSvrAdapter;

	class CGarbo
	{  
	public:  ~CGarbo()  
			 {  
				 if (HisGraphicSvrSliceAdapter::m_pInstance) 
				 {
					 delete HisGraphicSvrSliceAdapter::m_pInstance;  
				 }
			 }
	};
	static CGarbo Garbo;
};

#endif