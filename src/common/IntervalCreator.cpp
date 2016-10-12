#include "IntervalCreator.h"
#include <QSettings>
#include "SCLModelSvrAdapter.h"
#include "SCLNameTranslator.h"
#include <QTextCodec>
#include "../../include/tinyxml/tinyxml.h"

IntervalCreator::IntervalCreator(QString substationKey)
{
    m_SubstationKey = substationKey;
    ReadConfigFile();
}

IntervalCreator::~IntervalCreator()
{

}

void IntervalCreator::ReadConfigFile()
{
    QSettings qSetting("../cfg/IntervalInfo.ini", QSettings::IniFormat);
    qSetting.beginGroup("IntervalType");
    qSetting.setIniCodec(QTextCodec::codecForName("GB2312")); 
    QStringList intervalTypeList = qSetting.allKeys();
    foreach(QString key, intervalTypeList)
    {
        QString value = qSetting.value(key).toString();
        m_TypeInfoMap.insert(key, value);
    }
    qSetting.endGroup();

    qSetting.beginGroup("VoltageType");
    QStringList VoltageTypeList = qSetting.allKeys();
    foreach(QString key, VoltageTypeList)
    {
        QString value = qSetting.value(key).toString();
        m_TypeInfoMap.insert(key, value);
    }
    qSetting.endGroup();
}

void IntervalCreator::SetIEDInterval()
{
    QVector<QString> iedList;
    SCLModelAdapter::GetInstance()->GetIEDList(m_SubstationKey, &iedList);
    foreach (QString iedName, iedList)
    {
        ParseIEDName(iedName);
    }

    SaveIntervalToFile();
}

void IntervalCreator::ParseIEDName(QString iedKey)
{
    QIEDInfo tempIED;
    SCLModelAdapter::GetInstance()->GetIEDInfo(iedKey, tempIED);

    IEDIntervalInfo ied;
    if (tempIED.name.size() >= 5)
    {
        QString intervalDesc;
        QString voltageDesc;
        QString number;

        QString intervalType = tempIED.name.at(1);
        QMap<QString, QString>::iterator itIntervalType = m_TypeInfoMap.find(intervalType);
        if (itIntervalType != m_TypeInfoMap.end())
        {
            intervalDesc = itIntervalType.value();
        }

        QString voltageType = tempIED.name.mid(2, 2);
        QMap<QString, QString>::iterator itVoltageType = m_TypeInfoMap.find(voltageType);
        if (itVoltageType != m_TypeInfoMap.end())
        {
            voltageDesc = itVoltageType.value();
        }

        QString sub = tempIED.name.mid(4);
        for (int i = 0; i < sub.size(); i++)
        {
            char ch = sub.at(i).toAscii();
            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
            {
                continue;
            }
            number.push_back(ch);
        }

        ied.intervalDesc = voltageDesc + intervalDesc + number;
        ied.interval = intervalType + voltageType + number;
    }
    else
    {
        ied.intervalDesc = "Î´Öª¼ä¸ô";
        ied.interval = "Unknown";
    }
    ied.name = tempIED.name;
    ied.desc = tempIED.desc;
    ied.key = iedKey;

    m_IntervalMap[ied.interval].push_back(tempIED.name);
    m_IEDInfoMap.insert(tempIED.name, ied);
}

void IntervalCreator::SaveIntervalToFile()
{
    TiXmlDocument *doc = new TiXmlDocument();
    TiXmlElement *pRoot = new TiXmlElement("IntervalInfo");
    doc->LinkEndChild(pRoot);
    
    QMap<QString, QStringList>::iterator itInterval = m_IntervalMap.begin();
    while (itInterval != m_IntervalMap.end())
    {
        TiXmlElement *pInterval = new TiXmlElement("Interval");
        pRoot->LinkEndChild(pInterval);
        pInterval->SetAttribute(QString("name").toStdString().c_str(), itInterval.key().toStdString().c_str());
        foreach (QString ied, itInterval.value())
        {
            QMap<QString, IEDIntervalInfo>::iterator itIED = m_IEDInfoMap.find(ied);
            if (itIED != m_IEDInfoMap.end())
            {
                TiXmlElement *pIED = new TiXmlElement("IED");
                pInterval->LinkEndChild(pIED);
                pIED->SetAttribute(QString("name").toStdString().c_str(), itIED.key().toStdString().c_str());
                pIED->SetAttribute(QString("key").toStdString().c_str(), itIED.value().key.toStdString().c_str());
                pIED->SetAttribute(QString("desc").toStdString().c_str(), itIED.value().desc.toStdString().c_str());
                pIED->SetAttribute(QString("intervalDesc").toStdString().c_str(), itIED.value().intervalDesc.toStdString().c_str());
                pIED->SetAttribute(QString("interval").toStdString().c_str(), itIED.value().interval.toStdString().c_str());
            }
        }
        itInterval++;
    }

    QStringList subStationList = m_SubstationKey.split('.');
    if (subStationList.size() >= 3)
    {
        QString path = "../data/" + subStationList.at(1) + "/Interval.xml";
        if (doc->SaveFile(path.toStdString().c_str()))
        {
            doc->Clear();
        }
    }

}


void IntervalCreator::GetIEDInterval()
{
    ReadIntervalFromFile();
}

void IntervalCreator::ReadIntervalFromFile()
{
    m_IEDInfoMap.clear();
    QString path;
    QStringList subStationList = m_SubstationKey.split('.');
    if (subStationList.size() >= 3)
    {
        path = "../data/" + subStationList.at(1) + "/Interval.xml";
    }
    
    TiXmlDocument *pDoc = new TiXmlDocument(path.toStdString().c_str());
    pDoc->LoadFile();
    TiXmlElement *pRoot = pDoc->RootElement();
    TiXmlNode *pInterval = pRoot->FirstChild("Interval");
    for (; pInterval != NULL; pInterval = pInterval->NextSibling("Interval"))
    {
        TiXmlNode *pIEDNode = pInterval->FirstChild("IED");
        for (; pIEDNode != NULL; pIEDNode = pIEDNode->NextSibling("IED"))
        {
            TiXmlElement *pIED = pIEDNode->ToElement();
            IEDIntervalInfo tmpIED;
            string name = pIED->Attribute("name");
            string key = pIED->Attribute("key");
            string desc = pIED->Attribute("desc");
            string intervalDesc = pIED->Attribute("intervalDesc");
            string interval = pIED->Attribute("interval");
            tmpIED.name = QString::fromStdString(name);
            tmpIED.key = QString::fromStdString(key);
            tmpIED.desc = QString::fromStdString(desc);
            tmpIED.intervalDesc = QString::fromStdString(intervalDesc);
            tmpIED.interval = QString::fromStdString(interval);
            m_IEDInfoMap.insert(tmpIED.key, tmpIED);
        }
    }
}

QString IntervalCreator::GetIEDIntervalByIEDKey(QString iedKey)
{
    if (m_IEDInfoMap.size() == 0)
    {
        GetIEDInterval();
    }

    QMap<QString, IEDIntervalInfo>::Iterator it = m_IEDInfoMap.find(iedKey);
    if (it != m_IEDInfoMap.end())
    {
        return it.value().interval;
    }
    return "Unknown";
}

