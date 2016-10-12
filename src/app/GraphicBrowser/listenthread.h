#ifndef LISTENTHREAD_H
#define LISTENTHREAD_H

#include <QThread>

class ListenThread : public QThread
{
    Q_OBJECT
public:
    explicit ListenThread(QObject *parent = 0);
    void run();

signals:
    void EmitNewProcess(QString);
    
public slots:
    
};

#endif // LISTENTHREAD_H
