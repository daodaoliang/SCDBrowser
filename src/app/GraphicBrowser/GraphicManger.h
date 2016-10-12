#ifndef GRAPHICMANGER_H
#define GRAPHICMANGER_H
#include <QDebug>

#include "qdatastream.h"
#include "qfile.h"
#include "SecuMeasStructDefine.h"
#include "SCLModelSvrAdapter.h"
#include "GraphicNameGenerator.h"
#include "GraphicSvrAdapter.h"

class IEDGraphicsInfo
{
public:
	QString name;
	int     type;
	QString mainDev;
	QString relateDev;
    QByteArray graphicData;
	float   scaleValue;
};

//图形管理器;
class GraphicsManger
{
public:
	static GraphicsManger * GetInstance()  
	{  
		if (m_pInstance == NULL)
			m_pInstance = new GraphicsManger();  
		return m_pInstance; 
	};

	int GetCurGraphicIndex()
	{
		return preGraphicsList.size() - 1;
	};

	void SetCurGraphicIndex(int index)
	{
		if(index < curGraphics.size() & &index >= 0)
		{
			graphicIndex = index;
		}
	};

	void SetCurGraphicScale(float scaleValue)
	{
		if (preGraphicsList.size() > 0)
		{
			preGraphicsList.last().scaleValue = scaleValue;
		}
	};

	int GetCurGraphicType()
	{
		if (preGraphicsList.size() > 0)
		{
			return preGraphicsList.last().type;
		}
		return -1;
	};

	float GetCurGraphicScale()
	{
		if (preGraphicsList.size() > 0)
		{
			return preGraphicsList.last().scaleValue;
		}
		return 0;
	};

	bool LoadGraphicDataFromGraphicSvr(QString mainDevKey,QString relateDevKey,int nGraphicType,QByteArray& tempByteArray)
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
	};

	int GetGraphicCount()
	{
		return preGraphicsList.size();
	};

	QByteArray GetCurGraphicData()
	{
		if (preGraphicsList.size() > 0)
		{
			return preGraphicsList.last().graphicData;
		}
		return NULL;
	};

	//_____________________ 得到主设备IEDKEY
	QString GetCurGraphicIEdName()
	{
		if (preGraphicsList.size() > 0)
		{
			return preGraphicsList.last().mainDev;
		}
		return "";
	}

	bool IsFirstGraphic()
	{
		if (preGraphicsList.size() <= 1)
		{
			return true;
		}
		else
		{
			return false;
		}
	};

	bool MovePreGraphic()
	{
		if (preGraphicsList.size() > 1)
		{
			IEDGraphicsInfo tempInfo = preGraphicsList.last();
			preGraphicsList.removeLast();
			newtGraphicsList.push_front(tempInfo);
			return true;
		} 
		else
		{
			return false;
		}
	};

	bool IsLastGraphic()
	{
		if (newtGraphicsList.size() == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	};

	bool MoveNextGraphic()
	{
		if (newtGraphicsList.size() > 0)
		{
			IEDGraphicsInfo tempInfo = newtGraphicsList.first();
			preGraphicsList.push_back(tempInfo);
			newtGraphicsList.removeFirst();
			return true;
		}
		else
		{
			return false;
		}
	};

	//获得图数据
	QByteArray  LoadGraphicData(QString mainDev,QString relateDev,int graphicType)
	{
		QByteArray tempByteArray;
		tempByteArray.clear();

        IEDGraphicsInfo tempGraphicInfo;
        // 当端子排图时，重新加载数据，以便及时响应 配置文件中ShowOutSignalInTerminalGraphic配置
        if (graphicType == 3)
        {
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

            newtGraphicsList.clear();
            preGraphicsList.push_back(tempGraphicInfo);
            return tempByteArray;
        }

		int index = GetGraphicIndex(mainDev,relateDev,graphicType);
		if(index!=-1)
		{
			preGraphicsList.swap(index, preGraphicsList.size() - 1);
		}
		else
		{
			index = GetNextGraphicIndex(mainDev,relateDev,graphicType);
			if (index != -1)
			{
				tempGraphicInfo = newtGraphicsList.at(index);
				preGraphicsList.push_back(tempGraphicInfo);
				newtGraphicsList.removeAt(index);
			} 
			else
			{
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

				newtGraphicsList.clear();
				preGraphicsList.push_back(tempGraphicInfo);
			}
		}

		return tempByteArray;
	};

	//获得图在缓存中中序号
    int GetGraphicIndex(QString mainDev,QString relateDev,int graphicType)
	{
		int i = 0;
		for (QList<IEDGraphicsInfo>::iterator it = preGraphicsList.begin(); it != preGraphicsList.end(); it++)
		{
			if (mainDev == it->mainDev && relateDev == it->relateDev && graphicType == it->type)
			{
				return i;
			}
			i++;
		}
		return -1;
	};

	int GetNextGraphicIndex(QString mainDev,QString relateDev,int graphicType)
	{
		int i = 0;
		for (QList<IEDGraphicsInfo>::iterator it = newtGraphicsList.begin(); it != newtGraphicsList.end(); it++)
		{
			if (mainDev == it->mainDev && relateDev == it->relateDev && graphicType == it->type)
			{
				return i;
			}
			i++;
		}
		return -1;
	};

	void clearGraphics()
	{
		curGraphics.clear();
		nextGraphics.clear();
		preGraphicsList.clear();
		newtGraphicsList.clear();
		graphicIndex = -1;
	}

public:
	GraphicsManger()
	{
		sclModelSvrAdapter = SCLModelAdapter::GetInstance();
		graphicSvrAdapter  = GraphicSvrAdapter::GetInstance();
		graphicIndex = -1;
		preGraphicsList.clear();
		newtGraphicsList.clear();
	};

	~GraphicsManger(){};

private:
	static GraphicsManger  *m_pInstance;

	class CGarbo
	{  
	public:  ~CGarbo()
			 {  
				 if (GraphicsManger::m_pInstance) 
				 {
					 delete GraphicsManger::m_pInstance;
				 }
			 }
	};
	static CGarbo Garbo;

	QString voltageLevelName;
	QString substationName;
	QVector<IEDGraphicsInfo> curGraphics;
	QVector<IEDGraphicsInfo> nextGraphics;

	QList<IEDGraphicsInfo> preGraphicsList;
	QList<IEDGraphicsInfo> newtGraphicsList;

	int graphicIndex;

    SCLModelSvrAdapterInterface *sclModelSvrAdapter;
	GraphicSvrAdapterInterface  *graphicSvrAdapter;
};

#endif