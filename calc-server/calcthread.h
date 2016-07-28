#ifndef CALCTHREAD_H
#define CALCTHREAD_H
#include <QThread>
#include <QTcpSocket>
#include <QDataStream>
#include <QScopedPointer>

//! Класс потока, обслуживающего соединение с клиентом
class CalcThread : public QThread
{
    Q_OBJECT
    const int socketDescriptor;
public:
    CalcThread(int socketDescriptor, QObject *parent);
    ~CalcThread();
    void run() Q_DECL_OVERRIDE;
};
#endif // CALCTHREAD_H
