#include "DrawLoopGraphicWithDiff.h"
#include "IEDSlaveItem.h"
#include "VirLinkItem.h"
#include "CtrlBlockItem.h"

DrawLoopGraphicWithDiff::DrawLoopGraphicWithDiff(void)
{
}

DrawLoopGraphicWithDiff::DrawLoopGraphicWithDiff(HmiGraphicsScene *scene, const QString masterIEDKey, const QString slaveIEDKey, 
	const QString CtrlBlockKey, const QSignalTypeEnum sigType, int sigArrow)
{
	mainScene = scene;
	splitMasterSlaveKeys(masterIEDKey);
	m_CtrlBlockKey = slaveIEDKey;
	m_SigArrow = sigArrow;
	m_LastSigItem = NULL;
	m_LastSoftTripItem = NULL;
	isDifferGraphic();
	//mainScene->GetGraphicModel()->GetSignalInfosByIEDKeyAndCBKey(m_MasterIEDKey, m_SlaveIEDKey, m_CtrlBlockKey, &m_vtListOfMaster, &m_vtListOfSlave);
	//去连接的函数调用;
	mainScene->GetGraphicModel()->GetSignalInfosByIEDKeyAndCBKey(m_MasterIEDKey, m_SlaveIEDKey, m_CtrlBlockKey, &m_virLinks);
	//链接遍历,填充列表;
	FillDestSignalInfos();
}

DrawLoopGraphicWithDiff::~DrawLoopGraphicWithDiff(void)
{
}

void DrawLoopGraphicWithDiff::DrawIedHeaders()
{
	const int CtrlBlockWidth = 96;
	const int VirLinkLength = 213;

	const int SendCBXPos = 201;
	const int ReceiveCBXPos = 510;
	const int CBYPos = 40;
	const int VirLinkYPos = 50;

	mainScene->clear();

	QCtrlBlockInfo cbInfo;
	mainScene->GetGraphicModel()->GetCtrlBlockInfo(m_CtrlBlockKey, cbInfo);

	CtrlBlockItem *cbItem = new CtrlBlockItem(mainScene);
	if (cbInfo.type == "SampledValueControl")
	{
		cbItem->setSigType(svType);
		cbItem->setRegularClr(QColor(254,251,220));
		cbItem->setHighLightClr(QColor(253,217,0));
	}
	else if (cbInfo.type == "GSEControl")
	{
		cbItem->setSigType(gooseType);
		cbItem->setRegularClr(QColor(220,244,253));
		cbItem->setHighLightClr(QColor(253,217,0));
	}
	cbItem->setCBName(cbInfo.name);
	cbItem->setAPPID(cbInfo.APPID);
	cbItem->setWidth(CtrlBlockWidth);
	mainScene->addItem(cbItem);
	cbItem->setPos(SendCBXPos,CBYPos);

	CtrlBlockItem *inputCBItem = new CtrlBlockItem(mainScene);
	inputCBItem->setSigType(inputType);
	inputCBItem->setRegularClr(QColor(255,255,255));
	inputCBItem->setHighLightClr(QColor(253,217,0));
	inputCBItem->setCBName("接收");
	cbItem->setWidth(CtrlBlockWidth);
	mainScene->addItem(inputCBItem);
	inputCBItem->setPos(ReceiveCBXPos,CBYPos);

	CVirLinkItem *virLinkItem = new CVirLinkItem(mainScene);
	virLinkItem->setTypeLine(2);
	virLinkItem->setStartPt(QPoint(0,0));
	virLinkItem->setEndPt(QPoint(VirLinkLength,0));
	virLinkItem->setSrcItemID("Null");
	virLinkItem->setDestItemID("Null");
	mainScene->addItem(virLinkItem);
	virLinkItem->setPos(SendCBXPos + CtrlBlockWidth, VirLinkYPos);

	const int IEDHeight = 47 * m_vtListOfMaster.size() + 71;
	const int IEDWidth = 257;

	CIEDSlaveItem *srcIEDItem = new CIEDSlaveItem();
	CIEDSlaveItem *desIEDItem = new CIEDSlaveItem();

	srcIEDItem->setIedSlaveType(1);
	desIEDItem->setIedSlaveType(1);
	srcIEDItem->setWidth(IEDWidth);
	desIEDItem->setWidth(IEDWidth);
	srcIEDItem->setHeight(IEDHeight);
	desIEDItem->setHeight(IEDHeight);

	QIEDInfo srcInfo, desInfo;
	mainScene->GetGraphicModel()->GetIEDInfo(m_MasterIEDKey, srcInfo);
	mainScene->GetGraphicModel()->GetIEDInfo(m_SlaveIEDKey, desInfo);

	srcIEDItem->setMasterKey(m_MasterIEDKey);
	desIEDItem->setMasterKey(m_SlaveIEDKey);
	if (m_IsDiffGraphic)
	{
		srcIEDItem->setLinkType(link_type_diff_signal);
		desIEDItem->setLinkType(link_type_diff_signal);
	} 
	else
	{
		srcIEDItem->setLinkType(link_type_vterm);
		desIEDItem->setLinkType(link_type_vterm);
	}
	srcIEDItem->setIEDName(srcInfo.name);
	desIEDItem->setIEDName(desInfo.name);
	srcIEDItem->setIEDDesc(srcInfo.desc);
	desIEDItem->setIEDDesc(desInfo.desc);

	srcIEDItem->setZValue(1);
	desIEDItem->setZValue(1);

	mainScene->addItem(srcIEDItem);
	mainScene->addItem(desIEDItem);

	srcIEDItem->setPos(40,80);
	desIEDItem->setPos(510,80);
}

void DrawLoopGraphicWithDiff::DrawAllVloops()
{
	int initHeight = 80 + 40 + 30;
	int yPos = 0;
	QString tempName = "tempSigName";
	for (int i = 0; i < m_vtListOfMaster.size(); i++)
	{
		yPos = i * 47 + initHeight;
		QSignalInfo vtSrc = m_vtListOfMaster.at(i);
		QSignalInfo vtDest = m_vtListOfSlave.at(i);
		if (vtSrc.name == tempName)
		{
			DrawOneVloop(vtSrc,vtDest,yPos,i,true);
		}
		else
		{
			DrawOneVloop(vtSrc,vtDest,yPos,i);
		}
		tempName = vtSrc.name;
	}
	//last part of Draw() function in CDrawIEDAll class
	QRectF recScene = mainScene->sceneRect();
	recScene.setRight(770);
	recScene.setBottom(yPos + 80);
	mainScene->setSceneRect(recScene);
}

void DrawLoopGraphicWithDiff::DrawOneVloop(QSignalInfo srcVT, QSignalInfo descVT, int yPos, int linkIndex, bool isSame/* = false*/)
{
	if (isSame)
	{
		CVirSignalItem *desSigItem = new CVirSignalItem(mainScene);
		CVirLinkItem *vLinkItem = new CVirLinkItem(mainScene);
		vLinkItem->setTypeLine(2);
		vLinkItem->setStartPt(QPoint(286, yPos));
		vLinkItem->setEndPt(QPoint(522, yPos));
		vLinkItem->setPos(286, yPos+18);
		if (descVT.desc == "" && !m_IsDiffGraphic)
		{
			desSigItem->setVirSignalName(m_virLinks.at(linkIndex).inSigInfo);
			desSigItem->setDescEmpty(true);
		}
		else
		{
			desSigItem->setVirSignalName(descVT.desc);
		}
		m_LastSigItem->setHeight(m_LastSigItem->getHeight() + 47);
		vLinkItem->setChangeType((ItemChangeType)m_virLinks.at(linkIndex).DiffState);
		desSigItem->setChangeType((ItemChangeType)descVT.DiffState);
		vLinkItem->setToolTipPos(286, yPos+18);
		desSigItem->setHeight(37);
		desSigItem->setWidth(234);
		desSigItem->setRightImage(true);
		desSigItem->setItemID(GetItemIDStr());
		mainScene->addItem(desSigItem);
		vLinkItem->setItemID(GetItemIDStr());
		mainScene->addItem(vLinkItem);

		m_LastSigItem->appendAssoID(desSigItem->getItemID());
		m_LastSigItem->appendAssoID(vLinkItem->getItemID());
		desSigItem->appendAssoID(m_LastSigItem->getItemID());
		desSigItem->appendAssoID(vLinkItem->getItemID());
		vLinkItem->setSrcItemID(m_LastSigItem->getItemID());
		vLinkItem->setDestItemID(desSigItem->getItemID());

		desSigItem->setPos(522 , yPos);
		desSigItem->setToolTipPos(522, yPos);
		if (descVT.desc == "" && !m_IsDiffGraphic)
		{
			desSigItem->setDiffInfo("该输入信号未正确配置!!");
		} 
		else
		{
			desSigItem->setDiffInfo(descVT.diffDesc);
		}

		//更新软压板位置;
		if (m_LastSoftTripItem != NULL)
		{
			int tempSTYPos = m_LastSigItem->pos().y() + m_LastSigItem->getHeight()/2 - 5;
			m_LastSoftTripItem->setPos(72, tempSTYPos);
		}
	} 
	else		//Not same;
	{
		CVirSignalItem *srcSigItem = new CVirSignalItem(mainScene);
		CVirSignalItem *desSigItem = new CVirSignalItem(mainScene);
		CVirLinkItem *vLinkItem = new CVirLinkItem(mainScene);
		m_LastSigItem = srcSigItem;
		vLinkItem->setTypeLine(2);
		vLinkItem->setStartPt(QPoint(286, yPos));
		vLinkItem->setEndPt(QPoint(522, yPos));
		vLinkItem->setPos(286, yPos+18);
		srcSigItem->setVirSignalName(srcVT.desc);
		if (descVT.desc == "" && !m_IsDiffGraphic)
		{
			desSigItem->setVirSignalName(m_virLinks.at(linkIndex).inSigInfo);
            // desSigItem->SetSignalOrd(m_virLinks.at(linkIndex).iOrd);
			desSigItem->setDescEmpty(true);
		}
		else
		{
			desSigItem->setVirSignalName(descVT.desc);
            // desSigItem->SetSignalOrd(descVT.iOutSigOrd);
		}
		srcSigItem->setChangeType((ItemChangeType)srcVT.DiffState);
		desSigItem->setChangeType((ItemChangeType)descVT.DiffState);
		vLinkItem->setChangeType((ItemChangeType)m_virLinks.at(linkIndex).DiffState);
		vLinkItem->setToolTipPos(286, yPos+18);
		srcSigItem->setHeight(37);
		desSigItem->setHeight(37);
		srcSigItem->setWidth(234);
		desSigItem->setWidth(234);
		srcSigItem->setRightImage(false);
		desSigItem->setRightImage(true);
		srcSigItem->setItemID(GetItemIDStr());
		mainScene->addItem(srcSigItem);
		desSigItem->setItemID(GetItemIDStr());
		mainScene->addItem(desSigItem);
		vLinkItem->setItemID(GetItemIDStr());
		mainScene->addItem(vLinkItem);

		srcSigItem->appendAssoID(desSigItem->getItemID());
		srcSigItem->appendAssoID(vLinkItem->getItemID());
		desSigItem->appendAssoID(srcSigItem->getItemID());
		desSigItem->appendAssoID(vLinkItem->getItemID());
		vLinkItem->setSrcItemID(srcSigItem->getItemID());
		vLinkItem->setDestItemID(desSigItem->getItemID());

		srcSigItem->setPos(52, yPos);
		srcSigItem->setToolTipPos(52, yPos);
		srcSigItem->setDiffInfo(srcVT.diffDesc);
		desSigItem->setPos(522 , yPos);
		desSigItem->setToolTipPos(522, yPos);
		if (descVT.desc.compare("") == 0 && !m_IsDiffGraphic)
		{
			desSigItem->setDiffInfo("该输入信号未正确配置!!");
		} 
		else
		{
			desSigItem->setDiffInfo(descVT.diffDesc);
		}

		//绘制软压板
		QString tripSrcKey = srcVT.softTripKey;
		QString tripDestKey = descVT.softTripKey;
		if (tripSrcKey.length() > 0 || tripDestKey.length() > 0)
		{
			CSoftTripItem *tripItem = new CSoftTripItem(NULL,0,mainScene);
			QString tripName = mainScene->GetSclModel()->GetNameFromKey(tripSrcKey);
			tripItem->SetKeyValue(tripName);
			m_LastSoftTripItem = tripItem;
			mainScene->addItem(tripItem);
			tripItem->setPos(QPointF(72, yPos + 13));
		}
	}
}

QString DrawLoopGraphicWithDiff::GetItemIDStr()
{
	QList<QGraphicsItem*> iList = mainScene->items();
	return QString::number(iList.size());
}

void DrawLoopGraphicWithDiff::splitMasterSlaveKeys(QString msKeys)
{
	QList<QString> mAndsKey = msKeys.split("$");
	m_MasterIEDKey = mAndsKey.at(0);
	m_SlaveIEDKey = mAndsKey.at(1);
}

void DrawLoopGraphicWithDiff::FillDestSignalInfos()
{
	for (QVector<QVirtualLink>::Iterator j = m_virLinks.begin();j != m_virLinks.end(); ++j)
	{
		QVirtualLink a_vlink = *j;
		QString strSrcSignal = a_vlink.srcSignal;
		QString strDesSignal = a_vlink.destSignal;

		QSignalInfo srcSigInfo, desSigInfo;
		mainScene->GetGraphicModel()->GetInfoOfOutSignal(strSrcSignal,&srcSigInfo);
		mainScene->GetGraphicModel()->GetInfoOfInSignal(strDesSignal,&desSigInfo);
		m_vtListOfMaster.append(srcSigInfo);
		m_vtListOfSlave.append(desSigInfo);
	}

	//排序;
	int count = m_vtListOfMaster.size();
	if (count < 1)
	{
		return ;
	}
	int index = 0;
	QList<QVirtualLink> tempList = m_virLinks.toList();
	for (int i = 0; i < count - 1; i++)
	{
		index = i;
		int sOrd = m_vtListOfMaster.at(i).iOrd;
		for (int j = i + 1; j < count; j++)
		{
			int dOrd = m_vtListOfMaster.at(j).iOrd;
			if (sOrd > dOrd)
			{
				index = j;
				sOrd = dOrd;
			}
		}
		if (index != i)
		{
			m_vtListOfMaster.swap(i,index);
			m_vtListOfSlave.swap(i,index);
			tempList.swap(i,index);
		}
	}
	m_virLinks.clear();
	m_virLinks = tempList.toVector();
}

void DrawLoopGraphicWithDiff::isDifferGraphic()
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