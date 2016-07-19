#include "calcclient.h"
#include <QAbstractSocket>

CalcClient::CalcClient(QObject *parent) : QObject(parent)
{
    input.setDevice(&tcpSocket);
    input.setVersion(QDataStream::Qt_5_7);
    connect(&tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(&tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

}

void CalcClient::connectRemote(const QString &host, int port)
{
    tcpSocket.connectToHost(host, port);
}

void CalcClient::disconnect()
{
    tcpSocket.disconnectFromHost();
}

QString CalcClient::peer()
{
    return tcpSocket.peerName() + ":" + QString::number(tcpSocket.peerPort());
}

void CalcClient::sendExpression(const QString &expression)
{
    input << expression;
}

void CalcClient::error(QAbstractSocket::SocketError socketError)
{
    switch(socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        onSocketError("Соединение закрыто");
        break;
    case QAbstractSocket::HostNotFoundError:
        onSocketError("Сервер не найден");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        onSocketError("Соединение отклонено");
        break;
    default:
        onSocketError(tcpSocket.errorString());
    }
    tcpSocket.abort();
}

void CalcClient::readData()
{
    input.startTransaction();

    QString status, data;
    input >> status;
    if(status != "CONNECTED")
        input >> data;

    if (!input.commitTransaction())
        return;
    if(status == "ERROR")
        onCalcError(data);
    else if(status == "OK")
        onResult(data);
}


