#include "DrawInfoFlowGraphicWithDiff.h"
#include "IEDMasterItem.h"
#include "IEDSlaveItem.h"
#include "VirLinkItem.h"
#include "TableSCLModelImpl.h"

/** 
  * @brief    DrawInfoFlowGraphicWithDiff
  * 
  * 无参构造函数;
  * 
  * @class    DrawInfoFlowGraphicWithDiff
  * @param    void :
  * @param    
  * @return   
  * @author   牛子明;
  * @date     2014/12/24
  * @remarks  
  */ 
DrawInfoFlowGraphicWithDiff::DrawInfoFlowGraphicWithDiff(void)
{
}

/** 
  * @brief    DrawInfoFlowGraphicWithDiff
  * 
  * 构造函数;
  * 
  * @class    DrawInfoFlowGraphicWithDiff
  * @param    HmiGraphicsScene *scene : 主场景指针;
  * @param    const QString &iedKey   : 主IEDKey;
  * @param    
  * @return   
  * @author   牛子明;
  * @date     2014/12/24
  * @remarks  
  */ 
DrawInfoFlowGraphicWithDiff::DrawInfoFlowGraphicWithDiff(HmiGraphicsScene *scene, const QString &iedKey)
{
	mainScene = scene;
	m_MasterIEDKey = iedKey;

	//初始化数据;
	InitGraphicData();
}

/** 
  * @brief    ~DrawInfoFlowGraphicWithDiff
  * 
  * 析构函数;
  * 
  * @class    DrawInfoFlowGraphicWithDiff
  * @param    void :
  * @param    
  * @return   
  * @author   牛子明;
  * @date     2014/12/24
  * @remarks  
  */ 
DrawInfoFlowGraphicWithDiff::~DrawInfoFlowGraphicWithDiff(void)
{
}

/** 
  * @brief    DrawGraph
  * 
  * 绘制图形,供外部调用;
  * 
  * @class    DrawInfoFlowGraphicWithDiff
  * @param    
  * @return   void
  * @author   牛子明;
  * @date     2014/12/24
  * @remarks  
  */ 
void DrawInfoFlowGraphicWithDiff::DrawGraph()
{
	mainScene->clear();
	
	//绘制主IED及其控制块;
	DrawMasterIED();

	//绘制从IED及其控制块与信号线;
	DrawSlaveIEDs();
	
	int titleTableColumnWidth = 240;
	if (!m_IsDiffGraphic)
	{
		//填充标题栏信息;
		IEDTitleInfo titleInfo = FillTitleInfo_Simple();

		//绘制标题栏;
		titleTableColumnWidth = DrawTitleTable(titleInfo);
	}

	//重置场景区域;
	ResizeSceneRect(titleTableColumnWidth);
}

/** 
  * @brief    DrawMasterIED
  * 
  * 绘制主IED及其控制块;
  * 
  * @class    DrawInfoFlowGraphicWithDiff
  * @param    
  * @return   void
  * @author   牛子明;
  * @date     2014/12/24
  * @remarks  
  */ 
void DrawInfoFlowGraphicWithDiff::DrawMasterIED()
{
	CIEDMasterItem *masterItem = new CIEDMasterItem(mainScene);
	if (m_LeftSlaveIEDHeight > 400)
	{
		m_MasterIEDHeight = m_LeftSlaveIEDHeight + 100;
	}
	masterItem->setHeight(m_MasterIEDHeight);
	masterItem->setWidth(m_MasterIEDWidth);
	masterItem->setIEDKey(m_MasterIEDKey);
	masterItem->setIEDDesc(m_MasterIEDInfo.desc);
	masterItem->setIEDName(m_MasterIEDInfo.name);
	masterItem->setIEDType(m_IedType);
	masterItem->setMasterKey(m_MasterIEDKey);
	masterItem->setChangeType((ItemChangeType)m_MasterIEDInfo.DiffState);
	masterItem->setZValue(1);
	masterItem->setItemID(GetItemIDStr());
	mainScene->addItem(masterItem);
	masterItem->setPos(500,40);
	masterItem->setToolTipPos(500, 40);
	masterItem->setDiffInfo(m_MasterIEDInfo.diffDesc);
	if (m_IsDiffGraphic)
	{
		masterItem->setLinkType(link_type_diff_signal);
	}
	else
	{
		masterItem->setLinkType(link_type_vterm);
	}

	QVector<QString> masterCBList, realCBList;
	mainScene->GetGraphicModel()->GetCtrlBlockListByIED(m_MasterIEDKey,&masterCBList);
	reportCBFilter(masterCBList, &realCBList);
	m_MasterIEDHeight = masterItem->getAdjustHeight(realCBList.size());

	const int MASTER_CB_XPOS = 517;
	int masterCBYPos = (m_MasterIEDHeight - 42 * realCBList.size() + 15)/2 + 40;

	for (int cbIndex = 0; cbIndex != realCBList.size(); cbIndex++)
	{
		QCtrlBlockInfo cbInfo;
		mainScene->GetGraphicModel()->GetCtrlBlockInfo(realCBList.at(cbIndex),cbInfo);
		QString itemID = DrawMasterCB(cbInfo, MASTER_CB_XPOS, masterCBYPos);
		if (itemID.compare("") == 0)
		{
			continue;
		}
		masterItem->appendCtrlBlock(itemID);
		masterCBYPos = masterCBYPos + m_CBHeight + m_CBVerticalDis;
	}
}

QString DrawInfoFlowGraphicWithDiff::DrawMasterCB(const QCtrlBlockInfo& cbInfo, const int xPos, const int yPos)
{
	CtrlBlockItem *masterCBItem = new CtrlBlockItem(mainScene);

	if (cbInfo.type == "SampledValueControl")
	{
		masterCBItem->setSigType(svType);
		masterCBItem->setRegularClr(m_SVCBRegularClr);
	}
	else if (cbInfo.type == "GSEControl")
	{
		masterCBItem->setSigType(gooseType);
		masterCBItem->setRegularClr(m_GooseCBRegularClr);
	}
	else
	{
		return "";
	}
	masterCBItem->setHighLightClr(m_CBHighLightClr);
	const int MASTER_CB_WIDTH = 96;
	masterCBItem->setWidth(MASTER_CB_WIDTH);
	masterCBItem->setCBName(cbInfo.name);
	masterCBItem->setAPPID(cbInfo.APPID);
	masterCBItem->setItemID(GetItemIDStr());
	masterCBItem->setChangeType((ItemChangeType)cbInfo.DiffState);
	masterCBItem->setPos(xPos, yPos);
	masterCBItem->setToolTip(GetCBToolTipStr(cbInfo));
	masterCBItem->setToolTipPos(xPos, yPos);
	masterCBItem->setDiffInfo(cbInfo.diffDesc);
	
	mainScene->addItem(masterCBItem);
	m_MasterCBs.append(masterCBItem);
	return masterCBItem->getItemID();
}

/** 
  * @brief    DrawSlaveIEds
  * 
  * 绘制从IED及其控制块和信号线;
  * 
  * @class    DrawInfoFlowGraphicWithDiff
  * @param    
  * @return   void
  * @author   牛子明;
  * @date     2014/12/24
  * @remarks  
  */ 
void DrawInfoFlowGraphicWithDiff::DrawSlaveIEDs()
{
	IEDTypeAnalysor typeAnalysor;
	int leftHeight = 0;
	int leftYPos = (m_MasterIEDHeight - m_LeftSlaveIEDHeight)/2 + 40;
	int rightYPos = (m_MasterIEDHeight - m_AllSlaveIEDHeight + m_LeftSlaveIEDHeight)/2 + 40;
	bool isRight = false;
	for (int slaveIndex = 0; slaveIndex != m_SlaveIEDKeys.size(); slaveIndex++)
	{
		QIEDInfo iedModel;
		mainScene->GetGraphicModel()->GetIEDInfo(m_SlaveIEDKeys.at(slaveIndex),iedModel);
		QVector<QString> cbListWithRpt;
		mainScene->GetGraphicModel()->GetSlaveCtrlBlockByIEDKeys(m_MasterIEDKey, m_SlaveIEDKeys.at(slaveIndex), &cbListWithRpt);
		QVector<QString> cbList;
		reportCBFilter(cbListWithRpt, &cbList);
		if (cbList.size() == 0)
		{
			continue;
		}
		CIEDSlaveItem *slaveItem = new CIEDSlaveItem(mainScene);

		slaveItem->setIedSlaveType(0);

		slaveItem->setIEDDesc(iedModel.desc);
		slaveItem->setIEDName(iedModel.name);
		IEDType sIedType = typeAnalysor.parseIEDType(iedModel.name, iedModel.desc);
		slaveItem->setIEDType(sIedType);
		slaveItem->setMasterKey(m_SlaveIEDKeys.at(slaveIndex));
		slaveItem->setChangeType((ItemChangeType)iedModel.DiffState);
		if (m_IsDiffGraphic)
		{
			slaveItem->setLinkType(link_type_diff_msgflow);
		} 
		else
		{
			slaveItem->setLinkType(link_type_msgflow);
		}
		slaveItem->setSideOfMaster(isRight);
		slaveItem->setHeight(42 * cbList.size() + 5);
		slaveItem->setWidth(140);
		slaveItem->setItemID(GetItemIDStr());
		mainScene->addItem(slaveItem);
		int cbYPos = leftYPos + 10;
		int cbRYPos = rightYPos + 10;
		for (int cbIndex = 0; cbIndex != cbList.size(); cbIndex++)
		{
			QCtrlBlockInfo cbInfo;
			if (cbList.at(cbIndex) != "接收")
			{
				mainScene->GetGraphicModel()->GetCtrlBlockInfo(cbList.at(cbIndex),cbInfo);
			} 
			else
			{
				cbInfo.name = "接收";
				cbInfo.type = "Input";
				cbInfo.DiffState = DIFFSTATE_NORMAL;
			}

			CtrlBlockItem *cbItem = new CtrlBlockItem(mainScene);
			if (cbInfo.type == "Input")
			{
				cbItem->setSigType(inputType);
				cbItem->setRegularClr(QColor(255,255,255));
				cbItem->setHighLightClr(m_CBHighLightClr);
			}
			else if(cbInfo.type == "GSEControl")
			{
				cbItem->setSigType(gooseType);
				cbItem->setRegularClr(m_GooseCBRegularClr);
				cbItem->setHighLightClr(m_CBHighLightClr);
			}
			else if (cbInfo.type == "SampledValueControl")
			{
				cbItem->setSigType(svType);
				cbItem->setRegularClr(m_SVCBRegularClr);
				cbItem->setHighLightClr(m_CBHighLightClr);
			}else
			{
				continue;
			}
			cbItem->setAPPID(cbInfo.APPID);
			cbItem->setChangeType((ItemChangeType)cbInfo.DiffState);
			cbItem->setItemID(GetItemIDStr());
			cbItem->setToolTip(GetCBToolTipStr(cbInfo));
			slaveItem->appendCtrlBlock(cbItem->getItemID());
			mainScene->addItem(cbItem);
			if (cbInfo.type == "Input")		//接收;
			{
				QVector<QString> muCBList;
				mainScene->GetGraphicModel()->GetSlaveCtrlBlockByIEDKeys(m_SlaveIEDKeys.at(slaveIndex), m_MasterIEDKey, &muCBList);
				for (int muI = 0; muI != muCBList.size(); muI++)
				{
					QCtrlBlockInfo muCBInfo;
					//chenxin modify 2014.10.18 --->
					if (muCBList.at(muI) != "接收")
					{
						mainScene->GetGraphicModel()->GetCtrlBlockInfo(muCBList.at(muI),muCBInfo);
					} 
					//chenxin modify 2014.10.18 <---

					for (int mI = 0; mI != m_MasterCBs.size(); mI++)
					{
						QString tempName = m_MasterCBs.at(mI)->getCBName();
						QString tempAPPID = m_MasterCBs.at(mI)->getAPPID();
						if (tempName == muCBInfo.name && tempAPPID == muCBInfo.APPID)
						{
							QPointF tempPt = m_MasterCBs.at(mI)->pos();
							QPointF mCBPt(tempPt.x() + 96, tempPt.y()+6);
							CVirLinkItem *vlinkSToMItem = new CVirLinkItem(mainScene);
							int sType = m_MasterCBs.at(mI)->getSigType();
							vlinkSToMItem->setTypeLine(sType);
							vlinkSToMItem->setMasterSlaveIEDKey(m_MasterIEDKey, m_SlaveIEDKeys.at(slaveIndex));
							vlinkSToMItem->setCtrlBlockKey(muCBList.at(muI));
							VirLinkDiff virDiff = mainScene->GetGraphicModel()->GetVirLinkStateByIEDKeyAndCBKey(m_MasterIEDKey, m_SlaveIEDKeys.at(slaveIndex), muCBList.at(muI));
							vlinkSToMItem->setChangeType((ItemChangeType)virDiff.state);
							QString strDiffCount = QString::number(virDiff.iCount);
							QString qureStr=strDiffCount+"#"+virDiff.diffDesc;
							vlinkSToMItem->setDiffInfo(qureStr);
							vlinkSToMItem->setSrcItemID(m_MasterCBs.at(mI)->getItemID());
							vlinkSToMItem->setDestItemID(cbItem->getItemID());
							if (isRight)
							{
								cbItem->setPos(730, cbRYPos);
								cbItem->setIsRight(isRight);
								cbItem->setToolTipPos(730, cbRYPos);
								vlinkSToMItem->setStartPt(mCBPt);	//QPointF mCBPt(tempPt.x() + 96, tempPt.y() + 4);
								vlinkSToMItem->setEndPt(QPointF(730,cbRYPos));
								vlinkSToMItem->setPos(mCBPt);		//QPointF mCBPt(tempPt.x() + 96, tempPt.y() + 4);
								vlinkSToMItem->setToolTipPos(mCBPt.x(), mCBPt.y());
							}
							else
							{
								cbItem->setPos(320, cbYPos);
								cbItem->setToolTipPos(320, cbYPos);
								vlinkSToMItem->setStartPt(QPointF(tempPt.x(), tempPt.y() + 4));
								vlinkSToMItem->setEndPt(QPointF(400, cbYPos));
								vlinkSToMItem->setPos(400, cbYPos+6);
								vlinkSToMItem->setToolTipPos(400, cbYPos+6);
							}
							if (muCBInfo.type == "GSEControl")
							{
								if (m_IsDiffGraphic)
								{
									vlinkSToMItem->setLinkType(link_type_vloop_diff_goose);
								} 
								else
								{
									vlinkSToMItem->setLinkType(link_type_vloop_goose);
								}
							}
							else if(muCBInfo.type == "SampledValueControl")
							{
								if (m_IsDiffGraphic)
								{
									vlinkSToMItem->setLinkType(link_type_vloop_diff_sv);
								} 
								else
								{
									vlinkSToMItem->setLinkType(link_type_vloop_sv);
								}
							}
							vlinkSToMItem->setItemID(GetItemIDStr());
							slaveItem->appendCtrlBlock(vlinkSToMItem->getItemID());
							slaveItem->appendCtrlBlock(m_MasterCBs.at(mI)->getItemID());
							(m_MasterCBs.at(mI))->appendAssoID(vlinkSToMItem->getItemID());
							(m_MasterCBs.at(mI))->appendAssoID(cbItem->getItemID());
							cbItem->appendAssoID((m_MasterCBs.at(mI))->getItemID());
							cbItem->appendAssoID(vlinkSToMItem->getItemID());
							mainScene->addItem(vlinkSToMItem);
						}
					}
				}
			}
			else
			{
				CVirLinkItem *vlinkSToMItem = new CVirLinkItem(mainScene);

				vlinkSToMItem->setItemID(vlinkSToMItem->getItemID());
				vlinkSToMItem->setTypeLine(cbItem->getSigType());
				vlinkSToMItem->setMasterSlaveIEDKey(m_SlaveIEDKeys.at(slaveIndex), m_MasterIEDKey);
				vlinkSToMItem->setCtrlBlockKey(cbList.at(cbIndex));
				VirLinkDiff virDiff = mainScene->GetGraphicModel()->GetVirLinkStateByIEDKeyAndCBKey(m_SlaveIEDKeys.at(slaveIndex), m_MasterIEDKey, cbList.at(cbIndex));
				vlinkSToMItem->setChangeType((ItemChangeType)virDiff.state);
				QString strDiffCount = QString::number(virDiff.iCount);
				QString qureStr=strDiffCount+"#"+virDiff.diffDesc;
				vlinkSToMItem->setDiffInfo(qureStr);
				vlinkSToMItem->setSrcItemID(cbItem->getItemID());
				vlinkSToMItem->setDestItemID("");
				if(cbInfo.type == "GSEControl")
				{
					if (m_IsDiffGraphic)
					{
						vlinkSToMItem->setLinkType(link_type_vloop_diff_goose);
					} 
					else
					{
						vlinkSToMItem->setLinkType(link_type_vloop_goose);
					}
				}
				else if (cbInfo.type == "SampledValueControl")
				{
					if (m_IsDiffGraphic)
					{
						vlinkSToMItem->setLinkType(link_type_vloop_diff_sv);
					} 
					else
					{
						vlinkSToMItem->setLinkType(link_type_vloop_sv);
					}
				}
				if (isRight)
				{
					cbItem->setPos(730, cbRYPos);
					cbItem->setIsRight(isRight);
					cbItem->setToolTip(GetCBToolTipStr(cbInfo));
					cbItem->setToolTipPos(730, cbRYPos);
					cbItem->setDiffInfo(cbInfo.diffDesc);
					vlinkSToMItem->setStartPt(QPointF(100,0));
					vlinkSToMItem->setEndPt(QPointF(0,0));
					vlinkSToMItem->setPos(630, cbRYPos+6);
					vlinkSToMItem->setToolTipPos(630, cbRYPos+6);
					cbRYPos = cbRYPos + 42;
				}
				else
				{
					cbItem->setPos(320, cbYPos);
					cbItem->setToolTip(GetCBToolTipStr(cbInfo));
					cbItem->setToolTipPos(320, cbYPos);
					cbItem->setDiffInfo(cbInfo.diffDesc);
					vlinkSToMItem->setStartPt(QPointF(0,0));
					vlinkSToMItem->setEndPt(QPointF(100,0));
					vlinkSToMItem->setPos(400, cbYPos+6);
					vlinkSToMItem->setToolTipPos(400, cbYPos+6);
					cbYPos = cbYPos + 42;
				}
				vlinkSToMItem->setItemID(GetItemIDStr());
				slaveItem->appendCtrlBlock(vlinkSToMItem->getItemID());
				cbItem->appendAssoID(vlinkSToMItem->getItemID());
				mainScene->addItem(vlinkSToMItem);
			}
		}

		if (isRight)
		{
			slaveItem->setPos(810,rightYPos);
			slaveItem->setToolTipPos(810, rightYPos);
			slaveItem->setDiffInfo(iedModel.diffDesc);
			rightYPos = rightYPos + m_SlaveIEDCBCount.at(slaveIndex) * 42 + 15;
		}
		else
		{
			slaveItem->setPos(180,leftYPos);
			slaveItem->setToolTipPos(180, leftYPos);
			slaveItem->setDiffInfo(iedModel.diffDesc);
			leftYPos = leftYPos + m_SlaveIEDCBCount.at(slaveIndex) * 42 + 15;
		}

		//判断左边是否已满;
		if (isRight)
		{
			continue;
		}
		leftHeight = leftHeight + m_SlaveIEDCBCount.at(slaveIndex) * 42 + 15;
		if (leftHeight >= m_LeftSlaveIEDHeight)
		{
			isRight = true;
		}
	}
}

/** 
  * @brief    DrawTitleTable
  * 
  * 绘制标题栏;
  * 
  * @class    DrawInfoFlowGraphicWithDiff
  * @param    IEDTitleInfo& titleInfo : 标题栏信息;
  * @param    
  * @return   int
  * @author   牛子明;
  * @date     2014/12/24
  * @remarks  
  */ 
int DrawInfoFlowGraphicWithDiff::DrawTitleTable(IEDTitleInfo& titleInfo)
{
	CGraphicTitleTable titleTable(mainScene);

	const int TITLETABLE_XPOS = 700;
	const int TITLETABLE_YPOS = m_MasterIEDHeight + 20;
	QPoint titleOrigenPt(TITLETABLE_XPOS, TITLETABLE_YPOS);
	
	titleTable.setOrigenPt(titleOrigenPt);	//设置左上角原点;
	titleTable.DrawTitleInfo(m_MasterIEDKey, titleInfo);
	
	return titleTable.getColumnWidth();
}

/** 
  * @brief    ResizeSceneRect
  * 
  * 重置场景区域;
  * 
  * @class    DrawInfoFlowGraphicWithDiff
  * @param    const int titleTableWidth : 标题栏表格单列宽度;
  * @param    
  * @return   void
  * @author   牛子明;
  * @date     2014/12/24
  * @remarks  
  */ 
void DrawInfoFlowGraphicWithDiff::ResizeSceneRect(const int titleTableWidth)
{
	const int TITLETABLE_XPOS = 700;
	const int RIGHT_BORDER = 50;
	const int BOTTOM_BORDER = 200;

	QRectF recScene = mainScene->sceneRect();
	recScene.setRight(TITLETABLE_XPOS + titleTableWidth*2 + RIGHT_BORDER);
	recScene.setBottom(m_MasterIEDHeight + BOTTOM_BORDER);
	mainScene->setSceneRect(recScene);
}

/** 
  * @brief    InitGraphicData
  * 
  * 初始化绘制图形所需数据;
  * 
  * @class    DrawInfoFlowGraphicWithDiff
  * @param    
  * @return   void
  * @author   牛子明;
  * @date     2014/12/24
  * @remarks  
  */ 
void DrawInfoFlowGraphicWithDiff::InitGraphicData()
{
	const int IED_DEFAULT_HEIGHT = 500;
	const int IED_DEFAULT_WIDTH = 130;
	const int IED_VERTICAL_DIS = 10;
	const int IED_CB_BORDER = 10;

	const int CB_DEFAULT_HEIGHT = 27;
	const int CB_DEFAULT_WIDTH = 80;
	const int CB_VERTICAL_DIS = 15;

	m_MasterIEDHeight = IED_DEFAULT_HEIGHT;
	m_MasterIEDWidth = IED_DEFAULT_WIDTH;
	m_IEDVerticalDis = IED_VERTICAL_DIS;
	m_IEDCBBorder = IED_CB_BORDER;

	m_CBHeight = CB_DEFAULT_HEIGHT;
	m_CBWidth = CB_DEFAULT_WIDTH;
	m_CBVerticalDis = CB_VERTICAL_DIS;

	m_AllSlaveIEDHeight = 0;
	m_LeftSlaveIEDHeight = 0;

	m_GooseCBRegularClr = QColor(220,244,253);
	m_SVCBRegularClr = QColor(254,251,220);
	m_CBHighLightClr = QColor(253,217,0);

	//获取主IED信息;
	mainScene->GetGraphicModel()->GetIEDInfo(m_MasterIEDKey, m_MasterIEDInfo);

	//获取主IED类型;
	IEDTypeAnalysor typeAnalysor;
	m_IedType = typeAnalysor.parseIEDType(m_MasterIEDInfo.name, m_MasterIEDInfo.desc);

	isDifferGraphic();
	FillSlaveIEDVec();
	GetAllSlaveHeight();
}

/** 
  * @brief    isDifferGraphic
  * 
  * 判断是否为差异化状态;
  * 
  * @class    DrawInfoFlowGraphicWithDiff
  * @param    
  * @return   void
  * @author   牛子明;
  * @date     2014/12/24
  * @remarks  
  */ 
void DrawInfoFlowGraphicWithDiff::isDifferGraphic()
{
	QList<QString> keyList = m_MasterIEDKey.split(".");
	if (keyList.size() == 2)
	{
		m_IsDiffGraphic = true;
	}
	else
	{
		m_IsDiffGraphic = false;
	}
}

/** 
  * @brief    FillSlaveIEDVec
  * 
  * 填充从IED列表;
  * 
  * @class    DrawInfoFlowGraphicWithDiff
  * @param    
  * @return   void
  * @author   牛子明;
  * @date     2014/12/24
  * @remarks  
  */ 
void DrawInfoFlowGraphicWithDiff::FillSlaveIEDVec()
{
	QVector<QString> tempSlaveKeys;
	mainScene->GetGraphicModel()->GetIEDListByLogicalRealitionWithCurrentIED(m_MasterIEDKey, &tempSlaveKeys);//根据主IED获取从IED
	for (int i = 0; i != tempSlaveKeys.size(); i++)
	{
		QIEDInfo iedInfo;
		mainScene->GetGraphicModel()->GetIEDInfo(tempSlaveKeys.at(i), iedInfo);
		if (iedInfo.name == "")	//过滤空信息IED;
		{
			continue;
		}
		m_SlaveIEDKeys.append(tempSlaveKeys.at(i));
	}
}

/** 
  * @brief    GetAllSlaveHeight
  * 
  * 获取从IED总高度;
  * 
  * @class    DrawInfoFlowGraphicWithDiff
  * @param    
  * @return   void
  * @author   牛子明;
  * @date     2014/12/24
  * @remarks  
  */ 
void DrawInfoFlowGraphicWithDiff::GetAllSlaveHeight()
{
	if (m_SlaveIEDKeys.size() == 0)
	{
		return;
	}
	else
	{
		for (int slaveIndex = 0; slaveIndex != m_SlaveIEDKeys.size(); slaveIndex++)
		{
			QVector<QString> cbListWithRpt;
			mainScene->GetGraphicModel()->GetSlaveCtrlBlockByIEDKeys(m_MasterIEDKey, m_SlaveIEDKeys.at(slaveIndex), &cbListWithRpt);
			QVector<QString> cbList;
			reportCBFilter(cbListWithRpt,&cbList);
			m_SlaveIEDCBCount.append(cbList.size());

			int all_CBHeight = m_CBHeight * cbList.size();
			int all_CBVerticalDis = m_CBVerticalDis * (cbList.size() - 1);
			int topBottomBorder = m_IEDCBBorder * 2;
			m_AllSlaveIEDHeight += all_CBHeight + all_CBVerticalDis + topBottomBorder + m_IEDVerticalDis;
		}
		m_AllSlaveIEDHeight -= 10;
		int temp = m_AllSlaveIEDHeight/2;

		for (int cIndex = 0; cIndex != m_SlaveIEDCBCount.size(); cIndex++)
		{
			m_LeftSlaveIEDHeight += m_CBHeight * m_SlaveIEDCBCount.at(cIndex) + m_CBVerticalDis * (m_SlaveIEDCBCount.at(cIndex) - 1) + m_IEDCBBorder * 2 + m_IEDVerticalDis;
			if (m_LeftSlaveIEDHeight >= temp)
			{
				m_LeftSlaveIEDHeight -= 10;
				break;
			}
		}
	}
}

/** 
  * @brief    FillTitleInfo_Simple
  * 
  * 填充标题栏信息(简化);
  * 
  * @class    DrawInfoFlowGraphicWithDiff
  * @param    
  * @return   IEDTitleInfo
  * @author   牛子明;
  * @date     2014/12/24
  * @remarks  
  */ 
IEDTitleInfo DrawInfoFlowGraphicWithDiff::FillTitleInfo_Simple()
{
	IEDTitleInfo titleInfo;

	titleInfo.IEDName = m_MasterIEDInfo.name;
	titleInfo.confVer = m_MasterIEDInfo.configVer;
	titleInfo.desc = m_MasterIEDInfo.desc;
	titleInfo.manufac = m_MasterIEDInfo.manu;
	titleInfo.IEDModel = m_MasterIEDInfo.type;
	titleInfo.CRCCode = m_MasterIEDInfo.crc;
	switch (m_IedType)
	{
	case P:
		titleInfo.IEDTypeStr = "保护装置";
		break;
	case C:
		titleInfo.IEDTypeStr = "测控装置";
		break;
	case ST:
		titleInfo.IEDTypeStr = "智能终端";
		break;
	case MU:
		titleInfo.IEDTypeStr = "合并单元";
		break;
	case MS:
		titleInfo.IEDTypeStr = "合并单元智能终端一体化装置";
		break;
	case PC:
		titleInfo.IEDTypeStr = "保测一体装置";
		break;
	case NO:
		titleInfo.IEDTypeStr = "其他";
		break;
	}

	titleInfo.smv = QString::number(mainScene->GetGraphicModel()->GetSignalCountByIEDKey(m_MasterIEDKey, typeSV_));
	titleInfo.GooseInOut = QString::number(mainScene->GetGraphicModel()->GetSignalCountByIEDKey(m_MasterIEDKey, typeGoose_));
	QVector<QString> subNetKeys;
	mainScene->GetGraphicModel()->GetSubNetKeysByIEDKey(m_MasterIEDKey, &subNetKeys);
	titleInfo.subNetName = subNetKeys;
	
	return titleInfo;
}

/** 
  * @brief    FillTitleInfo_Full
  * 
  * 填充标题栏信息(全部);
  * 
  * @class    DrawInfoFlowGraphicWithDiff
  * @param    
  * @return   IEDTitleInfo
  * @author   牛子明;
  * @date     2014/12/24
  * @remarks  
  */ 
IEDTitleInfo DrawInfoFlowGraphicWithDiff::FillTitleInfo_Full()
{
	IEDTitleInfo titleInfo;
	//titleInfo.IEDName = m_MasterIEDInfo.name;
	//titleInfo.confVer = m_MasterIEDInfo.configVer;
	//titleInfo.desc = m_MasterIEDInfo.desc;
	//titleInfo.IEDModel = m_MasterIEDInfo.type;
	//titleInfo.CRCCode = m_MasterIEDInfo.crc;
	//titleInfo.manu = m_MasterIEDInfo.manu;
	//switch (m_IedType)
	//{
	//case P:
	//	titleInfo.IEDTypeStr = "保护装置";
	//	break;
	//case C:
	//	titleInfo.IEDTypeStr = "测控装置";
	//	break;
	//case ST:
	//	titleInfo.IEDTypeStr = "智能终端";
	//	break;
	//case MU:
	//	titleInfo.IEDTypeStr = "合并单元";
	//	break;
	//case MS:
	//	titleInfo.IEDTypeStr = "合并单元智能终端一体化装置";
	//	break;
	//case PC:
	//	titleInfo.IEDTypeStr = "保测一体装置";
	//	break;
	//case NO:
	//	titleInfo.IEDTypeStr = "其他";
	//	break;
	//}

	//titleInfo.SVIn  = QString::number(m_MasterIEDInfo.SVInSigCount);
	//titleInfo.SVOut = QString::number(m_MasterIEDInfo.SVOutSigCount);
	//titleInfo.GooseIn  = QString::number(m_MasterIEDInfo.GooseInSigCount);
	//titleInfo.GooseOut = QString::number(m_MasterIEDInfo.GooseOutSigCount);

	//QVector<QString> subNetKeys;
	//mainScene->GetGraphicModel()->GetSubNetKeysByIEDKey(m_MasterIEDKey, &subNetKeys);
	//titleInfo.subNetName = subNetKeys;

	return titleInfo;
}

/** 
  * @brief    reportCBFilter
  * 
  * 过滤Report控制块;
  * 
  * @class    DrawInfoFlowGraphicWithDiff
  * @param    QVector<QString> OrigenVec : 原列表;
  * @param    QVector<QString> *noReVec  : 过滤后列表;
  * @param    
  * @return   void
  * @author   牛子明;
  * @date     2014/12/24
  * @remarks  
  */ 
void DrawInfoFlowGraphicWithDiff::reportCBFilter(QVector<QString> OrigenVec, QVector<QString> *noReVec)
{
	noReVec->clear();
	for (int i = 0; i != OrigenVec.size(); i++)
	{
		if (OrigenVec.at(i).compare("接收") != 0)
		{
			QCtrlBlockInfo cbInfo;
			mainScene->GetGraphicModel()->GetCtrlBlockInfo(OrigenVec.at(i), cbInfo);
			if (cbInfo.type == "ReportControl")
			{
				continue;
			}
		}
		noReVec->append(OrigenVec.at(i));
	}
}

/** 
  * @brief    GetItemIDStr
  * 
  * 获取图元在场景中的ID;
  * 
  * @class    DrawInfoFlowGraphicWithDiff
  * @param    
  * @return   QString
  * @author   牛子明;
  * @date     2014/12/24
  * @remarks  
  */ 
QString DrawInfoFlowGraphicWithDiff::GetItemIDStr()
{
	QList<QGraphicsItem*> iList = mainScene->items();
	return QString::number(iList.size());
}

/** 
  * @brief    GetCBToolTipStr
  * 
  * 获取控制块ToolTip字符串;
  * 
  * @class    DrawInfoFlowGraphicWithDiff
  * @param    const QCtrlBlockInfo &ctrlBlockInfo : 控制块Info;
  * @param    
  * @return   QString
  * @author   牛子明;
  * @date     2014/12/24
  * @remarks  
  */ 
QString DrawInfoFlowGraphicWithDiff::GetCBToolTipStr(const QCtrlBlockInfo &ctrlBlockInfo)
{
	QString toolTipStr = "";
	if (ctrlBlockInfo.name.compare("接收") == 0)
	{
		return "接收";
	}
	toolTipStr += "APPID : 0x" + ctrlBlockInfo.APPID + "\n";
	toolTipStr += "name : " + ctrlBlockInfo.name + "\n";
	toolTipStr += "desc : " + ctrlBlockInfo.desc + "\n";
	toolTipStr += "type : " + ctrlBlockInfo.type + "\n";
	toolTipStr += "apName : " + ctrlBlockInfo.apName + "\n";
	toolTipStr += "apAPPID : " + ctrlBlockInfo.apAPPID + "\n";
	toolTipStr += "apMAC : " + ctrlBlockInfo.apMAC + "\n";
	toolTipStr += "apVLAN_ID : " + ctrlBlockInfo.apVLAN_ID + "\n";
	toolTipStr += "apVLAN_PRIORITY : " + ctrlBlockInfo.apVLAN_PRIORITY + "\n";
	toolTipStr += "ASDU : " + QString::number(ctrlBlockInfo.ASDU) + "\n";
	toolTipStr += "Buffered : " + ctrlBlockInfo.Buffered + "\n";
	toolTipStr += "bufTime : " + ctrlBlockInfo.bufTime + "\n";
	toolTipStr += "confRev : " + ctrlBlockInfo.confRev + "\n";
	toolTipStr += "dataSetName : " + ctrlBlockInfo.dataSetName + "\n";
	toolTipStr += "dataSetAddr : " + ctrlBlockInfo.dataSetAddr + "\n";
	toolTipStr += "gocbRef : " + ctrlBlockInfo.gocbRef + "\n";
	toolTipStr += "intgPd : " + ctrlBlockInfo.intgPd + "\n";
	toolTipStr += "LDInst : " + ctrlBlockInfo.LDInst + "\n";
	toolTipStr += "smpRate : " + ctrlBlockInfo.smpRate + "\n";
	toolTipStr += "maxTime : " + ctrlBlockInfo.maxTime + "\n";
	toolTipStr += "minTime : " + ctrlBlockInfo.minTime;
	return toolTipStr;
}
