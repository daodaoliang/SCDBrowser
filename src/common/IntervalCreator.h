#include <iostream>
#include <QStringList>
#include <QMap>

class IntervalCreator
{
public:
    IntervalCreator(QString substationKey);
    ~IntervalCreator();

public:
    void SetIEDInterval();
    void GetIEDInterval();
    QString GetIEDIntervalByIEDKey(QString iedKey);

private:
    void SaveIntervalToFile();
    void ReadIntervalFromFile();
    void ParseIEDName(QString iedKey);

private:
    QString m_SubstationKey;

    void ReadConfigFile(); // 从cfg的IntervalInfo.ini配置文件读取间隔类型和电压等级
    QMap<QString, QString> m_TypeInfoMap; // 保存间隔类型和电压等级信息

    struct IEDIntervalInfo 
    {
        QString key;
        QString name;
        QString desc;
        QString interval;
        QString intervalDesc;
    };
    QMap<QString, IEDIntervalInfo> m_IEDInfoMap;
    QMap<QString, QStringList> m_IntervalMap; // 记录各个间隔内的装置列表
};


