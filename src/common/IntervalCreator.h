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

    void ReadConfigFile(); // ��cfg��IntervalInfo.ini�����ļ���ȡ������ͺ͵�ѹ�ȼ�
    QMap<QString, QString> m_TypeInfoMap; // ���������ͺ͵�ѹ�ȼ���Ϣ

    struct IEDIntervalInfo 
    {
        QString key;
        QString name;
        QString desc;
        QString interval;
        QString intervalDesc;
    };
    QMap<QString, IEDIntervalInfo> m_IEDInfoMap;
    QMap<QString, QStringList> m_IntervalMap; // ��¼��������ڵ�װ���б�
};


