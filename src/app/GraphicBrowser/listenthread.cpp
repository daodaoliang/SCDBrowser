#include "listenthread.h"
#include "regmanager.h"
#include <QDebug>

ListenThread::ListenThread(QObject *parent) :
    QThread(parent)
{
}

void ListenThread::run()
{
    qDebug() << "ListenThread::run....";
    RegManager reg;
    QString value;
    while(1)
    {
        value = reg.ReadRegValue();
        if (value.size() > 0)
        {
            qDebug() << "ListenThread Listtend a new arg";
            emit EmitNewProcess(value);
        }
        msleep(500);
    }
}
