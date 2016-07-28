#include "calcthread.h"
#include "calcsockethandler.h"

CalcThread::CalcThread(int socketDescriptor, QObject *parent) :
    QThread(parent),
    socketDescriptor(socketDescriptor)
{

}

CalcThread::~CalcThread()
{
    quit();
    requestInterruption();
    wait();
}

void CalcThread::run()
{
    CalcSocketHandler handler(socketDescriptor, (int) QThread::currentThreadId());
    exec();
}


