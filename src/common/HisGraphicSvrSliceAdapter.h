#ifndef HisGraphicSvrSliceAdapter_H
#define HisGraphicSvrSliceAdapter_H

#include "HisGraphicSvrAdapterInterface.h"
#include "ice/ice.h"
#include "QtCore/qcoreapplication.h"
#define CFG_FILE_PATH "/../cfg/ssmsgrid/db/registry/registry.grid"

//������HisGraphicSvrSliceAdapter
//���ܣ���ICE�ṩ����ʷͼ�η���ת���ɱ��ؽӿ�
//�ص㣺����ģʽ��������ģʽ

class HisGraphicSvrSliceAdapter : public HisGraphicSvrAdapterInterface
{
public:
	virtual bool   GetGraphicData(QString graphicName, QString SCDVersion,QByteArray& fileData);//���ͼ���ļ���
	virtual bool   AddGraphic(QString graphicName, QString SCDVersion,QString graphicType,QByteArray& fileData);//����ͼ��
	virtual bool   DeleteGraphic(QString graphicName, QString SCDVersion); //ɾ��ͼ��
	virtual bool   IsGraphicExist(QString graphicName, QString SCDVersion);//ͼ���Ƿ����
	virtual bool   ModifyGraphic(QString graphicName, QString SCDVersion, QByteArray& fileData);//�޸�ͼ��

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