#ifndef REGMANAGER_H
#define REGMANAGER_H

#include <QString>
#include <QSettings>

class RegManager
{
public:
    RegManager(QString key);
    RegManager();
    ~RegManager();

    void WriteRegValue(QString argv);
    QString ReadRegValue();
    void DeleteRegValue();

private:
    QString m_HKey;
    QSettings *m_Setting;

};

#endif // REGMANAGER_H
