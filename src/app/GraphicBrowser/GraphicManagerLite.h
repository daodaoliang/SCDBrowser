/** 
  Copyright (C), 2011-2016, Roof Tech. Co., Ltd. 
  * File name:      GraphicManagerLite.h
  * Author:         zhang-xch
  * Version:        V1.0
  * Date:           2013/12/25
  * Description:   ���ڻ������·ͼ�Ի������
  */ 
#pragma once
#include "GraphicManger.h"

class CGraphicManagerLite
{
public:
	CGraphicManagerLite(void);
	~CGraphicManagerLite(void);

	int IsGraphicExists();
	double GetGraphicScale();
	QByteArray GetGraphicData();

	bool LoadGraphicDataFromGraphicSvr(QString mainDevKey,QString relateDevKey,int nGraphicType,QByteArray& tempByteArray);
	QByteArray LoadGraphicData(QString mainDev,QString relateDev,int graphicType);

	SCLModelSvrAdapterInterface* GetSCLModel();

private:
	SCLModelSvrAdapterInterface *sclModelSvrAdapter;
	GraphicSvrAdapterInterface  *graphicSvrAdapter;

	IEDGraphicsInfo m_GraphicInfo;
	int m_IsGraphicExists;
};
