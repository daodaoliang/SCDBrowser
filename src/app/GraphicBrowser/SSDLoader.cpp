#include "SSDLoader.h"
#include "SCLModelSvrAdapter.h"

CSSDLoader::CSSDLoader()
{
    SCLModel = SCLModelAdapter::GetInstance();
}

CSSDLoader::~CSSDLoader()
{

}

void CSSDLoader::SetCurrentSubstation(QString subStationKey)
{
    SCLModel->GetSSDInfoFromSCD("220kV." + subStationKey, m_SSDSubStationCache);
    m_IEDEquipmentMap.clear();
    GetIEDMultiMapEqupment();
}

bool CSSDLoader::IsIEDHaveEquipment(QString iedName)
{
    if (m_IEDEquipmentMap.find(iedName) != m_IEDEquipmentMap.end())
    {
        return true;
    }
    return false;
}

void CSSDLoader::GetIEDMultiMapEqupment()
{
    if (m_SSDSubStationCache.vVoltageLevel.size() > 0)
    {
        QString subStationName = m_SSDSubStationCache.name.c_str();
        for (int i = 0; i < m_SSDSubStationCache.vVoltageLevel.size(); i++)
        {
            VoltageLevelCache *vlCache = &(m_SSDSubStationCache.vVoltageLevel.at(i));
            QString voltageLevel = vlCache->name.c_str();
            for (BayCacheMap::iterator it = vlCache->bayMap.begin(); it != vlCache->bayMap.end(); it++)
            {
                BayCache *bay = &it->second;
                QString bayName = bay->name.c_str();
                // 遍历Pt
                PowerTransformerCacheMap::iterator itPt = bay->ptMap.begin();
                while (itPt != bay->ptMap.end())
                {
                    QPowerTransformerCache ptTemp;
                    ptTemp.name = itPt->second.name.c_str();
                    ptTemp.desc = itPt->second.desc.c_str();

                    // 更新TransformWinding
                    TransformerWindingCacheMap::iterator itTw = itPt->second.tfWindingMap.begin();
                    while (itTw != itPt->second.tfWindingMap.end())
                    {
                        QTransformerWindingCache twTemp;
                        twTemp.name = itTw->second.name.c_str();
                        twTemp.desc = itTw->second.desc.c_str();

                        // 更新LNode
                        LNodeCacheMap::iterator itLn = itTw->second.lnMap.begin();
                        while (itLn != itTw->second.lnMap.end())
                        {
                            QString iedName = itLn->second.iedName.c_str();

                            EquipmentInfo tempEq;
                            tempEq.subStationName = subStationName;
                            tempEq.voltageLevel = voltageLevel;
                            tempEq.bayInfo = bayName;
                            tempEq.eqipmentName = twTemp.name;
                            tempEq.eqipmentDesc = twTemp.desc;

                            m_IEDEquipmentMap.insertMulti(iedName, tempEq);
                            itLn++;
                        }
                        itTw++;
                    }

                    // 更新LNode
                    LNodeCacheMap::iterator itLn = itPt->second.lnodeMap.begin();
                    while (itLn != itPt->second.lnodeMap.end())
                    {
                        QString iedName = itLn->second.iedName.c_str();

                        EquipmentInfo tempEq;
                        tempEq.subStationName = subStationName;
                        tempEq.voltageLevel = voltageLevel;
                        tempEq.bayInfo = bayName;
                        tempEq.eqipmentName = ptTemp.name;
                        tempEq.eqipmentDesc = ptTemp.desc;

                        m_IEDEquipmentMap.insertMulti(iedName, tempEq);
                        itLn++;
                    }
                    itPt++;
                }
                // 遍历Ce
                ConductingEquipmentCacheMap::iterator itCe = bay->ceMap.begin();
                while (itCe != bay->ceMap.end())
                {
                    QConductingEquipmentCache ceTemp;
                    ceTemp.name = itCe->second.name.c_str();
                    ceTemp.desc = itCe->second.desc.c_str();

                    // 更新LNode
                    LNodeCacheMap::iterator itLn = itCe->second.lnMap.begin();
                    while (itLn != itCe->second.lnMap.end())
                    {
                        QString iedName = itLn->second.iedName.c_str();

                        EquipmentInfo tempEq;
                        tempEq.subStationName = subStationName;
                        tempEq.voltageLevel = voltageLevel;
                        tempEq.bayInfo = bayName;
                        tempEq.eqipmentName = ceTemp.name;
                        tempEq.eqipmentDesc = ceTemp.desc;

                        m_IEDEquipmentMap.insertMulti(iedName, tempEq);

                        itLn++;
                    }

                    itCe++;
                }

            }
        }        
    }
}

bool CSSDLoader::GetSubStationInfo(QSSDSubstationCache &ssdInfo)
{
    ssdInfo.name = m_SSDSubStationCache.name.c_str();
    ssdInfo.desc = m_SSDSubStationCache.desc.c_str();
    ssdInfo.dtype = m_SSDSubStationCache.dtype.c_str();
    ssdInfo.areaName = m_SSDSubStationCache.areaName.c_str();
    ssdInfo.areaDesc = m_SSDSubStationCache.areaDesc.c_str();

    return true;
}

bool CSSDLoader::GetSSDVoltageLevelInfoByKey(QString voltageLevel, QVoltageLevelCache &vlInfo)
{
    if (m_SSDSubStationCache.vVoltageLevel.size() > 0)
    {
        for (int i = 0; i < m_SSDSubStationCache.vVoltageLevel.size(); i++)
        {
            if (voltageLevel == m_SSDSubStationCache.vVoltageLevel.at(i).name.c_str())
            {
                vlInfo.name = m_SSDSubStationCache.vVoltageLevel.at(i).name.c_str();
                vlInfo.desc = m_SSDSubStationCache.vVoltageLevel.at(i).desc.c_str();
                vlInfo.voltage.multiplier = m_SSDSubStationCache.vVoltageLevel.at(i).voltage.multiplier.c_str();
                vlInfo.voltage.unit = m_SSDSubStationCache.vVoltageLevel.at(i).voltage.unit.c_str();
                vlInfo.voltage.text = m_SSDSubStationCache.vVoltageLevel.at(i).voltage.text.c_str();
                return true;
            }
        }
        
    }
    return false;
}

bool CSSDLoader::GetVoltagelevelListFromSSDInfo(QVector<QString> &vlList)
{
    if (m_SSDSubStationCache.vVoltageLevel.size() > 0)
    {
        for (int i = 0; i < m_SSDSubStationCache.vVoltageLevel.size(); i++)
        {
            vlList.append(m_SSDSubStationCache.vVoltageLevel.at(i).name.c_str());
        }
        return true;
    }
    return false;
}

bool CSSDLoader::GetBayListByVoltageLevel(QString voltageLevel, QMap<QString, QString> &bayList)
{
    if (m_SSDSubStationCache.vVoltageLevel.size() > 0)
    {
        for (int i = 0; i < m_SSDSubStationCache.vVoltageLevel.size(); i++)
        {
            if (m_SSDSubStationCache.vVoltageLevel.at(i).name.c_str() == voltageLevel)
            {
                VoltageLevelCache *vlCache = &(m_SSDSubStationCache.vVoltageLevel.at(i));
                for (BayCacheMap::iterator it = vlCache->bayMap.begin(); it != vlCache->bayMap.end(); it++)
                {
                    QString bayName = it->second.name.c_str();
                    QString bayDesc = it->second.desc.c_str();
                    bayList.insert(bayName, bayDesc);
                }
                return true;
            }            
        }        
    }
    return false;
}

bool CSSDLoader::GetBayInfoByBayKey(QString voltageLevel, QString bayName, QBayCache &bayInfo)
{
    if (m_SSDSubStationCache.vVoltageLevel.size() > 0)
    {
        for (int i = 0; i < m_SSDSubStationCache.vVoltageLevel.size(); i++)
        {
            if (m_SSDSubStationCache.vVoltageLevel.at(i).name.c_str() == voltageLevel)
            {
                VoltageLevelCache *vlCache = &(m_SSDSubStationCache.vVoltageLevel.at(i));
                for (BayCacheMap::iterator it = vlCache->bayMap.begin(); it != vlCache->bayMap.end(); it++)
                {
                    BayCache *bay = &it->second;
                    if (bay->name.c_str() == bayName)
                    {
                        bayInfo.name = bay->name.c_str();
                        bayInfo.desc = bay->desc.c_str();
                        bayInfo.dtype = bay->dtype.c_str();
                        return true;
                    }
                }
            }            
        }        
    }
    return false;
}

bool CSSDLoader::GetPowerTransformByBay(QString voltageLevel, QString bayName, QVector<QPowerTransformerCache> &ptVec)
{
    if (m_SSDSubStationCache.vVoltageLevel.size() > 0)
    {
        for (int i = 0; i < m_SSDSubStationCache.vVoltageLevel.size(); i++)
        {
            if (m_SSDSubStationCache.vVoltageLevel.at(i).name.c_str() == voltageLevel)
            {
                VoltageLevelCache *vlCache = &(m_SSDSubStationCache.vVoltageLevel.at(i));
                for (BayCacheMap::iterator it = vlCache->bayMap.begin(); it != vlCache->bayMap.end(); it++)
                {
                    BayCache *bay = &it->second;
                    if (bay->name.c_str() == bayName)
                    {
                        PowerTransformerCacheMap::iterator itPt = bay->ptMap.begin();
                        while (itPt != bay->ptMap.end())
                        {
                            QPowerTransformerCache ptTemp;
                            ptTemp.name = itPt->second.name.c_str();
                            ptTemp.desc = itPt->second.desc.c_str();
                            ptTemp.vir = itPt->second.vir.c_str();
                            ptTemp.type = itPt->second.type.c_str();

                            // 更新TransformWinding
                            TransformerWindingCacheMap::iterator itTw = itPt->second.tfWindingMap.begin();
                            while (itTw != itPt->second.tfWindingMap.end())
                            {
                                QTransformerWindingCache twTemp;
                                twTemp.name = itTw->second.name.c_str();
                                twTemp.desc = itTw->second.desc.c_str();
                                twTemp.vir = itTw->second.vir.c_str();
                                twTemp.type = itTw->second.type.c_str();

                                // 更新Terminal
                                TerminalCacheMap::iterator itTer = itTw->second.terminalMap.begin();
                                while (itTer != itTw->second.terminalMap.end())
                                {
                                    QTerminalCache terTemp;
                                    terTemp.substationName = itTer->second.substationName.c_str();
                                    terTemp.voltageLevelName = itTer->second.voltageLevelName.c_str();
                                    terTemp.bayName = itTer->second.bayName.c_str();
                                    terTemp.cnodeName = itTer->second.cnodeName.c_str();
                                    terTemp.connectivityNode = itTer->second.connectivityNode.c_str();
                                    terTemp.name = itTer->second.name.c_str();
                                    terTemp.desc = itTer->second.desc.c_str();

                                    twTemp.terminalVec.append(terTemp);
                                    itTer++;
                                }

                                // 更新LNode
                                LNodeCacheMap::iterator itLn = itTw->second.lnMap.begin();
                                while (itLn != itTw->second.lnMap.end())
                                {
                                    QLNodeCache lnTemp;
                                    lnTemp.iedName = itLn->second.iedName.c_str();
                                    lnTemp.ldInst = itLn->second.ldInst.c_str();
                                    lnTemp.prefix = itLn->second.prefix.c_str();
                                    lnTemp.lnClass = itLn->second.lnClass.c_str();
                                    lnTemp.lnInst = itLn->second.lnInst.c_str();
                                    lnTemp.lnType = itLn->second.lnType.c_str();
                                    // lnTemp.key = itLn->second.iedName.c_str() + "." + itLn->second.ldInst.c_str() + "/" + itLn->second.prefix.c_str() + itLn->second.lnClass.c_str() + itLn->second.lnInst.c_str();
                                    lnTemp.key = lnTemp.iedName + "." + lnTemp.ldInst + "/" + lnTemp.prefix+lnTemp.lnClass+lnTemp.lnInst;
                                    twTemp.lnVec.append(lnTemp);
                                    itLn++;
                                }

                                ptTemp.tfWindingVec.append(twTemp);
                                itTw++;
                            }

                            // 更新LNode
                            LNodeCacheMap::iterator itLn = itPt->second.lnodeMap.begin();
                            while (itLn != itPt->second.lnodeMap.end())
                            {
                                QLNodeCache lnTemp;
                                lnTemp.iedName = itLn->second.iedName.c_str();
                                lnTemp.ldInst = itLn->second.ldInst.c_str();
                                lnTemp.prefix = itLn->second.prefix.c_str();
                                lnTemp.lnClass = itLn->second.lnClass.c_str();
                                lnTemp.lnInst = itLn->second.lnInst.c_str();
                                lnTemp.lnType = itLn->second.lnType.c_str();
                                lnTemp.key = lnTemp.iedName + "." + lnTemp.ldInst + "/" + lnTemp.prefix+lnTemp.lnClass+lnTemp.lnInst;
                                ptTemp.lnodeVec.append(lnTemp);
                                itLn++;
                            }

                            ptVec.append(ptTemp);
                            itPt++;
                        }
                        return true;
                    }
                }
            }            
        }        
    }
    return false;
}

bool CSSDLoader::GetConductingEquipmentByBay(QString voltageLevel, QString bayName, QConductingEquipmentCacheVec &ceVec)
{
    if (m_SSDSubStationCache.vVoltageLevel.size() > 0)
    {
        for (int i = 0; i < m_SSDSubStationCache.vVoltageLevel.size(); i++)
        {
            if (m_SSDSubStationCache.vVoltageLevel.at(i).name.c_str() == voltageLevel)
            {
                VoltageLevelCache *vlCache = &(m_SSDSubStationCache.vVoltageLevel.at(i));
                for (BayCacheMap::iterator it = vlCache->bayMap.begin(); it != vlCache->bayMap.end(); it++)
                {
                    BayCache *bay = &it->second;
                    if (bay->name.c_str() == bayName)
                    {
                        ConductingEquipmentCacheMap::iterator itCe = bay->ceMap.begin();
                        while (itCe != bay->ceMap.end())
                        {
                            QConductingEquipmentCache ceTemp;
                            ceTemp.name = itCe->second.name.c_str();
                            ceTemp.desc = itCe->second.desc.c_str();
                            ceTemp.vir = itCe->second.vir.c_str();
                            ceTemp.type = itCe->second.type.c_str();

                            // 更新Terminal
                            TerminalCacheMap::iterator itTer = itCe->second.terminalMap.begin();
                            while (itTer != itCe->second.terminalMap.end())
                            {
                                QTerminalCache terTemp;
                                terTemp.substationName = itTer->second.substationName.c_str();
                                terTemp.voltageLevelName = itTer->second.voltageLevelName.c_str();
                                terTemp.bayName = itTer->second.bayName.c_str();
                                terTemp.cnodeName = itTer->second.cnodeName.c_str();
                                terTemp.connectivityNode = itTer->second.connectivityNode.c_str();
                                terTemp.name = itTer->second.name.c_str();
                                terTemp.desc = itTer->second.desc.c_str();

                                ceTemp.terminalVec.append(terTemp);
                                itTer++;
                            }

                            // 更新LNode
                            LNodeCacheMap::iterator itLn = itCe->second.lnMap.begin();
                            while (itLn != itCe->second.lnMap.end())
                            {
                                QLNodeCache lnTemp;
                                lnTemp.iedName = itLn->second.iedName.c_str();
                                lnTemp.ldInst = itLn->second.ldInst.c_str();
                                lnTemp.prefix = itLn->second.prefix.c_str();
                                lnTemp.lnClass = itLn->second.lnClass.c_str();
                                lnTemp.lnInst = itLn->second.lnInst.c_str();
                                lnTemp.lnType = itLn->second.lnType.c_str();
                                lnTemp.key = lnTemp.iedName + "." + lnTemp.ldInst + "/" + lnTemp.prefix+lnTemp.lnClass+lnTemp.lnInst;
                                ceTemp.lnVec.append(lnTemp);
                                itLn++;
                            }

                            ceVec.append(ceTemp);
                            itCe++;
                        }
                        return true;
                    }
                }
            }            
        }        
    }
    return false;
}

bool CSSDLoader::GetConnectivityNodeByBay(QString voltageLevel, QString bayName, QConnectivityNodeCacheVec &cnVec)
{
    if (m_SSDSubStationCache.vVoltageLevel.size() > 0)
    {
        for (int i = 0; i < m_SSDSubStationCache.vVoltageLevel.size(); i++)
        {
            if (m_SSDSubStationCache.vVoltageLevel.at(i).name.c_str() == voltageLevel)
            {
                VoltageLevelCache *vlCache = &(m_SSDSubStationCache.vVoltageLevel.at(i));
                for (BayCacheMap::iterator it = vlCache->bayMap.begin(); it != vlCache->bayMap.end(); it++)
                {
                    BayCache *bay = &it->second;
                    if (bay->name.c_str() == bayName)
                    {
                        ConnectivityNodeCacheMap::iterator itCN = bay->cnMap.begin();
                        while (itCN != bay->cnMap.end())
                        {
                            QConnectivityNodeCache cnTemp;
                            cnTemp.name = itCN->second.name.c_str();
                            cnTemp.desc = itCN->second.desc.c_str();
                            cnTemp.pathname = itCN->second.pathname.c_str();

                            cnVec.append(cnTemp);
                            itCN++;
                        }
                        return true;
                    }
                }
            }            
        }        
    }
    return false;
}








