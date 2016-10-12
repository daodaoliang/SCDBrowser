#pragma once
#include "HmiGraphicsScene.h"
#include "CtrlBlockItem.h"
#include "GraphicTitleTable.h"
#include "iedtypeanalysor.h"

class HMIDLL_EXPORT DrawInfoFlowGraphicWithDiff
{
public:
	DrawInfoFlowGraphicWithDiff(void);
	DrawInfoFlowGraphicWithDiff(HmiGraphicsScene *scene, const QString &iedKey);
	~DrawInfoFlowGraphicWithDiff(void);

	void DrawGraph();			//绘制图形,供外部调用;

private:
	void InitGraphicData();		//初始化数据;
	void FillSlaveIEDVec();		//填充从IED列表;
	void GetAllSlaveHeight();	//计算所有从IED高度;
	void isDifferGraphic();		//判断是否为差异化状态;

	void DrawMasterIED();		//绘制主IED以及控制块;
	void DrawSlaveIEDs();		//绘制从IED以及控制块和信号线;
	int  DrawTitleTable(IEDTitleInfo& titleInfo);	//绘制右下角标题栏;
	void ResizeSceneRect(const int titleTableWidth);//重置图形区域;

	QString DrawMasterCB(const QCtrlBlockInfo& cbInfo, const int xPos, const int yPos);

	IEDTitleInfo FillTitleInfo_Simple();	//填充标题栏数据 -- 简化;
	IEDTitleInfo FillTitleInfo_Full();		//填充标题栏数据 -- 全部(暂时不用,勿删);
	
	QString GetItemIDStr();					//获取图元ID;
	
	//过滤Report控制块;
	void reportCBFilter(QVector<QString> OrigenVec, QVector<QString> *noReVec);

	//获取控制块ToolTip字符串;
	QString GetCBToolTipStr(const QCtrlBlockInfo &ctrlBlockInfo);

private:
	HmiGraphicsScene *mainScene;			//scene;
	QString m_MasterIEDKey;					//主IED的key;
	QIEDInfo m_MasterIEDInfo;				//主IED的Info;
	IEDType m_IedType;						//主IED的类型;
	QVector<CtrlBlockItem*> m_MasterCBs;	//主IED的控制块列表;
	QVector<QString> m_SlaveIEDKeys;		//从IEDKey列表;
	QVector<int> m_SlaveIEDCBCount;			//从IED包含控制块数量;
	
	int m_AllSlaveIEDHeight;				//所有从IED的高度和(含间隔);
	int m_LeftSlaveIEDHeight;				//左侧从IED的高度和(含间隔);
	
	int m_MasterIEDHeight;					//主IED高度;
	int m_MasterIEDWidth;					//主IED宽度;
	int m_IEDVerticalDis;					//从IED图元之间垂直距离;
	int m_IEDCBBorder;						//从IED图元与控制块图元上下空隙;
	
	int m_CBHeight;							//控制块图元高度;
	int m_CBWidth;							//控制块图元宽度;
	int m_CBVerticalDis;					//控制块图元之间垂直距离;

	QColor m_GooseCBRegularClr;				//Goose控制块常规颜色;
	QColor m_SVCBRegularClr;				//SV控制块常规颜色;
	QColor m_CBHighLightClr;				//控制块高亮颜色;

	bool m_IsDiffGraphic;					//是否为差异化模式;
};