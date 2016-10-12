#include "DrawTerminalGraphicWithDiff.h"
#include "IEDSlaveItem.h"
#include "PhyLinkVirLinkItem.h"
#include "VirSignalItem.h"
#include "SCLNameTranslator.h"
#include "csofttripitem.h"
#include "CtrlBlockItem.h"

DrawTerminalGraphicWithDiff::DrawTerminalGraphicWithDiff(void)
{
    m_ShowVirLinkOnly = false;
}

DrawTerminalGraphicWithDiff::DrawTerminalGraphicWithDiff(HmiGraphicsScene *scene, const QString IEDKey)
{
	mainScene = scene;
	m_IEDKey = IEDKey;
	m_nSpace = 5;
	m_PhyPorts.clear();
	m_PortItems.clear();

    ShowOutSignalInTerminalGraphic();

	isDifferGraphic();
	mainScene->GetGraphicModel()->GetSignalInfosByIEDKey(IEDKey, &m_virLinks);
    FillVLinksToList();
    // add by zxc
    if (!m_IsDiffGraphic) // ��Ϊ���ӻ�״̬��ֻ���������·,��ȡ������ź�
    {
        // ȥ�������·
        SCLNameTranslator translator;
        for (int i = 0; i < m_virLinks.size(); i++)
        {
            QString srcIED = translator.GetParentKey(m_virLinks.at(i).srcSignal.toLocal8Bit().constData()).c_str();
            if (srcIED == m_IEDKey)
            {
                m_SendSrcList.push_back(m_vtList[i]);
                m_SendDestList.push_back(m_vtDestList[i]);
            }
            else
            {
                m_ReceiveSrcList.push_back(m_vtList[i]);
                m_ReceiveDestList.push_back(m_vtDestList[i]);
            }
        }

        // ��ȡ���ƿ���Ϣ���������ƿ��ڵ��ź���Ϣ����
        mainScene->GetGraphicModel()->GetCtrlBlockListByIED(IEDKey, &m_CBList);
        m_iOutSignalCount = 0;
        for (int ii = 0; ii < m_CBList.size(); ii++)
        {
            QCtrlBlockInfo tempBlock;
            mainScene->GetGraphicModel()->GetCtrlBlockInfo(m_CBList.at(ii), tempBlock);
            if (tempBlock.type != "SampledValueControl" && tempBlock.type != "GSEControl")
            {
                continue;
            }
            QVector<QString> outSignalList;
            mainScene->GetGraphicModel()->GetOutSignalsByCBKey(IEDKey, m_CBList.at(ii), &outSignalList);

            // ��ȡ�ź���Ϣ
            QList<QOutSignalInfo> outSignalInfoList;
            
            for (int i = 0; i < outSignalList.size(); i++)
            {
                QSignalInfo tempSignal;
                QOutSignalInfo temp;
                mainScene->GetGraphicModel()->GetInfoOfOutSignal(outSignalList.at(i), &tempSignal);
                temp.key = tempSignal.ID;
                temp.path = tempSignal.name;
                temp.desc = tempSignal.DOIDesc;
                temp.iOutSignalOrd = tempSignal.iOutSigOrd;
                temp.softTripKey = tempSignal.softTripKey;

                // �ж�����ź��Ƿ��л�·
                int vtCount = 0;
                for (int j = 0; j < m_SendSrcList.size(); j++)
                {
                    if (m_SendSrcList.at(j).name == temp.path)
                    {
                        temp.outList.push_back(j);
                        vtCount++;
                    }
                }
                if (vtCount > 1)
                {
                    m_iOutSignalCount = m_iOutSignalCount + vtCount - 1;
                }
                
                outSignalInfoList.push_back(temp);
                m_iOutSignalCount++;
            }
            
            // �Կ��ƿ��ڵ�����źŽ�������
            int index = 0;
            for (int i = 0; i < outSignalInfoList.size() - 1; i++)
            {
                index = i;
                int sOrd = outSignalInfoList.at(i).iOutSignalOrd;
                for (int j = i + 1; j < outSignalInfoList.size(); j++)
                {
                    int dOrd = outSignalInfoList.at(j).iOutSignalOrd;
                    if (sOrd > dOrd)
                    {
                        index = j;
                        sOrd = dOrd;
                    }
                }
                if (index != i)
                {
                    outSignalInfoList.swap(i,index);
                }
            }
            m_OutSignalMap[m_CBList.at(ii)] = outSignalInfoList.toVector();
        }
    }
    	
	mainScene->GetGraphicModel()->GetPhyPortListByIED(IEDKey, &m_PhyPorts);
	PhyPortFilter();

    // ����IEDKey��ȡȫվ�����Ϣ
    if (IEDKey.split(".").size() == 4)
    {
        QString subStationKey = IEDKey.left(IEDKey.lastIndexOf('.'));
        interval = new IntervalCreator(subStationKey);
        interval->GetIEDInterval();
    }

}

DrawTerminalGraphicWithDiff::~DrawTerminalGraphicWithDiff(void)
{
}

void DrawTerminalGraphicWithDiff::Draw()
{
    if (m_IsDiffGraphic)
    {
        DrawDiffTerminalGraphic();
    }
    else
    {
        DrawNormalTerminalGraphic();
    }
}

void DrawTerminalGraphicWithDiff::DrawNormalTerminalGraphic()
{
    mainScene->clear();
    SCLNameTranslator translator;

    bool isPhyListNull = false;
    if (m_PhyPorts.size() == 0)
    {
        isPhyListNull = true;
    }

    int IEDHeight = 31 * m_ReceiveSrcList.size() + 60 + m_iOutSignalCount * 31;
    if (m_ShowVirLinkOnly)
    {
        IEDHeight = 31 * m_ReceiveSrcList.size() + 60 + m_SendSrcList.size() * 31;
    }

    int IEDWidth = 395 + 40 + 40; // ������+40 �����ѹ����+40
    CIEDSlaveItem *iedItem = new CIEDSlaveItem(mainScene);
    iedItem->setMasterKey(m_IEDKey);
    iedItem->setLinkType(link_type_msgflow);
    iedItem->setIedSlaveType(2);
    iedItem->setWidth(IEDWidth);
    iedItem->setHeight(IEDHeight);
    QIEDInfo iedInfo;
    mainScene->GetGraphicModel()->GetIEDInfo(m_IEDKey, iedInfo);
    iedItem->setIEDDesc(iedInfo.desc);
    iedItem->setZValue(1);
    iedItem->setItemID(GetItemIDStr());
    mainScene->addItem(iedItem);
    iedItem->setPos(0,40); // (40, 40)->(10, 40);

    const int INITPORT_X = 913 + 40; // +50 �����ѹ����+40
    int portXPos = INITPORT_X;
    int initPortY = /*IEDHeight + */40;
    for (int pIt = 0; pIt != m_PhyPorts.size(); pIt++)
    {
        portXPos += 63;
        QPhyPortInfo portInfo;
        mainScene->GetGraphicModel()->GetPhyPortInfo(m_PhyPorts.at(pIt), portInfo);
        CPhyPortItem *portItem = new CPhyPortItem(mainScene);
        portItem->setPortName(portInfo.pIndex);
        portItem->setPortType(portInfo.type);
        portItem->setWidth(63);
        portItem->setHeight(26);
        portItem->setVerticalSize(IEDHeight - 40);
        portItem->setChangeType((ItemChangeType)portInfo.DiffState);
        portItem->setDiffInfo(portInfo.diffDesc);
        portItem->setItemID(GetItemIDStr());
        mainScene->addItem(portItem);
        portItem->setPos(portXPos, initPortY);
        portItem->setToolTipPos(portXPos, initPortY);
        portItem->setDiffInfo(portInfo.diffDesc);
        m_PortItems.insert(portInfo.pIndex, portItem);
        portXPos += 20;
    }

    int initTop = 40 + 21 + 27;
    int yPos = initTop;
    QString tempName = "tempSigName";
    CVirSignalItem *lastSigItem = NULL;
    CSoftTripItem *lastSoftTripItem = NULL;
    int phyLinkItemRightPos = 0;

    // ����������ƿ�
    int cbxPos = 10;
    int cbWidth = 42;
    int cbyPos = initTop;
    CtrlBlockItem *pReceiveCtrlBlock = new CtrlBlockItem(mainScene);
    pReceiveCtrlBlock->setWidth(cbWidth);
    pReceiveCtrlBlock->setSigType(inputType);
    pReceiveCtrlBlock->setRegularClr(QColor(255, 255, 255));
    pReceiveCtrlBlock->setHighLightClr(QColor(253,217,0));
    pReceiveCtrlBlock->setCBName("����");
    pReceiveCtrlBlock->setPos(cbxPos, cbyPos);

    // ���������·
    int iVirSignalItemWidth = 351 + 40; // �����ѹ����+40
    int iVirLinkPos = 413 + 40; //363->413 �����ѹ����+40
    for (int i = 0; i != m_ReceiveSrcList.size(); i++)
    {
        bool isSame = false;
        if (m_ReceiveSrcList.at(i).name == tempName)
        {
            isSame = true;
        }

        yPos = initTop + 31 * i;
        if (isSame)
        {
            lastSigItem->setHeight(lastSigItem->getHeight() + 31);
        }
        else
        {
            CVirSignalItem *signalItem = new CVirSignalItem(mainScene);
            lastSigItem = signalItem;
            tempName = m_ReceiveSrcList.at(i).name;
            lastSigItem->setHeight(26);
            lastSigItem->setWidth(iVirSignalItemWidth);
            lastSigItem->setVirSignalName(m_ReceiveSrcList.at(i).desc);
            // չʾ�ź�·����Ord
            lastSigItem->SetSignalPath(m_ReceiveSrcList.at(i).path);
            // lastSigItem->SetSignalOrd(m_ReceiveSrcList.at(i).iOutSigOrd);
            lastSigItem->setChangeType((ItemChangeType)m_ReceiveSrcList.at(i).DiffState);
            lastSigItem->setDiffInfo(m_ReceiveSrcList.at(i).diffDesc);
            lastSigItem->setItemID(GetItemIDStr());
            mainScene->addItem(lastSigItem);
            lastSigItem->setPos(62, yPos);
            lastSigItem->setToolTipPos(62, yPos);
            lastSigItem->setDiffInfo(m_vtList.at(i).diffDesc);

            // Ϊ�����ź����ÿ��ƿ����
            pReceiveCtrlBlock->appendAssoID(lastSigItem->getItemID());
        }

        CPhyLinkVirLinkItem *phyLinkItem = NULL;
        QString sigIEDKey;
        sigIEDKey = translator.GetParentKey(m_ReceiveSrcList.at(i).name.toLocal8Bit().constData()).c_str();
        VTermType termType;
        if (sigIEDKey == m_IEDKey)
        {
            termType = term_output;
        }
        else
        {
            termType = term_input;
        }

        QVector<int> xPos = GetPortXPos(m_ReceiveSrcList.at(i).ID, termType);
        QVector<CPhyPortItem*> phyPorts = GetPortItem(m_ReceiveSrcList.at(i).ID, termType);
        if (termType == term_input)
        {
            phyLinkItem = new CPhyLinkVirLinkItem(mainScene,QPointF(portXPos - 321,0),QPointF(0,0));
        }
        else if (termType == term_output)
        {
            phyLinkItem = new CPhyLinkVirLinkItem(mainScene,QPointF(0,0),QPointF(portXPos - 321,0));
        }
        phyLinkItem->setportPosVec(xPos);

        QString tempSigIEDKey;
        QIEDInfo tempIedInfo;
        tempSigIEDKey = translator.GetParentKey(m_ReceiveDestList.at(i).ID.toLocal8Bit().constData()).c_str();

        if (!m_IsDiffGraphic)
        {
            if (!IsVirLinkInSameInterval(m_ReceiveSrcList.at(i).ID, m_ReceiveDestList.at(i).ID))
            {
                phyLinkItem->SetDestIsInSameInterval(false);
            }
        }
        mainScene->GetGraphicModel()->GetIEDInfo(tempSigIEDKey, tempIedInfo);
        phyLinkItem->setDestDesc(tempIedInfo.desc + "  " + m_ReceiveDestList.at(i).desc);
        if (phyLinkItem->getRealWidth() + iVirLinkPos > phyLinkItemRightPos)
        {
            phyLinkItemRightPos = phyLinkItem->getRealWidth() + iVirLinkPos;
        }

        if (lastSigItem->getChangeType() != NoChange)
        {
            phyLinkItem->setChangeType((ItemChangeType)lastSigItem->getChangeType());
        }

        //������ѹ��;
        if (isSame)
        {
            if (lastSoftTripItem != NULL)
            {
                int stYPos = lastSigItem->pos().y() + lastSigItem->getHeight()/2 - 5;
                lastSoftTripItem->setPos(82, stYPos);
            }
        }
        else
        {
            QString tripKey = m_vtList.at(i).softTripKey;
            if (tripKey.length() > 0)
            {
                //������ѹ��
                CSoftTripItem *tripItem = new CSoftTripItem(NULL,0,mainScene);
                QString tripName = mainScene->GetSclModel()->GetNameFromKey(tripKey);
                tripItem->SetKeyValue(tripName);
                lastSoftTripItem = tripItem;
                mainScene->addItem(tripItem);
                //tripItem->setPos(QPointF(401, yPos + 5));
                tripItem->setPos(QPointF(82, yPos + 8));
            }
        }

        phyLinkItem->setItemID(GetItemIDStr());
        mainScene->addItem(phyLinkItem);
        phyLinkItem->setPos(iVirLinkPos, yPos+11);

        //��ӹ���;
        lastSigItem->appendAssoID(phyLinkItem->getItemID());
        phyLinkItem->setSrcItemID(lastSigItem->getItemID());
        CPhyPortItem *phyPort = NULL;
        for (uint pIndex = 0; pIndex != phyPorts.size(); pIndex++)
        {
            phyPort = phyPorts.at(pIndex);
            lastSigItem->appendAssoID(phyPort->getItemID());
            phyLinkItem->appendDestItemID(phyPort->getItemID());
            phyPort->appendAssoID(lastSigItem->getItemID());
            phyPort->appendAssoID(phyLinkItem->getItemID());
        }
    }

    // Ϊ������ƿ����ø߶�
    if (m_ReceiveSrcList.size() > 0)
    {
        mainScene->addItem(pReceiveCtrlBlock);
        int cbHeight = yPos + 26 - initTop;
        pReceiveCtrlBlock->SetHeight(cbHeight);
    }


    if (!m_IsDiffGraphic)
    {
        QMap<QString, QVector<QOutSignalInfo>>::iterator it = m_OutSignalMap.begin();
        int initYPos = yPos;

        int lastItemHight = 0;
        if (m_ReceiveSrcList.size() > 0)
        {
            lastItemHight = 31;
        }

        for (; it != m_OutSignalMap.end(); it++)
        {
            // ��ӿ��ƿ�
            // ����������ƿ�
            int cbxPos = 10;
            int cbWidth = 42;
            int cbyPos = yPos + lastItemHight;
            CtrlBlockItem *pSendCtrlBlock = new CtrlBlockItem(mainScene);
            pSendCtrlBlock->setWidth(cbWidth);

            QCtrlBlockInfo tempBlock;
            mainScene->GetGraphicModel()->GetCtrlBlockInfo(it.key(), tempBlock);

            if (tempBlock.type == "GSEControl")
            {
                pSendCtrlBlock->setSigType(gooseType);
            }
            else 
            {
                pSendCtrlBlock->setSigType(svType);
            }

            pSendCtrlBlock->setAPPID(tempBlock.APPID);
            pSendCtrlBlock->setCBName(tempBlock.name);
            pSendCtrlBlock->setRegularClr(QColor(255, 255, 255));
            pSendCtrlBlock->setHighLightClr(QColor(253,217,0));
            mainScene->addItem(pSendCtrlBlock);
            pSendCtrlBlock->setPos(cbxPos, cbyPos);

            // �������ź�
            QVector<QOutSignalInfo> tempVector = it.value();

            for (int i = 0; i < tempVector.size(); i++)
            {
                int outCount = tempVector.at(i).outList.size();
                if (m_ShowVirLinkOnly && outCount < 1)
                {
                    continue;
                }
                yPos = yPos + lastItemHight;

                CVirSignalItem *signalItem = new CVirSignalItem(mainScene);
                lastSigItem = signalItem;

                int heigth = 26;
                lastItemHight = 31;
                if (outCount > 1)
                {
                    heigth = heigth + 31 * (outCount - 1);
                    lastItemHight = 31 * outCount;
                }

                lastSigItem->setHeight(heigth);
                lastSigItem->setWidth(iVirSignalItemWidth);
                lastSigItem->setVirSignalName(tempVector.at(i).desc);
                lastSigItem->SetSignalPath(tempVector.at(i).path);
                lastSigItem->SetSignalOrd(tempVector.at(i).iOutSignalOrd);
                lastSigItem->setItemID(GetItemIDStr());
                mainScene->addItem(lastSigItem);
                lastSigItem->setPos(62, yPos);
                lastSigItem->setToolTipPos(62, yPos);

                // Ϊ����ź���ӿ��ƿ����
                pSendCtrlBlock->appendAssoID(lastSigItem->getItemID());


                // ������ѹ��;
                QString tripKey = tempVector.at(i).softTripKey;
                if (tripKey.length() > 0)
                {
                    //������ѹ��
                    CSoftTripItem *tripItem = new CSoftTripItem(NULL,0,mainScene);
                    QString tripName = mainScene->GetSclModel()->GetNameFromKey(tripKey);
                    tripItem->SetKeyValue(tripName);
                    mainScene->addItem(tripItem);
                    int stYPos = lastSigItem->pos().y() + lastSigItem->getHeight()/2 - 5;
                    tripItem->setPos(QPointF(97, stYPos));
                }

                if (outCount >= 1)
                {
                    for (int jj = 0; jj < outCount; jj++)
                    {
                        CPhyLinkVirLinkItem *phyLinkItem = NULL;
                        int id = tempVector.at(i).outList.at(jj);
                        QString sigIEDKey = translator.GetParentKey(m_SendSrcList.at(id).name.toLocal8Bit().constData()).c_str();

                        VTermType termType = term_output;

                        QVector<int> xPos = GetPortXPos(m_SendSrcList.at(id).ID, termType);
                        QVector<CPhyPortItem*> phyPorts = GetPortItem(m_SendSrcList.at(id).ID, termType);
                        phyLinkItem = new CPhyLinkVirLinkItem(mainScene,QPointF(0,0),QPointF(portXPos - 321,0));
                        phyLinkItem->setportPosVec(xPos);

                        QString tempSigIEDKey;
                        QIEDInfo tempIedInfo;
                        tempSigIEDKey = translator.GetParentKey(m_SendDestList.at(id).ID.toLocal8Bit().constData()).c_str();

                        mainScene->GetGraphicModel()->GetIEDInfo(tempSigIEDKey, tempIedInfo);
                        phyLinkItem->setDestDesc(tempIedInfo.desc + "  " + m_SendDestList.at(id).desc);
                        if (phyLinkItem->getRealWidth() + iVirLinkPos > phyLinkItemRightPos)
                        {
                            phyLinkItemRightPos = phyLinkItem->getRealWidth() + iVirLinkPos;
                        }

                        if (!IsVirLinkInSameInterval(m_SendSrcList.at(id).ID, m_SendDestList.at(id).ID))
                        {
                            phyLinkItem->SetDestIsInSameInterval(false);
                        }

                        if (lastSigItem->getChangeType() != NoChange)
                        {
                            phyLinkItem->setChangeType((ItemChangeType)lastSigItem->getChangeType());
                        }

                        phyLinkItem->setItemID(GetItemIDStr());
                        mainScene->addItem(phyLinkItem);
                        phyLinkItem->setPos(iVirLinkPos, yPos + 11 + 31*jj);

                        //��ӹ���;
                        lastSigItem->appendAssoID(phyLinkItem->getItemID());
                        phyLinkItem->setSrcItemID(lastSigItem->getItemID());
                        CPhyPortItem *phyPort = NULL;
                        for (uint pIndex = 0; pIndex != phyPorts.size(); pIndex++)
                        {
                            phyPort = phyPorts.at(pIndex);
                            lastSigItem->appendAssoID(phyPort->getItemID());
                            phyLinkItem->appendDestItemID(phyPort->getItemID());
                            phyPort->appendAssoID(lastSigItem->getItemID());
                            phyPort->appendAssoID(phyLinkItem->getItemID());
                        }
                    }

                }
            }

            // Ϊ������ƿ����ø߶�
            int cbHeight = yPos + lastItemHight - cbyPos - 5;
            pSendCtrlBlock->SetHeight(cbHeight);
            if (cbHeight < 26)
            {
                mainScene->removeItem(pSendCtrlBlock);
            }
        }
    }


    QRectF recScene = mainScene->sceneRect();
    if (portXPos == 0)
    {
        recScene.setRight(870);
    }
    else
    {
        recScene.setRight(80 + portXPos);
    }
    if (isPhyListNull)
    {
        recScene.setBottom(yPos + 80);
    }
    else
    {
        recScene.setBottom(yPos + 200);
    }
    mainScene->setSceneRect(recScene);
}

void DrawTerminalGraphicWithDiff::DrawDiffTerminalGraphic()
{
    mainScene->clear();
    SCLNameTranslator translator;

    bool isPhyListNull = false;
    if (m_PhyPorts.size() == 0)
    {
        isPhyListNull = true;
    }

    int IEDHeight = 31 * m_vtList.size() + 60;
    int IEDWidth = 345;
    CIEDSlaveItem *iedItem = new CIEDSlaveItem(mainScene);
    iedItem->setMasterKey(m_IEDKey);
    iedItem->setLinkType(link_type_diff_msgflow);
    iedItem->setIedSlaveType(2);
    iedItem->setWidth(IEDWidth);
    iedItem->setHeight(IEDHeight);
    QIEDInfo iedInfo;
    mainScene->GetGraphicModel()->GetIEDInfo(m_IEDKey, iedInfo);
    iedItem->setIEDDesc(iedInfo.desc);
    iedItem->setZValue(1);
    iedItem->setItemID(GetItemIDStr());
    mainScene->addItem(iedItem);
    iedItem->setPos(40,40);

    const int INITPORT_X = 863;
    int portXPos = INITPORT_X;
    int initPortY = /*IEDHeight + */40;
    for (int pIt = 0; pIt != m_PhyPorts.size(); pIt++)
    {
        portXPos += 63;
        QPhyPortInfo portInfo;
        mainScene->GetGraphicModel()->GetPhyPortInfo(m_PhyPorts.at(pIt), portInfo);
        CPhyPortItem *portItem = new CPhyPortItem(mainScene);
        portItem->setPortName(portInfo.pIndex);
        portItem->setPortType(portInfo.type);
        portItem->setWidth(63);
        portItem->setHeight(26);
        portItem->setVerticalSize(IEDHeight - 40);
        portItem->setChangeType((ItemChangeType)portInfo.DiffState);
        portItem->setDiffInfo(portInfo.diffDesc);
        portItem->setItemID(GetItemIDStr());
        mainScene->addItem(portItem);
        portItem->setPos(portXPos, initPortY);
        portItem->setToolTipPos(portXPos, initPortY);
        portItem->setDiffInfo(portInfo.diffDesc);
        m_PortItems.insert(portInfo.pIndex, portItem);
        portXPos += 20;
    }

    int initTop = 40 + 21 + 27;
    int yPos = 0;
    QString tempName = "tempSigName";
    CVirSignalItem *lastSigItem = NULL;
    CSoftTripItem *lastSoftTripItem = NULL;
    int phyLinkItemRightPos = 0;
    for (int i = 0; i != m_vtList.size(); i++)
    {
        bool isSame = false;
        if (m_vtList.at(i).name == tempName)
        {
            isSame = true;
        }

        yPos = initTop + 31 * i;
        if (isSame)
        {
            lastSigItem->setHeight(lastSigItem->getHeight() + 31);
        }
        else
        {
            CVirSignalItem *signalItem = new CVirSignalItem(mainScene);
            lastSigItem = signalItem;
            tempName = m_vtList.at(i).name;
            lastSigItem->setHeight(26);
            lastSigItem->setWidth(301);
            lastSigItem->setVirSignalName(m_vtList.at(i).desc);
            lastSigItem->setChangeType((ItemChangeType)m_vtList.at(i).DiffState);
            lastSigItem->setDiffInfo(m_vtList.at(i).diffDesc);
            lastSigItem->setItemID(GetItemIDStr());
            mainScene->addItem(lastSigItem);
            lastSigItem->setPos(62, yPos);
            lastSigItem->setToolTipPos(62, yPos);
            lastSigItem->setDiffInfo(m_vtList.at(i).diffDesc);
        }

        CPhyLinkVirLinkItem *phyLinkItem = NULL;
        QString sigIEDKey;
        sigIEDKey = getIEDNameInDiff(m_virLinks.at(i).srcSignal);

        VTermType termType;
        if (sigIEDKey == m_IEDKey)
        {
            termType = term_output;
        }
        else
        {
            termType = term_input;
        }

        QVector<int> xPos = GetPortXPos(m_vtList.at(i).ID, termType);
        QVector<CPhyPortItem*> phyPorts = GetPortItem(m_vtList.at(i).ID, termType);
        if (termType == term_input)
        {
            phyLinkItem = new CPhyLinkVirLinkItem(mainScene,QPointF(portXPos - 321,0),QPointF(0,0));
        }
        else if (termType == term_output)
        {
            phyLinkItem = new CPhyLinkVirLinkItem(mainScene,QPointF(0,0),QPointF(portXPos - 321,0));
        }
        phyLinkItem->setportPosVec(xPos);

        QString tempSigIEDKey;
        QIEDInfo tempIedInfo;
        tempSigIEDKey = getIEDNameInDiff(m_vtDestList.at(i).ID);
        mainScene->GetGraphicModel()->GetIEDInfo(tempSigIEDKey, tempIedInfo);
        phyLinkItem->setDestDesc(tempIedInfo.desc + "  " + m_vtDestList.at(i).desc);
        if (phyLinkItem->getRealWidth() + 363 > phyLinkItemRightPos)
        {
            phyLinkItemRightPos = phyLinkItem->getRealWidth() + 363;
        }

        if (lastSigItem->getChangeType() != NoChange)
        {
            phyLinkItem->setChangeType((ItemChangeType)lastSigItem->getChangeType());
        }

        //������ѹ��;
        if (isSame)
        {
            if (lastSoftTripItem != NULL)
            {
                int stYPos = lastSigItem->pos().y() + lastSigItem->getHeight()/2 - 5;
                lastSoftTripItem->setPos(82, stYPos);
            }
        }
        else
        {
            QString tripKey = m_vtList.at(i).softTripKey;
            if (tripKey.length() > 0)
            {
                //������ѹ��
                CSoftTripItem *tripItem = new CSoftTripItem(NULL,0,mainScene);
                QString tripName = mainScene->GetSclModel()->GetNameFromKey(tripKey);
                tripItem->SetKeyValue(tripName);
                lastSoftTripItem = tripItem;
                mainScene->addItem(tripItem);
                //tripItem->setPos(QPointF(401, yPos + 5));
                tripItem->setPos(QPointF(82, yPos + 8));
            }
        }

        phyLinkItem->setItemID(GetItemIDStr());
        mainScene->addItem(phyLinkItem);
        phyLinkItem->setPos(363, yPos+11);

        //��ӹ���;
        lastSigItem->appendAssoID(phyLinkItem->getItemID());
        phyLinkItem->setSrcItemID(lastSigItem->getItemID());
        CPhyPortItem *phyPort = NULL;
        for (uint pIndex = 0; pIndex != phyPorts.size(); pIndex++)
        {
            phyPort = phyPorts.at(pIndex);
            lastSigItem->appendAssoID(phyPort->getItemID());
            phyLinkItem->appendDestItemID(phyPort->getItemID());
            phyPort->appendAssoID(lastSigItem->getItemID());
            phyPort->appendAssoID(phyLinkItem->getItemID());
        }
    }

    QRectF recScene = mainScene->sceneRect();
    if (portXPos == 0)
    {
        recScene.setRight(870);
    }
    else
    {
        recScene.setRight(80 + portXPos);
    }
    if (isPhyListNull)
    {
        recScene.setBottom(yPos + 80);
    }
    else
    {
        recScene.setBottom(yPos + 200);
    }
    mainScene->setSceneRect(recScene);
}

QVector<int> DrawTerminalGraphicWithDiff::GetPortXPos(QString sigKey, VTermType termType)
{
	QVector<int> portPosVec;
	portPosVec.clear();
	if (m_PhyPorts.size() == 0)
	{
		return portPosVec;
	}
	QString pPortIndex;
	QVector<QString> pPortList;
	if (termType == term_output)
	{
		mainScene->GetGraphicModel()->GetPhyPortKeyByOutSignalID(sigKey, &pPortIndex);
	}
	else if (termType == term_input)
	{
		mainScene->GetGraphicModel()->GetPhyPortListByInSignalID(sigKey, pPortList);
	}
	CPhyPortItem *p = NULL;
	for (uint i = 0; i != pPortList.size(); i++)
	{
		p = m_PortItems.value(pPortList.at(i));
		if (p == NULL)
		{
			continue;
		}
		else
		{
			QPointF pt = p->pos();
            if (m_IsDiffGraphic)
            {
                portPosVec.append(pt.x() - 350);
            }
            else
            {
			    portPosVec.append(pt.x() - 440); // ����λ��350->400
            }
            
		}
	}
	
	return portPosVec;
}

QVector<CPhyPortItem*> DrawTerminalGraphicWithDiff::GetPortItem(QString sigKey,  VTermType termType)
{
	QVector<CPhyPortItem*> portItemVec;
	portItemVec.clear();
	if (m_PhyPorts.size() == 0)
	{
		return portItemVec;
	}
	QString pPortIndex;
	QVector<QString> pPortList;
	if (termType == term_output)
	{
		mainScene->GetGraphicModel()->GetPhyPortKeyByOutSignalID(sigKey, &pPortIndex);
	}
	else if (termType == term_input)
	{
		mainScene->GetGraphicModel()->GetPhyPortListByInSignalID(sigKey, pPortList);
	}
	//return m_PortItems.value(pPortIndex);

	for (uint i = 0; i != pPortList.size(); i++)
	{
		portItemVec.append(m_PortItems.value(pPortList.at(i)));
	}

	return portItemVec;
}

QString DrawTerminalGraphicWithDiff::GetItemIDStr()
{
	QList<QGraphicsItem*> iList = mainScene->items();
	return QString::number(iList.size());
}

void DrawTerminalGraphicWithDiff::FillVLinksToList()
{
	SCLNameTranslator translator;
	for (QVector<QVirtualLink>::Iterator j = m_virLinks.begin();j != m_virLinks.end(); ++j)
	{
		QVirtualLink a_vlink = *j;
		QString strSrcSignal = a_vlink.srcSignal;
		QString strDestSignal = a_vlink.destSignal;
		QString srcIED;
		if (m_IsDiffGraphic)
		{
			QList<QString> tList = strSrcSignal.split(".");
			srcIED = tList.at(0) + "." + tList.at(1);
		} 
		else
		{
			srcIED = translator.GetParentKey(strSrcSignal.toLocal8Bit().constData()).c_str();
		}
		QSignalInfo srcInfo, desInfo;
		mainScene->GetGraphicModel()->GetInfoOfOutSignal(strSrcSignal,&srcInfo);
		mainScene->GetGraphicModel()->GetInfoOfInSignal(strDestSignal,&desInfo);

		if (srcInfo.ID == "")
		{
			srcInfo.ID = strSrcSignal;
			srcInfo.name = strSrcSignal;
		}
		if (desInfo.ID == "")
		{
			desInfo.ID = strDestSignal;
			desInfo.name = strDestSignal;
		}
		if (desInfo.desc == "")
		{
			desInfo.desc = a_vlink.inSigInfo;
		}

		if(m_IEDKey == srcIED)//��װ��Ϊ���;
		{
			//��������ź�;
			m_vtList.push_back(srcInfo);
			m_vtDestList.push_back(desInfo);
		}
		else//�Զ�װ��Ϊ���;
		{
			m_vtList.push_back(desInfo);
			m_vtDestList.push_back(srcInfo);
		}
	}

	//����;
	if (m_IsDiffGraphic)		//���컯������;
	{
		return;
	}
	int count  = m_vtList.size();
	if (count < 1)
	{
		return;
	}
	QList<QVirtualLink> tempList = m_virLinks.toList();
	int index = 0;
	for (int i = 0; i < count - 1; i++)
	{
		index = i;
		int sOrd = m_vtList.at(i).iOrd;
		for (int j = i + 1; j < count; j++)
		{
			int dOrd = m_vtList.at(j).iOrd;
			if (sOrd > dOrd)
			{
				index = j;
				sOrd = dOrd;
			}
		}
		if (index != i)
		{
			m_vtList.swap(i,index);
			m_vtDestList.swap(i,index);
			tempList.swap(i,index);
		}
	}
	m_virLinks.clear();
	m_virLinks = tempList.toVector();
}

void DrawTerminalGraphicWithDiff::PhyPortFilter()
{
	if (m_PhyPorts.size() == 0)
	{
		return;
	}
	QVector<QString> tempPortVec;
	for (uint i = 0; i != m_vtList.size(); i++)
	{
		QSignalInfo tempInfo = m_vtList.at(i);
		for (uint j = 0; j != tempInfo.portIndexList.size(); j++)
		{
			QString tempPortName = tempInfo.portIndexList.at(j);
			for (uint iter = 0; iter != m_PhyPorts.size(); iter++)
			{
				QPhyPortInfo portInfo;
				mainScene->GetGraphicModel()->GetPhyPortInfo(m_PhyPorts.at(iter), portInfo);
				if (tempPortName.compare(portInfo.pIndex) == 0)
				{
					if (!tempPortVec.contains(m_PhyPorts.at(iter)))
					{
						tempPortVec.append(m_PhyPorts.at(iter));
					}
				}
			}
		}
	}
	m_PhyPorts.clear();
	m_PhyPorts = tempPortVec;
}

void DrawTerminalGraphicWithDiff::isDifferGraphic()
{
	QList<QString> keyList = m_IEDKey.split(".");
	if (keyList.size() == 2)
	{
		m_IsDiffGraphic = true;
	}
	else
	{
		m_IsDiffGraphic = false;
	}
}

QString DrawTerminalGraphicWithDiff::getIEDNameInDiff(QString sigKey)
{
	QList<QString> keyList = sigKey.split(".");
	return keyList.at(0) + "." + keyList.at(1);
}

bool DrawTerminalGraphicWithDiff::IsVirLinkInSameInterval(QString srcSignalKey, QString destSignalKey)
{
    SCLNameTranslator translator;
    QString srcIED = translator.GetParentKey(srcSignalKey.toLocal8Bit().constData()).c_str();
    QString destIED = translator.GetParentKey(destSignalKey.toLocal8Bit().constData()).c_str();

    if (interval->GetIEDIntervalByIEDKey(srcIED) == interval->GetIEDIntervalByIEDKey(destIED))
    {
        return true;
    }
    return false;
}

void DrawTerminalGraphicWithDiff::ShowOutSignalInTerminalGraphic()
{
    QSettings *setting = new QSettings("../cfg/SubStationInfo.ini", QSettings::IniFormat);
    m_ShowVirLinkOnly = !(setting->value("config/ShowOutSignalInTerminalGraphic").toBool());
}