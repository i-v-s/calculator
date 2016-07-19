#include "calcserver.h"
#include "calcthread.h"
#include <QJsonDocument>
#include <QTcpSocket>
#include <QDataStream>

CalcServer::CalcServer(quint16 port, QObject *parent) : QTcpServer(parent), textOut(stdout)
{
    if(!listen(QHostAddress::Any, port))
    {
        textOut << "Unable to start server" << endl;
        close();
    }
    textOut << "Calc server listen port " << serverPort() << endl;
}

void CalcServer::incomingConnection(qintptr socketDescriptor)
{
    CalcThread *thread = new CalcThread(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

