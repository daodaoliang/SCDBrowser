#include "regmanager.h"
#include <QDebug>

RegManager::RegManager()
{
    m_HKey = "HKEY_CURRENT_USER\\Software\\Roof\\SCDBrowserArg";
    m_Setting = new QSettings(m_HKey, QSettings::NativeFormat);
}

RegManager::RegManager(QString key)
{
    m_HKey = key;
    m_Setting = new QSettings(m_HKey, QSettings::NativeFormat);
}

RegManager::~RegManager()
{
    delete m_Setting;
}


void RegManager::WriteRegValue(QString argv)
{
    m_Setting->setValue("arg", QVariant(argv));
    qDebug() << "RegManager::WriteRegValue" << argv;
}

QString RegManager::ReadRegValue()
{
    return m_Setting->value("arg").toString();
    qDebug() << "RegManager::ReadRegValue";
}

void RegManager::DeleteRegValue()
{
    m_Setting->remove("arg");
    qDebug() << "RegManager::DeleteRegValue";
}
