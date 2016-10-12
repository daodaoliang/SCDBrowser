#ifndef SSDLOADER_H
#define SSDLOADER_H

#include "SCLModelSvrAdapterInterface.h"
#include <QMultiMap>

struct EquipmentInfo
{
    QString subStationName;
    QString voltageLevel;
    QString bayInfo;
    QString eqipmentName;
    QString eqipmentDesc;
};

class CSSDLoader{
public:
    CSSDLoader();
    ~CSSDLoader();


    void SetCurrentSubstation(QString subStationKey);
    void GetIEDMultiMapEqupment(); // ȡ��IED��Ӧһ���豸���б�
    bool IsIEDHaveEquipment(QString iedName);
        
    bool GetSubStationInfo(QSSDSubstationCache &ssdInfo);
    bool GetSSDVoltageLevelInfoByKey(QString voltageLevel, QVoltageLevelCache &vlInfo);
    bool GetVoltagelevelListFromSSDInfo(QVector<QString> &vlList); // ȡ�õ�ѹ�ȼ��б�
    bool GetBayListByVoltageLevel(QString voltageLevel, QMap<QString, QString> &bayList);
    bool GetBayInfoByBayKey(QString voltageLevel, QString bayName, QBayCache &bayInfo);
    bool GetPowerTransformByBay(QString voltageLevel, QString bayName, QVector<QPowerTransformerCache> &ptVec);
    bool GetConductingEquipmentByBay(QString voltageLevel, QString bayName, QConductingEquipmentCacheVec &ceVec);
    bool GetConnectivityNodeByBay(QString voltageLevel, QString bayName, QConnectivityNodeCacheVec &cnVec);

public:
    SCLModelSvrAdapterInterface   *SCLModel;
    SubstationCache               m_SSDSubStationCache;

    QMap<QString, EquipmentInfo> m_IEDEquipmentMap; // IED ��Ӧһ���豸��Ϣ
};

#endif