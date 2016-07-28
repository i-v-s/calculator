#include "calcclient.h"
#include <QAbstractSocket>

CalcClient::CalcClient(QObject *parent) : QObject(parent), receivingSize(0)
{
    connect(&tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(&tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
}

CalcClient::~CalcClient()
{
    tcpSocket.disconnectFromHost();
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
    QByteArray bytes;
    QDataStream outputStream(&bytes, QIODevice::WriteOnly);
    outputStream.setVersion(QDataStream::Qt_5_7);
    outputStream << expression;
    quint16 size = bytes.size();
    tcpSocket.write(reinterpret_cast<char *>(&size), sizeof(size));
    tcpSocket.write(bytes);
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
    while(size_t available = tcpSocket.bytesAvailable())
        if(receivingSize)
        {
            if(available < sizeof(receivingSize)) break;
            QByteArray bytes = tcpSocket.read(receivingSize);
            if((size_t) bytes.size() != receivingSize)
            {
                onSocketError("Ошибка чтения из сокета");
                tcpSocket.abort();
                break;
            }
            QDataStream inputStream(bytes);
            inputStream.setVersion(QDataStream::Qt_5_7);
            QString status, result;
            inputStream >> status;
            inputStream >> result;
            if(status == "OK")
                onResult(result);
            else if(status == "ERROR")
                onCalcError(result);
            else if(status != "CONNECTED")
                onCalcError(result);
            receivingSize = 0;
        }
        else
        {
            quint16 size;
            if(available < sizeof(size)) break;
            if(tcpSocket.read(reinterpret_cast<char *>(&size), sizeof(size)) != sizeof(size))
            {
                onSocketError("Ошибка чтения из сокета");
                tcpSocket.abort();
                break;
            }
            receivingSize = size;
        }
}


